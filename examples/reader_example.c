#include "bxf.h"

#include <stdio.h>

int main(void)
{
    BxfReader reader;

    BxfResult result = bxf_reader_open(&reader, "example.bxf");

    if (result != BXF_OK)
    {
        printf("Failed to open reader: %d\n", result);
        return 1;
    }

    BxfValue value;

    while ((result = bxf_read(&reader, &value)) == BXF_OK)
    {
        switch (value.type)
        {
            case BXF_TYPE_U32:
                printf("u32: %u\n", value.u32);
                break;

            case BXF_TYPE_I64:
                printf("i64: %lld\n",
                       (long long)value.i64);
                break;

            case BXF_TYPE_FLOAT:
                printf("float: %f\n", value.float32);
                break;

            case BXF_TYPE_DOUBLE:
                printf("double: %.15f\n",
                       value.float64);
                break;

            case BXF_TYPE_STRING:
                printf("string: %s\n", value.string);
                break;

            default:
                printf("Unknown value type\n");
                break;
        }

        bxf_value_free(&value);
    }

    if (result != BXF_END_OF_FILE)
    {
        printf("Failed to read value: %d\n", result);
        bxf_reader_close(&reader);
        return 1;
    }

    result = bxf_reader_close(&reader);

    if (result != BXF_OK)
        return 1;

    return 0;
}