# BXF C API

This document describes the public C API provided by BXF.

BXF uses a small high-level API based around two main objects:

* `BxfWriter` — writes values to a BXF file.
* `BxfReader` — reads values from a BXF file.

Values are represented by `BxfValue`.

---

## Requirements

BXF is written in C17.

Include the public header:

```c
#include "bxf.h"
```

---

# Result Codes

Most BXF functions return `BxfResult`.

```c
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
```

### `BXF_OK`

The operation completed successfully.

### `BXF_ERROR`

A general error occurred.

### `BXF_INVALID_ARGUMENT`

One or more arguments are invalid.

For example:

```c
bxf_writer_open(NULL, "file.bxf");
```

### `BXF_IO_ERROR`

A file I/O operation failed.

### `BXF_INVALID_FILE`

The file is not a valid BXF file.

This can happen when:

* the magic value is invalid;
* the header is incomplete;
* the file is otherwise malformed.

### `BXF_UNSUPPORTED_VERSION`

The BXF file uses a format version that the current library does not support.

### `BXF_OUT_OF_MEMORY`

Memory allocation failed.

### `BXF_INVALID_TYPE`

An unknown or invalid value type was encountered.

### `BXF_END_OF_FILE`

The reader reached the end of the BXF file.

This is not necessarily an error. It is the normal result after all values have been read.

---

# BxfType

`BxfType` identifies the type of a stored value.

```c
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
```

The numeric identifiers are part of the BXF binary format and should be treated as stable.

| Type              | ID | C type     |
| ----------------- | -: | ---------- |
| `BXF_TYPE_U8`     |  1 | `uint8_t`  |
| `BXF_TYPE_U16`    |  2 | `uint16_t` |
| `BXF_TYPE_U32`    |  3 | `uint32_t` |
| `BXF_TYPE_U64`    |  4 | `uint64_t` |
| `BXF_TYPE_I8`     |  5 | `int8_t`   |
| `BXF_TYPE_I16`    |  6 | `int16_t`  |
| `BXF_TYPE_I32`    |  7 | `int32_t`  |
| `BXF_TYPE_I64`    |  8 | `int64_t`  |
| `BXF_TYPE_FLOAT`  |  9 | `float`    |
| `BXF_TYPE_DOUBLE` | 10 | `double`   |
| `BXF_TYPE_STRING` | 11 | `char *`   |

---

# BxfValue

`BxfValue` stores one BXF value.

```c
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
```

The active member of the union is determined by `type`.

For example:

```c
BxfValue value = bxf_u32(123456);

printf("%u\n", value.u32);
```

Do not access a union member that does not correspond to `value.type`.

---

# Writer

## `bxf_writer_open`

```c
BxfResult bxf_writer_open(
    BxfWriter *writer,
    const char *path
);
```

Creates or overwrites a BXF file and initializes a writer.

The function automatically writes the BXF header.

Example:

```c
BxfWriter writer;

BxfResult result =
    bxf_writer_open(&writer, "example.bxf");

if (result != BXF_OK)
    return 1;
```

### Parameters

* `writer` — pointer to a `BxfWriter`.
* `path` — output file path.

### Returns

* `BXF_OK` on success.
* `BXF_INVALID_ARGUMENT` for invalid arguments.
* `BXF_IO_ERROR` if the file cannot be opened or the header cannot be written.

---

## `bxf_writer_close`

```c
BxfResult bxf_writer_close(
    BxfWriter *writer
);
```

Closes the output file.

Example:

```c
bxf_writer_close(&writer);
```

The writer should be closed after writing is complete.

---

# Reader

## `bxf_reader_open`

```c
BxfResult bxf_reader_open(
    BxfReader *reader,
    const char *path
);
```

Opens a BXF file for reading.

The function automatically:

1. opens the file;
2. checks the magic value;
3. reads the format version;
4. validates the version.

Example:

```c
BxfReader reader;

BxfResult result =
    bxf_reader_open(&reader, "example.bxf");

if (result != BXF_OK)
    return 1;
```

---

## `bxf_reader_close`

```c
BxfResult bxf_reader_close(
    BxfReader *reader
);
```

Closes the input file.

Example:

```c
bxf_reader_close(&reader);
```

---

# Value Constructors

Constructors provide a convenient way to create `BxfValue` objects.

## Unsigned integers

```c
BxfValue bxf_u8(uint8_t value);
BxfValue bxf_u16(uint16_t value);
BxfValue bxf_u32(uint32_t value);
BxfValue bxf_u64(uint64_t value);
```

Example:

```c
BxfValue a = bxf_u8(42);
BxfValue b = bxf_u32(123456);
```

---

## Signed integers

```c
BxfValue bxf_i8(int8_t value);
BxfValue bxf_i16(int16_t value);
BxfValue bxf_i32(int32_t value);
BxfValue bxf_i64(int64_t value);
```

Example:

```c
BxfValue value = bxf_i64(-9876543210LL);
```

---

## Floating-point values

```c
BxfValue bxf_float(float value);
BxfValue bxf_double(double value);
```

Example:

```c
BxfValue a = bxf_float(3.14f);
BxfValue b = bxf_double(3.141592653589793);
```

---

## Strings

```c
BxfValue bxf_string(const char *value);
```

Example:

```c
BxfValue value = bxf_string("Hello BXF!");
```

The constructor stores the supplied string pointer in the `BxfValue`.

The constructor does not currently allocate a copy of the string.

Therefore, the original string must remain valid while the value is being used for writing.

---

# High-Level API

## `bxf_write`

```c
BxfResult bxf_write(
    BxfWriter *writer,
    BxfValue value
);
```

Writes one complete value to a BXF file.

The function automatically writes the value's type identifier followed by its data.

Example:

```c
bxf_write(&writer, bxf_u32(123456));
bxf_write(&writer, bxf_i64(-9876543210LL));
bxf_write(&writer, bxf_double(3.141592653589793));
bxf_write(&writer, bxf_string("Hello BXF!"));
```

This is the recommended API for normal BXF usage.

---

## `bxf_read`

```c
BxfResult bxf_read(
    BxfReader *reader,
    BxfValue *value
);
```

Reads one complete value from a BXF file.

Example:

```c
BxfValue value;

BxfResult result =
    bxf_read(&reader, &value);

if (result == BXF_OK)
{
    printf("Type: %d\n", value.type);

    bxf_value_free(&value);
}
```

When there are no more values, the function returns:

```c
BXF_END_OF_FILE
```

Typical reading loop:

```c
BxfValue value;

while (bxf_read(&reader, &value) == BXF_OK)
{
    /* Use value */

    bxf_value_free(&value);
}
```

---

# Memory Management

## `bxf_value_free`

```c
void bxf_value_free(
    BxfValue *value
);
```

Releases memory owned by a `BxfValue`.

Currently, strings read from a BXF file are dynamically allocated.

Therefore:

```c
BxfValue value;

if (bxf_read(&reader, &value) == BXF_OK)
{
    printf("%s\n", value.string);

    bxf_value_free(&value);
}
```

`bxf_value_free()` must be called after processing a value containing a string returned by `bxf_read()`.

For non-string values, it is safe to call the function as well.

---

# Low-Level API

BXF also exposes primitive read/write functions.

These functions operate directly on the binary stream without writing or reading a type identifier.

For example:

```c
bxf_write_u32(&writer, 123456);
```

writes only the four-byte integer representation.

It does **not** write:

```text
BXF_TYPE_U32
```

The low-level functions are useful when implementing custom structures or higher-level serialization.

---

## Write functions

```c
BxfResult bxf_write_u8(BxfWriter *, uint8_t);
BxfResult bxf_write_u16(BxfWriter *, uint16_t);
BxfResult bxf_write_u32(BxfWriter *, uint32_t);
BxfResult bxf_write_u64(BxfWriter *, uint64_t);

BxfResult bxf_write_i8(BxfWriter *, int8_t);
BxfResult bxf_write_i16(BxfWriter *, int16_t);
BxfResult bxf_write_i32(BxfWriter *, int32_t);
BxfResult bxf_write_i64(BxfWriter *, int64_t);

BxfResult bxf_write_float(BxfWriter *, float);
BxfResult bxf_write_double(BxfWriter *, double);

BxfResult bxf_write_string(
    BxfWriter *,
    const char *
);
```

All integer values are encoded in little-endian byte order.

---

## Read functions

```c
BxfResult bxf_read_u8(BxfReader *, uint8_t *);
BxfResult bxf_read_u16(BxfReader *, uint16_t *);
BxfResult bxf_read_u32(BxfReader *, uint32_t *);
BxfResult bxf_read_u64(BxfReader *, uint64_t *);

BxfResult bxf_read_i8(BxfReader *, int8_t *);
BxfResult bxf_read_i16(BxfReader *, int16_t *);
BxfResult bxf_read_i32(BxfReader *, int32_t *);
BxfResult bxf_read_i64(BxfReader *, int64_t *);

BxfResult bxf_read_float(BxfReader *, float *);
BxfResult bxf_read_double(BxfReader *);

BxfResult bxf_read_string(
    BxfReader *,
    char **
);
```

The actual declaration for `bxf_read_double` is:

```c
BxfResult bxf_read_double(
    BxfReader *reader,
    double *value
);
```

All read functions return a `BxfResult`.

---

# Complete Example

The following example writes several values and then reads them back.

```c
#include "bxf.h"

#include <stdio.h>

int main(void)
{
    const char *path = "example.bxf";

    BxfWriter writer;

    if (bxf_writer_open(&writer, path) != BXF_OK)
        return 1;

    bxf_write(&writer, bxf_u32(123456));
    bxf_write(&writer, bxf_i64(-9876543210LL));
    bxf_write(&writer, bxf_double(3.141592653589793));
    bxf_write(&writer, bxf_string("Hello BXF!"));

    if (bxf_writer_close(&writer) != BXF_OK)
        return 1;

    BxfReader reader;

    if (bxf_reader_open(&reader, path) != BXF_OK)
        return 1;

    BxfValue value;

    while (bxf_read(&reader, &value) == BXF_OK)
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

            case BXF_TYPE_DOUBLE:
                printf("double: %.15f\n",
                       value.float64);
                break;

            case BXF_TYPE_STRING:
                printf("string: %s\n",
                       value.string);
                break;

            default:
                break;
        }

        bxf_value_free(&value);
    }

    bxf_reader_close(&reader);

    return 0;
}
```

---

# API Design

The high-level API is intentionally small:

```text
bxf_writer_open()
        ↓
bxf_write()
        ↓
bxf_write()
        ↓
bxf_writer_close()
```

and:

```text
bxf_reader_open()
        ↓
bxf_read()
        ↓
bxf_read()
        ↓
bxf_reader_close()
```

The lower-level functions are available when direct control over the binary representation is required.

---

# API Stability

The BXF C API is currently experimental.

Before version `1.0.0`, functions, structures, error codes, and ownership rules may change.

The binary format is documented separately in:
