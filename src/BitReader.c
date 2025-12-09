#include "libstream/BitReader.h"
#include "libstream/Reader.h"

#define TRY(canFail)                          \
    do {                                      \
        LibStream_ReadStatus err = canFail;   \
        if (err != LibStream_ReadStatus_Ok) { \
            return err;                       \
        }                                     \
    } while (0);

// LE is not needed for MPEG-1
static uint8_t getNthBit_BE(uint8_t byte, uint8_t offset)
{
    const uint8_t masked = byte & (1 << (7 - offset));
    return masked != 0;
}

BitReader bitreader_init(Reader* reader)
{
    return (BitReader){.byteReader = reader, .subOffset = 0};
}

LibStream_ReadStatus bitreader_takeSingleBit(BitReader* br, uint32_t* out)
{
    if (br->subOffset == 0) {
        TRY(reader_peekInto(br->byteReader, 1, &br->currentByte));
        TRY(reader_skip(br->byteReader, 1));
    }

    uint8_t bit = getNthBit_BE(br->currentByte, br->subOffset);

    br->subOffset++;
    if (br->subOffset == 8) {
        br->subOffset = 0;
    }

    *out = (bit != 0);
    return LibStream_ReadStatus_Ok;
}

bool bitreader_isByteAligned(const BitReader* br)
{
    return br->subOffset == 0;
}

// returns n. of bits skipped
size_t bitreader_byteAlign(BitReader* br)
{
    if (bitreader_isByteAligned(br)) {
        return 0;
    }

    const size_t skipped = 8 - br->subOffset;
    br->subOffset = 0;
    return skipped;
}

// skipBytes always starts by aligning
LibStream_ReadStatus bitreader_skipBytes(BitReader* br, size_t nBytes)
{
    bitreader_byteAlign(br);
    return reader_skip(br->byteReader, nBytes);
}

uint64_t bitreader_getByteOffset(const BitReader* br)
{
    return reader_offset(br->byteReader);
}

LibStream_ReadStatus bitreader_takeBits(BitReader* br,
                                        size_t nBits,
                                        uint32_t* out)
{
    uint32_t bit;
    *out = 0;

    // empty out the buffer
    while (nBits && br->subOffset) {
        TRY(bitreader_takeSingleBit(br, &bit));
        *out = (*out << 1) | bit;
        nBits--;
    }
    if (nBits == 0) {
        return LibStream_ReadStatus_Ok;
    }

    // read bytes at a time while possible
    while (nBits > 8) {
        Slice byte;
        TRY(reader_takeSlice(br->byteReader, 1, &byte));
        *out = (*out << 8) | *byte.slice;
        nBits -= 8;
    }
    if (nBits == 0) {
        return LibStream_ReadStatus_Ok;
    }

    // remainder
    while (nBits) {
        TRY(bitreader_takeSingleBit(br, &bit));
        *out = (*out << 1) | bit;
        nBits--;
    }

    return LibStream_ReadStatus_Ok;
}
