# BXF Binary Format

This document describes the BXF binary file format.

The specification is independent of the reference C implementation. Any programming language can implement a BXF reader or writer by following this document.

---

# 1. Overview

BXF is a sequential binary serialization format.

A BXF file consists of:

1. a fixed-size file header;
2. zero or more serialized values.

General structure:

```text
┌──────────────────────┐
│       Header         │
├──────────────────────┤
│       Value          │
├──────────────────────┤
│       Value          │
├──────────────────────┤
│        ...           │
└──────────────────────┘
```

Values are stored sequentially.

---

# 2. Byte Order

All integer values in BXF use **little-endian** byte order.

For example, the `uint32` value:

```text
0x12345678
```

is encoded as:

```text
78 56 34 12
```

This rule applies to:

* unsigned integers;
* signed integers;
* string lengths;
* version numbers;
* type identifiers where applicable.

---

# 3. File Header

Every BXF file begins with an 8-byte header.

```text
Offset   Size   Field
------   ----   ----------------
0x00     4      Magic
0x04     2      Major version
0x06     2      Minor version
```

Total size:

```text
8 bytes
```

---

## 3.1 Magic

The first four bytes are:

```text
42 58 46 31
```

ASCII:

```text
B X F 1
```

The magic value is:

```text
BXF1
```

A reader must reject a file if the magic does not match.

---

## 3.2 Major Version

The next two bytes contain the major format version.

The integer is encoded as little-endian.

For version `1`:

```text
01 00
```

The current major version is:

```text
1
```

A different major version is considered incompatible.

---

## 3.3 Minor Version

The next two bytes contain the minor format version.

For version `1.1`:

```text
01 00
```

The current minor version is:

```text
1
```

The reference implementation currently accepts a file when:

```text
major == supported_major
minor <= supported_minor
```

A future implementation may define more detailed compatibility rules as the format evolves.

---

# 4. Values

After the header, the file contains zero or more values.

Each value begins with a one-byte type identifier:

```text
[type][payload]
```

For example:

```text
03 40 E2 01 00
```

means:

```text
type    = 3
value   = 123456
```

because type `3` represents `U32`.

---

# 5. Type Identifiers

The following type identifiers are currently defined:

|   ID | Type     | Payload size |
| ---: | -------- | -----------: |
|  `1` | `U8`     |       1 byte |
|  `2` | `U16`    |      2 bytes |
|  `3` | `U32`    |      4 bytes |
|  `4` | `U64`    |      8 bytes |
|  `5` | `I8`     |       1 byte |
|  `6` | `I16`    |      2 bytes |
|  `7` | `I32`    |      4 bytes |
|  `8` | `I64`    |      8 bytes |
|  `9` | `FLOAT`  |      4 bytes |
| `10` | `DOUBLE` |      8 bytes |
| `11` | `STRING` |     variable |

Type identifiers are part of the binary format.

Once a type identifier is published, it must not be reassigned to another type.

---

# 6. Unsigned Integers

## 6.1 U8

Type ID:

```text
01
```

Payload:

```text
1 byte
```

Example:

```text
42
```

is encoded as:

```text
01 2A
```

---

## 6.2 U16

Type ID:

```text
02
```

Payload:

```text
2 bytes
```

Example:

```text
1337
```

is:

```text
02 39 05
```

because:

```text
1337 = 0x0539
```

---

## 6.3 U32

Type ID:

```text
03
```

Payload:

```text
4 bytes
```

Example:

```text
123456
```

is:

```text
03 40 E2 01 00
```

---

## 6.4 U64

Type ID:

```text
04
```

Payload:

```text
8 bytes
```

The payload is encoded as a little-endian 64-bit unsigned integer.

---

# 7. Signed Integers

Signed integer values use the same byte widths as their unsigned counterparts.

The binary representation uses the standard two's-complement representation.

## 7.1 I8

Type ID:

```text
05
```

Payload:

```text
1 byte
```

---

## 7.2 I16

Type ID:

```text
06
```

Payload:

```text
2 bytes
```

---

## 7.3 I32

Type ID:

```text
07
```

Payload:

```text
4 bytes
```

---

## 7.4 I64

Type ID:

```text
08
```

Payload:

```text
8 bytes
```

---

# 8. Floating-Point Values

BXF stores floating-point values using their binary representation.

No textual conversion is performed.

---

## 8.1 FLOAT

Type ID:

```text
09
```

Payload:

```text
4 bytes
```

The value is represented by the binary representation of a C `float` with a size of 32 bits.

The reference implementation serializes the value by copying its object representation into a `uint32_t`.

---

## 8.2 DOUBLE

Type ID:

```text
0A
```

Payload:

```text
8 bytes
```

The value is represented by the binary representation of a C `double` with a size of 64 bits.

The reference implementation serializes the value by copying its object representation into a `uint64_t`.

---

# 9. Strings

Strings have a variable size.

The representation is:

```text
[type]
[length]
[data]
```

where:

```text
type   = 1 byte
length = uint32
data   = length bytes
```

The length is stored as a little-endian `uint32`.

The string data does **not** include a terminating NUL byte.

---

## Example

The string:

```text
Hello BXF!
```

contains 10 bytes.

Its representation is:

```text
0B
0A 00 00 00
48 65 6C 6C 6F 20 42 58 46 21
```

Breaking it down:

```text
0B
│
└── STRING type

0A 00 00 00
│
└── length = 10

48 65 6C 6C 6F 20 42 58 46 21
└──────────────────────────────┘
             data
```

The reference C implementation adds a NUL terminator when allocating a string after reading it, but that terminator is not stored in the BXF file.

---

# 10. Complete Example

Consider the following values:

```text
U32     = 123456
I64     = -9876543210
DOUBLE  = 3.141592653589793
STRING  = "Hello BXF!"
```

The file begins with:

```text
42 58 46 31
01 00
01 00
```

which represents:

```text
BXF1
major = 1
minor = 1
```

The values then follow sequentially.

Conceptually:

```text
┌──────────┬───────────────┐
│  Header  │ 8 bytes       │
├──────────┼───────────────┤
│ U32      │ type + 4 bytes│
├──────────┼───────────────┤
│ I64      │ type + 8 bytes│
├──────────┼───────────────┤
│ DOUBLE   │ type + 8 bytes│
├──────────┼───────────────┤
│ STRING   │ type + length │
│          │ + data        │
└──────────┴───────────────┘
```

A reader can process the file from beginning to end without needing to know how many values are stored.

---

# 11. End of File

There is no special end-of-file marker.

The end of the file itself indicates that there are no more values.

A reader should continue reading values until it reaches the end of the file.

In the reference C implementation:

```c
bxf_read(...)
```

returns:

```text
BXF_END_OF_FILE
```

when there are no more values.

---

# 12. Invalid Data

A BXF reader should reject malformed data.

Examples include:

* invalid magic;
* incomplete header;
* unsupported version;
* unknown type identifier;
* incomplete integer payload;
* incomplete floating-point payload;
* incomplete string length;
* incomplete string data.

A reader must not interpret an unknown type identifier as another known type.

---

# 13. Versioning

BXF has two version components:

```text
major.minor
```

For example:

```text
1.1
```

### Major version

A major version change indicates an incompatible format change.

A reader supporting version `1.x` must not assume that it can read version `2.x`.

### Minor version

A minor version may introduce compatible additions.

The current reference implementation accepts older minor versions within the same major version.

Example:

```text
Reader supports: 1.1
File version:     1.0
Result:           supported
```

Whereas:

```text
Reader supports: 1.1
File version:     2.0
Result:           unsupported
```

---

# 14. Forward Compatibility

New type identifiers may be introduced in future versions.

Older readers cannot necessarily interpret values using types they do not know.

Therefore, applications should treat unknown type identifiers as an error unless a future extension mechanism explicitly defines how unknown values can be skipped.

---

# 15. Reserved Values

Type identifiers that are not currently assigned are reserved for future versions.

Currently assigned:

```text
1  U8
2  U16
3  U32
4  U64
5  I8
6  I16
7  I32
8  I64
9  FLOAT
10 DOUBLE
11 STRING
```

Identifiers greater than `11` are currently unassigned.

They must not be used by applications as private extensions without an explicit extension mechanism.

---

# 16. Design Principles

The BXF format follows several principles.

### Simple

A basic reader should be possible to implement with only standard file and byte operations.

### Explicit

Every value contains an explicit type identifier.

### Portable

The format does not depend on:

* C structure padding;
* compiler ABI;
* pointer size;
* native endianness;
* memory addresses.

### Extensible

Additional types can be introduced through future format versions.

### Language-independent

The binary format is not tied to the C API.

A BXF implementation in another language should produce files compatible with the C implementation.

---

# 17. Current Format Version

Current BXF format:

```text
BXF 1.1
```

Magic:

```text
BXF1
```

Header size:

```text
8 bytes
```

Byte order:

```text
Little-endian
```

Defined types:

```text
11
```

---

# 18. Future Extensions

The following features are being considered for future versions:

* arrays;
* binary blobs;
* maps/objects;
* nested values;
* optional values;
* field identifiers;
* metadata;
* compression;
* checksums.

These features are not part of the current BXF 1.1 specification.
