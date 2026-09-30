#include "bxf.h"

#include <string.h>

BxfResult bxf_writer_open(BxfWriter *writer, const char *path)
{
    if (writer == NULL || path == NULL)
        return BXF_INVALID_ARGUMENT;

    writer->file = fopen(path, "wb");

    if (writer->file == NULL)
        return BXF_IO_ERROR;

    /* Magic */
    if (fwrite(BXF_MAGIC, 1, BXF_MAGIC_SIZE, writer->file)
        != BXF_MAGIC_SIZE)
    {
        fclose(writer->file);
        writer->file = NULL;
        return BXF_IO_ERROR;
    }

    /* Version */
    uint8_t version[4];

    version[0] = (uint8_t)BXF_VERSION_MAJOR;
    version[1] = (uint8_t)(BXF_VERSION_MAJOR >> 8);
    version[2] = (uint8_t)BXF_VERSION_MINOR;
    version[3] = (uint8_t)(BXF_VERSION_MINOR >> 8);

    if (fwrite(version, 1, sizeof(version), writer->file)
        != sizeof(version))
    {
        fclose(writer->file);
        writer->file = NULL;
        return BXF_IO_ERROR;
    }

    return BXF_OK;
}

BxfResult bxf_writer_close(BxfWriter *writer)
{
    if (writer == NULL || writer->file == NULL)
        return BXF_INVALID_ARGUMENT;

    if (fclose(writer->file) != 0)
    {
        writer->file = NULL;
        return BXF_IO_ERROR;
    }

    writer->file = NULL;

    return BXF_OK;
}


/* ============================================================
 * Low-level write functions
 * ============================================================ */

BxfResult bxf_write_u8(BxfWriter *writer, uint8_t value)
{
    if (writer == NULL || writer->file == NULL)
        return BXF_INVALID_ARGUMENT;

    if (fputc((int)value, writer->file) == EOF)
        return BXF_IO_ERROR;

    return BXF_OK;
}

BxfResult bxf_write_u16(BxfWriter *writer, uint16_t value)
{
    if (writer == NULL || writer->file == NULL)
        return BXF_INVALID_ARGUMENT;

    uint8_t bytes[2];

    bytes[0] = (uint8_t)value;
    bytes[1] = (uint8_t)(value >> 8);

    if (fwrite(bytes, 1, sizeof(bytes), writer->file)
        != sizeof(bytes))
    {
        return BXF_IO_ERROR;
    }

    return BXF_OK;
}

BxfResult bxf_write_u32(BxfWriter *writer, uint32_t value)
{
    if (writer == NULL || writer->file == NULL)
        return BXF_INVALID_ARGUMENT;

    uint8_t bytes[4];

    bytes[0] = (uint8_t)value;
    bytes[1] = (uint8_t)(value >> 8);
    bytes[2] = (uint8_t)(value >> 16);
    bytes[3] = (uint8_t)(value >> 24);

    if (fwrite(bytes, 1, sizeof(bytes), writer->file)
        != sizeof(bytes))
    {
        return BXF_IO_ERROR;
    }

    return BXF_OK;
}

BxfResult bxf_write_u64(BxfWriter *writer, uint64_t value)
{
    if (writer == NULL || writer->file == NULL)
        return BXF_INVALID_ARGUMENT;

    uint8_t bytes[8];

    for (size_t i = 0; i < sizeof(bytes); ++i)
        bytes[i] = (uint8_t)(value >> (i * 8));

    if (fwrite(bytes, 1, sizeof(bytes), writer->file)
        != sizeof(bytes))
    {
        return BXF_IO_ERROR;
    }

    return BXF_OK;
}

BxfResult bxf_write_i8(BxfWriter *writer, int8_t value)
{
    return bxf_write_u8(writer, (uint8_t)value);
}

BxfResult bxf_write_i16(BxfWriter *writer, int16_t value)
{
    return bxf_write_u16(writer, (uint16_t)value);
}

BxfResult bxf_write_i32(BxfWriter *writer, int32_t value)
{
    return bxf_write_u32(writer, (uint32_t)value);
}

BxfResult bxf_write_i64(BxfWriter *writer, int64_t value)
{
    return bxf_write_u64(writer, (uint64_t)value);
}

BxfResult bxf_write_float(BxfWriter *writer, float value)
{
    if (writer == NULL || writer->file == NULL)
        return BXF_INVALID_ARGUMENT;

    if (sizeof(float) != sizeof(uint32_t))
        return BXF_ERROR;

    uint32_t bits;

    memcpy(&bits, &value, sizeof(bits));

    return bxf_write_u32(writer, bits);
}

BxfResult bxf_write_double(BxfWriter *writer, double value)
{
    if (writer == NULL || writer->file == NULL)
        return BXF_INVALID_ARGUMENT;

    if (sizeof(double) != sizeof(uint64_t))
        return BXF_ERROR;

    uint64_t bits;

    memcpy(&bits, &value, sizeof(bits));

    return bxf_write_u64(writer, bits);
}

BxfResult bxf_write_string(BxfWriter *writer, const char *value)
{
    if (writer == NULL || writer->file == NULL || value == NULL)
        return BXF_INVALID_ARGUMENT;

    size_t length = strlen(value);

    if (length > UINT32_MAX)
        return BXF_ERROR;

    BxfResult result =
        bxf_write_u32(writer, (uint32_t)length);

    if (result != BXF_OK)
        return result;

    if (length == 0)
        return BXF_OK;

    if (fwrite(value, 1, length, writer->file) != length)
        return BXF_IO_ERROR;

    return BXF_OK;
}


/* ============================================================
 * BxfValue constructors
 * ============================================================ */

BxfValue bxf_u8(uint8_t value)
{
    return (BxfValue){
        .type = BXF_TYPE_U8,
        .u8 = value
    };
}

BxfValue bxf_u16(uint16_t value)
{
    return (BxfValue){
        .type = BXF_TYPE_U16,
        .u16 = value
    };
}

BxfValue bxf_u32(uint32_t value)
{
    return (BxfValue){
        .type = BXF_TYPE_U32,
        .u32 = value
    };
}

BxfValue bxf_u64(uint64_t value)
{
    return (BxfValue){
        .type = BXF_TYPE_U64,
        .u64 = value
    };
}

BxfValue bxf_i8(int8_t value)
{
    return (BxfValue){
        .type = BXF_TYPE_I8,
        .i8 = value
    };
}

BxfValue bxf_i16(int16_t value)
{
    return (BxfValue){
        .type = BXF_TYPE_I16,
        .i16 = value
    };
}

BxfValue bxf_i32(int32_t value)
{
    return (BxfValue){
        .type = BXF_TYPE_I32,
        .i32 = value
    };
}

BxfValue bxf_i64(int64_t value)
{
    return (BxfValue){
        .type = BXF_TYPE_I64,
        .i64 = value
    };
}

BxfValue bxf_float(float value)
{
    return (BxfValue){
        .type = BXF_TYPE_FLOAT,
        .float32 = value
    };
}

BxfValue bxf_double(double value)
{
    return (BxfValue){
        .type = BXF_TYPE_DOUBLE,
        .float64 = value
    };
}

BxfValue bxf_string(const char *value)
{
    return (BxfValue){
        .type = BXF_TYPE_STRING,
        .string = (char *)value
    };
}


/* ============================================================
 * High-level API
 * ============================================================ */

BxfResult bxf_write(BxfWriter *writer, BxfValue value)
{
    if (writer == NULL || writer->file == NULL)
        return BXF_INVALID_ARGUMENT;

    BxfResult result =
        bxf_write_u8(writer, (uint8_t)value.type);

    if (result != BXF_OK)
        return result;

    switch (value.type)
    {
        case BXF_TYPE_U8:
            return bxf_write_u8(writer, value.u8);

        case BXF_TYPE_U16:
            return bxf_write_u16(writer, value.u16);

        case BXF_TYPE_U32:
            return bxf_write_u32(writer, value.u32);

        case BXF_TYPE_U64:
            return bxf_write_u64(writer, value.u64);

        case BXF_TYPE_I8:
            return bxf_write_i8(writer, value.i8);

        case BXF_TYPE_I16:
            return bxf_write_i16(writer, value.i16);

        case BXF_TYPE_I32:
            return bxf_write_i32(writer, value.i32);

        case BXF_TYPE_I64:
            return bxf_write_i64(writer, value.i64);

        case BXF_TYPE_FLOAT:
            return bxf_write_float(writer, value.float32);

        case BXF_TYPE_DOUBLE:
            return bxf_write_double(writer, value.float64);

        case BXF_TYPE_STRING:
            return bxf_write_string(writer, value.string);

        default:
            return BXF_INVALID_TYPE;
    }
}