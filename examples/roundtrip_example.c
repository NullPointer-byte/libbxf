#include "bxf.h"

#include <stdio.h>

static int write_values(const char *path)
{
    BxfWriter writer;

    BxfResult result = bxf_writer_open(&writer, path);

    if (result != BXF_OK)
    {
        printf("Failed to open writer: %d\n", result);
        return 0;
    }

    result = bxf_write(&writer, bxf_u32(123456));
    if (result != BXF_OK)
        goto fail;

    result = bxf_write(&writer, bxf_i64(-9876543210LL));
    if (result != BXF_OK)
        goto fail;

    result = bxf_write(&writer, bxf_float(3.14f));
    if (result != BXF_OK)
        goto fail;

    result = bxf_write(&writer, bxf_double(3.141592653589793));
    if (result != BXF_OK)
        goto fail;

    result = bxf_write(&writer, bxf_string("Hello BXF!"));
    if (result != BXF_OK)
        goto fail;

    result = bxf_writer_close(&writer);

    if (result != BXF_OK)
    {
        printf("Failed to close writer: %d\n", result);
        return 0;
    }

    return 1;

fail:
    printf("Failed to write value: %d\n", result);
    bxf_writer_close(&writer);

    return 0;
}

static int read_values(const char *path)
{
    BxfReader reader;

    BxfResult result = bxf_reader_open(&reader, path);

    if (result != BXF_OK)
    {
        printf("Failed to open reader: %d\n", result);
        return 0;
    }

    BxfValue value;

    while ((result = bxf_read(&reader, &value)) == BXF_OK)
    {
        switch (value.type)
        {
            case BXF_TYPE_U32:
                printf("u32    = %u\n", value.u32);
                break;

            case BXF_TYPE_I64:
                printf("i64    = %lld\n",
                       (long long)value.i64);
                break;

            case BXF_TYPE_FLOAT:
                printf("float  = %f\n", value.float32);
                break;

            case BXF_TYPE_DOUBLE:
                printf("double = %.15f\n", value.float64);
                break;

            case BXF_TYPE_STRING:
                printf("string = %s\n", value.string);
                break;

            default:
                printf("Unknown value type: %d\n",
                       value.type);
                break;
        }

        bxf_value_free(&value);
    }

    if (result != BXF_END_OF_FILE)
    {
        printf("Failed to read value: %d\n", result);
        bxf_reader_close(&reader);
        return 0;
    }

    result = bxf_reader_close(&reader);

    if (result != BXF_OK)
        return 0;

    return 1;
}

int main(void)
{
    const char *path = "roundtrip.bxf";

    printf("Writing values...\n");

    if (!write_values(path))
        return 1;

    printf("\nReading values...\n");

    if (!read_values(path))
        return 1;

    printf("\nRoundtrip successful!\n");

    return 0;
}
