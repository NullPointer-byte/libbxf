#include "bxf.h"

#include <stdio.h>

static void print_value(const BxfValue *value)
{
    switch (value->type)
    {
        case BXF_TYPE_U8:
            printf("u8 = %u\n", value->u8);
            break;

        case BXF_TYPE_U16:
            printf("u16 = %u\n", value->u16);
            break;

        case BXF_TYPE_U32:
            printf("u32 = %u\n", value->u32);
            break;

        case BXF_TYPE_U64:
            printf("u64 = %llu\n",
                   (unsigned long long)value->u64);
            break;

        case BXF_TYPE_I8:
            printf("i8 = %d\n", value->i8);
            break;

        case BXF_TYPE_I16:
            printf("i16 = %d\n", value->i16);
            break;

        case BXF_TYPE_I32:
            printf("i32 = %d\n", value->i32);
            break;

        case BXF_TYPE_I64:
            printf("i64 = %lld\n",
                   (long long)value->i64);
            break;

        case BXF_TYPE_FLOAT:
            printf("float = %f\n", value->float32);
            break;

        case BXF_TYPE_DOUBLE:
            printf("double = %.15f\n", value->float64);
            break;

        case BXF_TYPE_STRING:
            printf("string = %s\n", value->string);
            break;

        default:
            printf("unknown\n");
            break;
    }
}

int main(void)
{
    BxfValue values[] =
    {
        bxf_u8(42),
        bxf_u32(123456),
        bxf_i64(-9876543210LL),
        bxf_double(3.141592653589793),
        bxf_string("Hello BXF!")
    };

    size_t count = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < count; ++i)
        print_value(&values[i]);

    return 0;
}