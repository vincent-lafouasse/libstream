#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    ReadStatus_Ok,
    ReadStatus_ReadFailed,
    ReadStatus_UnexpectedEOF,
} ReadStatus;

typedef struct Reader Reader;

typedef struct {
    const uint8_t* slice;
    size_t len;
} Slice;

typedef ReadStatus (*PeekSliceFn)(Reader* reader, size_t n, Slice* out);
typedef ReadStatus (*SkipFn)(Reader* reader, size_t n);

struct Reader {
    void* ctx;
    PeekSliceFn peekSlice;
    SkipFn skip;
    size_t offset;
};

ReadStatus reader_peekSlice(Reader* reader, size_t n, Slice* out);
ReadStatus reader_peekInto(Reader* reader, size_t n, uint8_t* out);
ReadStatus reader_skip(Reader* reader, size_t n);

ReadStatus reader_takeSlice(Reader* reader, size_t n, Slice* out);

ReadStatus reader_peekFourCC(Reader* reader, uint8_t* out);
ReadStatus reader_takeFourCC(Reader* reader, uint8_t* out);

ReadStatus reader_peekU16_LE(Reader* reader, uint16_t* out);
ReadStatus reader_takeU16_LE(Reader* reader, uint16_t* out);
ReadStatus reader_peekU16_BE(Reader* reader, uint16_t* out);
ReadStatus reader_takeU16_BE(Reader* reader, uint16_t* out);
ReadStatus reader_peekI16_LE(Reader* reader, int16_t* out);
ReadStatus reader_takeI16_LE(Reader* reader, int16_t* out);
ReadStatus reader_peekI16_BE(Reader* reader, int16_t* out);
ReadStatus reader_takeI16_BE(Reader* reader, int16_t* out);

ReadStatus reader_peekU32_LE(Reader* reader, uint32_t* out);
ReadStatus reader_takeU32_LE(Reader* reader, uint32_t* out);
ReadStatus reader_peekU32_BE(Reader* reader, uint32_t* out);
ReadStatus reader_takeU32_BE(Reader* reader, uint32_t* out);
ReadStatus reader_peekI32_LE(Reader* reader, int32_t* out);
ReadStatus reader_takeI32_LE(Reader* reader, int32_t* out);
ReadStatus reader_peekI32_BE(Reader* reader, int32_t* out);
ReadStatus reader_takeI32_BE(Reader* reader, int32_t* out);

ReadStatus reader_peekU64_LE(Reader* reader, uint64_t* out);
ReadStatus reader_takeU64_LE(Reader* reader, uint64_t* out);
ReadStatus reader_peekU64_BE(Reader* reader, uint64_t* out);
ReadStatus reader_takeU64_BE(Reader* reader, uint64_t* out);
ReadStatus reader_peekI64_LE(Reader* reader, int64_t* out);
ReadStatus reader_takeI64_LE(Reader* reader, int64_t* out);
ReadStatus reader_peekI64_BE(Reader* reader, int64_t* out);
ReadStatus reader_takeI64_BE(Reader* reader, int64_t* out);

ReadStatus reader_peekI24_LE(Reader* reader, int32_t* out);
ReadStatus reader_takeI24_LE(Reader* reader, int32_t* out);
ReadStatus reader_peekI24_BE(Reader* reader, int32_t* out);
ReadStatus reader_takeI24_BE(Reader* reader, int32_t* out);
ReadStatus reader_peekU24_LE(Reader* reader, uint32_t* out);
ReadStatus reader_takeU24_LE(Reader* reader, uint32_t* out);
ReadStatus reader_peekU24_BE(Reader* reader, uint32_t* out);
ReadStatus reader_takeU24_BE(Reader* reader, uint32_t* out);

ReadStatus reader_peekF32_LE(Reader* reader, float* out);
ReadStatus reader_takeF32_LE(Reader* reader, float* out);
ReadStatus reader_peekF32_BE(Reader* reader, float* out);
ReadStatus reader_takeF32_BE(Reader* reader, float* out);

ReadStatus reader_peekF64_LE(Reader* reader, double* out);
ReadStatus reader_takeF64_LE(Reader* reader, double* out);
ReadStatus reader_peekF64_BE(Reader* reader, double* out);
ReadStatus reader_takeF64_BE(Reader* reader, double* out);

#ifdef __cplusplus
}
#endif
