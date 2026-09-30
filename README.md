# BXF

**BXF (Binary eXchange Format)** is a small, simple binary serialization format and C library.

BXF is designed to be:

* simple to implement;
* easy to understand;
* cross-platform;
* language-independent at the format level;
* suitable for storing structured binary data.

The reference implementation is written in **C17**.

> BXF is currently under development. The binary format and API may change before the first stable release.

---

## Features

Current version supports:

* unsigned integers:

  * `u8`
  * `u16`
  * `u32`
  * `u64`
* signed integers:

  * `i8`
  * `i16`
  * `i32`
  * `i64`
* floating-point values:

  * `float`
  * `double`
* strings
* automatic file header
* format versioning
* little-endian integer encoding
* sequential value reading and writing
* simple C API

---

## Example

### Writing

```c
#include "bxf.h"

int main(void)
{
    BxfWriter writer;

    if (bxf_writer_open(&writer, "example.bxf") != BXF_OK)
        return 1;

    bxf_write(&writer, bxf_u32(123456));
    bxf_write(&writer, bxf_i64(-9876543210LL));
    bxf_write(&writer, bxf_double(3.141592653589793));
    bxf_write(&writer, bxf_string("Hello BXF!"));

    bxf_writer_close(&writer);

    return 0;
}
```

### Reading

```c
#include "bxf.h"
#include <stdio.h>

int main(void)
{
    BxfReader reader;

    if (bxf_reader_open(&reader, "example.bxf") != BXF_OK)
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

## Building

BXF uses CMake.

### Requirements

* C compiler with C17 support
* CMake 3.20 or newer

### Build

```bash
cmake -S . -B build
cmake --build build
```

The static library and example programs will be placed in the `build` directory.

---

## Examples

The project contains several examples:

| Example                 | Description                               |
| ----------------------- | ----------------------------------------- |
| `bxf_writer_example`    | Writing values to a BXF file              |
| `bxf_reader_example`    | Reading values from a BXF file            |
| `bxf_values_example`    | Working with `BxfValue`                   |
| `bxf_error_example`     | Handling errors                           |
| `bxf_roundtrip_example` | Writing and reading values in one program |

For example:

```bash
./build/bxf_roundtrip_example
```

---

## Format

A BXF file begins with an 8-byte header:

```text
4 bytes   Magic
2 bytes   Major version
2 bytes   Minor version
```

The current magic is:

```text
BXF1
```

Values are then stored sequentially:

```text
[type][value]
[type][value]
[type][value]
...
```

Each value begins with a one-byte type identifier.

For example:

```text
01 2A
```

represents:

```text
type = U8
value = 42
```

More information about the binary format is available in:

`docs/FORMAT.md`

---

## API Documentation

The C API is documented in:

`docs/API.md`

The API is split conceptually into:

* writer/reader lifecycle;
* value constructors;
* high-level serialization;
* low-level serialization;
* error handling;
* memory management.

---

## Project Structure

```text
bxf/
├── include/
│   └── bxf.h
│
├── src/
│   ├── writer.c
│   └── reader.c
│
├── examples/
│   ├── writer_example.c
│   ├── reader_example.c
│   ├── values_example.c
│   ├── error_example.c
│   └── roundtrip_example.c
│
├── docs/
│   ├── API.md
│   └── FORMAT.md
│
├── CMakeLists.txt
├── build.sh
├── .clangd
└── .gitignore
```

---

## Design Goals

BXF is intentionally kept small.

The main goals are:

1. **Simple format**

   The binary layout should be understandable without a complicated specification.

2. **Easy implementation**

   Implementing a BXF reader or writer in another language should be straightforward.

3. **Stable primitives**

   Primitive type identifiers should remain stable once they become part of a released format version.

4. **Cross-language compatibility**

   The format should not depend on C structures, compiler ABI, padding, or platform-specific binary layouts.

5. **Minimal dependencies**

   The reference C implementation uses the standard C library.

---

## Planned Features

Possible future additions include:

* arrays;
* binary blobs;
* objects/maps;
* nested values;
* optional fields;
* metadata;
* streaming support;
* additional language implementations.

Possible future implementations include:

* C
* Lua
* Python
* Java

---

## Versioning

BXF uses semantic project versions such as:

```text
0.2.0
```

The binary format itself has its own version stored in every BXF file.

Current format version:

```text
1.1
```

The format version and library version are separate concepts.

---

## Status

BXF is currently an experimental project.

The API and binary format are not considered stable yet.

Until version `1.0.0`, breaking changes may occur.

---

## License

License information will be added before the first public release.

---

## Documentation

- [API Documentation](docs/API.md)
- [Binary Format Specification](docs/FORMAT.md)