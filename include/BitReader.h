#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "Reader.h"

// A stream wrapper that handles reading non-byte-aligned data
// The bit reading order is MSB-first (Big-Endian bit order) per the MPEG-1
// standard (ISO/IEC 11172-3).
//
// a sub-offset of 0 means the next byte is across the byte boundary and will trigger a read
typedef struct BitReader {
    Reader* byteReader;
    size_t subOffset;  // N. of bits already consumed from current_byte (0-7).
    uint8_t currentByte;
} BitReader;

BitReader bitreader_init(Reader* reader);

// --- core read functions
// yes the u32 is wasteful but i'm trying uniformity
LibStream_ReadStatus bitreader_readBit(BitReader* br, uint32_t* out);
LibStream_ReadStatus bitreader_read_bits(BitReader* br,
                                         size_t nBits,
                                         uint32_t* out);

// --- navigation/alignment
bool bitreader_isByteAligned(const BitReader* br);
size_t bitreader_byteAlign(BitReader* br);  // returns n. of bits skipped
// skipBytes always starts by aligning
LibStream_ReadStatus bitreader_skipBytes(BitReader* br, size_t nBytes);
uint64_t bitreader_getBitOffset(const BitReader* br);

#ifdef __cplusplus
}
#endif
