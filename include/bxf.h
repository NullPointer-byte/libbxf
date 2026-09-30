#ifndef BXF_H
#define BXF_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define BXF_MAGIC "BXF1"
#define BXF_MAGIC_SIZE 4

#define BXF_VERSION_MAJOR 1
#define BXF_VERSION_MINOR 1

typedef enum
{
    BXF_OK = 0,
    BXF_ERROR,
    BXF_INVALID_ARGUMENT,
    BXF_IO_ERROR,
    BXF_INVALID_FILE,
    BXF_UNSUPPORTED_VERSION,
    BXF_OUT_OF_MEMORY,
    BXF_INVALID_TYPE,
    BXF_END_OF_FILE
} BxfResult;

typedef enum
{
    BXF_TYPE_U8 = 1,
    BXF_TYPE_U16,
    BXF_TYPE_U32,
    BXF_TYPE_U64,

    BXF_TYPE_I8,
    BXF_TYPE_I16,
    BXF_TYPE_I32,
    BXF_TYPE_I64,

    BXF_TYPE_FLOAT,
    BXF_TYPE_DOUBLE,
    BXF_TYPE_STRING
} BxfType;

typedef struct
{
    BxfType type;

    union
    {
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;

        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;

        float float32;
        double float64;

        char *string;
    };
} BxfValue;

typedef struct
{
    FILE *file;
} BxfWriter;

typedef struct
{
    FILE *file;
} BxfReader;

/* Lifecycle */

BxfResult bxf_writer_open(BxfWriter *writer, const char *path);
BxfResult bxf_writer_close(BxfWriter *writer);

BxfResult bxf_reader_open(BxfReader *reader, const char *path);
BxfResult bxf_reader_close(BxfReader *reader);

/* Value constructors */

BxfValue bxf_u8(uint8_t value);
BxfValue bxf_u16(uint16_t value);
BxfValue bxf_u32(uint32_t value);
BxfValue bxf_u64(uint64_t value);

BxfValue bxf_i8(int8_t value);
BxfValue bxf_i16(int16_t value);
BxfValue bxf_i32(int32_t value);
BxfValue bxf_i64(int64_t value);

BxfValue bxf_float(float value);
BxfValue bxf_double(double value);
BxfValue bxf_string(const char *value);

/* High-level serialization */

BxfResult bxf_write(BxfWriter *writer, BxfValue value);
BxfResult bxf_read(BxfReader *reader, BxfValue *value);

void bxf_value_free(BxfValue *value);

/* Low-level primitives */

BxfResult bxf_write_u8(BxfWriter *writer, uint8_t value);
BxfResult bxf_write_u16(BxfWriter *writer, uint16_t value);
BxfResult bxf_write_u32(BxfWriter *writer, uint32_t value);
BxfResult bxf_write_u64(BxfWriter *writer, uint64_t value);

BxfResult bxf_write_i8(BxfWriter *writer, int8_t value);
BxfResult bxf_write_i16(BxfWriter *writer, int16_t value);
BxfResult bxf_write_i32(BxfWriter *writer, int32_t value);
BxfResult bxf_write_i64(BxfWriter *writer, int64_t value);

BxfResult bxf_write_float(BxfWriter *writer, float value);
BxfResult bxf_write_double(BxfWriter *writer, double value);
BxfResult bxf_write_string(BxfWriter *writer, const char *value);

BxfResult bxf_read_u8(BxfReader *reader, uint8_t *value);
BxfResult bxf_read_u16(BxfReader *reader, uint16_t *value);
BxfResult bxf_read_u32(BxfReader *reader, uint32_t *value);
BxfResult bxf_read_u64(BxfReader *reader, uint64_t *value);

BxfResult bxf_read_i8(BxfReader *reader, int8_t *value);
BxfResult bxf_read_i16(BxfReader *reader, int16_t *value);
BxfResult bxf_read_i32(BxfReader *reader, int32_t *value);
BxfResult bxf_read_i64(BxfReader *reader, int64_t *value);

BxfResult bxf_read_float(BxfReader *reader, float *value);
BxfResult bxf_read_double(BxfReader *reader, double *value);
BxfResult bxf_read_string(BxfReader *reader, char **value);

#endif