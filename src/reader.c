#include "bxf.h"

#include <stdlib.h>
#include <string.h>


BxfResult bxf_reader_open(BxfReader *reader, const char *path)
{
    if (reader == NULL || path == NULL)
        return BXF_INVALID_ARGUMENT;

    reader->file = fopen(path, "rb");

    if (reader->file == NULL)
        return BXF_IO_ERROR;

    /* Magic */

    char magic[BXF_MAGIC_SIZE];

    if (fread(magic, 1, BXF_MAGIC_SIZE, reader->file)
        != BXF_MAGIC_SIZE)
    {
        fclose(reader->file);
        reader->file = NULL;
        return BXF_INVALID_FILE;
    }

    if (memcmp(magic, BXF_MAGIC, BXF_MAGIC_SIZE) != 0)
    {
        fclose(reader->file);
        reader->file = NULL;
        return BXF_INVALID_FILE;
    }

    /* Version */

    uint8_t version[4];

    if (fread(version, 1, sizeof(version), reader->file)
        != sizeof(version))
    {
        fclose(reader->file);
        reader->file = NULL;
        return BXF_INVALID_FILE;
    }

    uint16_t major =
        (uint16_t)version[0] |
        ((uint16_t)version[1] << 8);

    uint16_t minor =
        (uint16_t)version[2] |
        ((uint16_t)version[3] << 8);

    if (major != BXF_VERSION_MAJOR ||
        minor > BXF_VERSION_MINOR)
    {
        fclose(reader->file);
        reader->file = NULL;
        return BXF_UNSUPPORTED_VERSION;
    }

    return BXF_OK;
}


BxfResult bxf_reader_close(BxfReader *reader)
{
    if (reader == NULL || reader->file == NULL)
        return BXF_INVALID_ARGUMENT;

    if (fclose(reader->file) != 0)
    {
        reader->file = NULL;
        return BXF_IO_ERROR;
    }

    reader->file = NULL;

    return BXF_OK;
}


/* ============================================================
 * Low-level read functions
 * ============================================================ */

BxfResult bxf_read_u8(BxfReader *reader, uint8_t *value)
{
    if (reader == NULL || reader->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    int byte = fgetc(reader->file);

    if (byte == EOF)
    {
        if (feof(reader->file))
            return BXF_END_OF_FILE;

        return BXF_IO_ERROR;
    }

    *value = (uint8_t)byte;

    return BXF_OK;
}


BxfResult bxf_read_u16(BxfReader *reader, uint16_t *value)
{
    if (reader == NULL || reader->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    uint8_t bytes[2];

    if (fread(bytes, 1, sizeof(bytes), reader->file)
        != sizeof(bytes))
    {
        return feof(reader->file)
            ? BXF_END_OF_FILE
            : BXF_IO_ERROR;
    }

    *value =
        (uint16_t)bytes[0] |
        ((uint16_t)bytes[1] << 8);

    return BXF_OK;
}


BxfResult bxf_read_u32(BxfReader *reader, uint32_t *value)
{
    if (reader == NULL || reader->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    uint8_t bytes[4];

    if (fread(bytes, 1, sizeof(bytes), reader->file)
        != sizeof(bytes))
    {
        return feof(reader->file)
            ? BXF_END_OF_FILE
            : BXF_IO_ERROR;
    }

    *value =
        (uint32_t)bytes[0] |
        ((uint32_t)bytes[1] << 8) |
        ((uint32_t)bytes[2] << 16) |
        ((uint32_t)bytes[3] << 24);

    return BXF_OK;
}


BxfResult bxf_read_u64(BxfReader *reader, uint64_t *value)
{
    if (reader == NULL || reader->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    uint8_t bytes[8];

    if (fread(bytes, 1, sizeof(bytes), reader->file)
        != sizeof(bytes))
    {
        return feof(reader->file)
            ? BXF_END_OF_FILE
            : BXF_IO_ERROR;
    }

    *value = 0;

    for (size_t i = 0; i < sizeof(bytes); ++i)
        *value |= (uint64_t)bytes[i] << (i * 8);

    return BXF_OK;
}


BxfResult bxf_read_i8(BxfReader *reader, int8_t *value)
{
    if (reader == NULL || reader->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    uint8_t bits;

    BxfResult result = bxf_read_u8(reader, &bits);

    if (result != BXF_OK)
        return result;

    memcpy(value, &bits, sizeof(bits));

    return BXF_OK;
}


BxfResult bxf_read_i16(BxfReader *reader, int16_t *value)
{
    if (reader == NULL || reader->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    uint16_t bits;

    BxfResult result = bxf_read_u16(reader, &bits);

    if (result != BXF_OK)
        return result;

    memcpy(value, &bits, sizeof(bits));

    return BXF_OK;
}


BxfResult bxf_read_i32(BxfReader *reader, int32_t *value)
{
    if (reader == NULL || reader->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    uint32_t bits;

    BxfResult result = bxf_read_u32(reader, &bits);

    if (result != BXF_OK)
        return result;

    memcpy(value, &bits, sizeof(bits));

    return BXF_OK;
}


BxfResult bxf_read_i64(BxfReader *reader, int64_t *value)
{
    if (reader == NULL || reader->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    uint64_t bits;

    BxfResult result = bxf_read_u64(reader, &bits);

    if (result != BXF_OK)
        return result;

    memcpy(value, &bits, sizeof(bits));

    return BXF_OK;
}


BxfResult bxf_read_float(BxfReader *reader, float *value)
{
    if (reader == NULL || reader->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    if (sizeof(float) != sizeof(uint32_t))
        return BXF_ERROR;

    uint32_t bits;

    BxfResult result = bxf_read_u32(reader, &bits);

    if (result != BXF_OK)
        return result;

    memcpy(value, &bits, sizeof(bits));

    return BXF_OK;
}


BxfResult bxf_read_double(BxfReader *reader, double *value)
{
    if (reader == NULL || reader->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    if (sizeof(double) != sizeof(uint64_t))
        return BXF_ERROR;

    uint64_t bits;

    BxfResult result = bxf_read_u64(reader, &bits);

    if (result != BXF_OK)
        return result;

    memcpy(value, &bits, sizeof(bits));

    return BXF_OK;
}


BxfResult bxf_read_string(BxfReader *reader, char **value)
{
    if (reader == NULL || reader->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    uint32_t length;

    BxfResult result = bxf_read_u32(reader, &length);

    if (result != BXF_OK)
        return result;

    char *string = malloc((size_t)length + 1);

    if (string == NULL)
        return BXF_OUT_OF_MEMORY;

    if (length > 0)
    {
        if (fread(string, 1, length, reader->file) != length)
        {
            free(string);

            return feof(reader->file)
                ? BXF_END_OF_FILE
                : BXF_IO_ERROR;
        }
    }

    string[length] = '\0';

    *value = string;

    return BXF_OK;
}


/* ============================================================
 * High-level API
 * ============================================================ */

BxfResult bxf_read(BxfReader *reader, BxfValue *value)
{
    if (reader == NULL || reader->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    uint8_t type;

    BxfResult result = bxf_read_u8(reader, &type);

    if (result != BXF_OK)
        return result;

    if (type < BXF_TYPE_U8 || type > BXF_TYPE_STRING)
        return BXF_INVALID_TYPE;

    value->type = (BxfType)type;

    switch (value->type)
    {
        case BXF_TYPE_U8:
            return bxf_read_u8(reader, &value->u8);

        case BXF_TYPE_U16:
            return bxf_read_u16(reader, &value->u16);

        case BXF_TYPE_U32:
            return bxf_read_u32(reader, &value->u32);

        case BXF_TYPE_U64:
            return bxf_read_u64(reader, &value->u64);

        case BXF_TYPE_I8:
            return bxf_read_i8(reader, &value->i8);

        case BXF_TYPE_I16:
            return bxf_read_i16(reader, &value->i16);

        case BXF_TYPE_I32:
            return bxf_read_i32(reader, &value->i32);

        case BXF_TYPE_I64:
            return bxf_read_i64(reader, &value->i64);

        case BXF_TYPE_FLOAT:
            return bxf_read_float(reader, &value->float32);

        case BXF_TYPE_DOUBLE:
            return bxf_read_double(reader, &value->float64);

        case BXF_TYPE_STRING:
            return bxf_read_string(reader, &value->string);

        default:
            return BXF_INVALID_TYPE;
    }
}


void bxf_value_free(BxfValue *value)
{
    if (value == NULL)
        return;

    if (value->type == BXF_TYPE_STRING)
        free(value->string);

    memset(value, 0, sizeof(*value));
}