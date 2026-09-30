#include "bxf.h"

#include <stdio.h>

int main(void)
{
    BxfWriter writer;

    BxfResult result = bxf_writer_open(&writer, "example.bxf");

    if (result != BXF_OK)
    {
        printf("Failed to open writer: %d\n", result);
        return 1;
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
        return 1;
    }

    printf("Created example.bxf\n");

    return 0;

fail:
    printf("Failed to write value: %d\n", result);
    bxf_writer_close(&writer);

    return 1;
}