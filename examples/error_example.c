#include "bxf.h"

#include <stdio.h>

static const char *result_name(BxfResult result)
{
    switch (result)
    {
        case BXF_OK:
            return "OK";

        case BXF_ERROR:
            return "ERROR";

        case BXF_INVALID_ARGUMENT:
            return "INVALID_ARGUMENT";

        case BXF_IO_ERROR:
            return "IO_ERROR";

        case BXF_INVALID_FILE:
            return "INVALID_FILE";

        case BXF_UNSUPPORTED_VERSION:
            return "UNSUPPORTED_VERSION";

        case BXF_OUT_OF_MEMORY:
            return "OUT_OF_MEMORY";

        case BXF_INVALID_TYPE:
            return "INVALID_TYPE";

        case BXF_END_OF_FILE:
            return "END_OF_FILE";

        default:
            return "UNKNOWN";
    }
}

int main(void)
{
    BxfReader reader;

    BxfResult result =
        bxf_reader_open(&reader, "does_not_exist.bxf");

    if (result != BXF_OK)
    {
        printf(
            "Failed to open file: %s\n",
            result_name(result)
        );

        return 1;
    }

    bxf_reader_close(&reader);

    return 0;
}