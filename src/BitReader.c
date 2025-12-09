#include "BitReader.h"

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

LibStream_ReadStatus bitreader_readBit(BitReader* br, uint32_t* out)
{
    if (br->subOffset == 0) {
        TRY(reader_peekInto(br->byteReader, 1, &br->currentByte));
        TRY(reader_skip(br->byteReader, 1));
    }

    uint8_t bit = getNthBit_BE(br->currentByte, br->subOffset);
    *out = (bit != 0);
    if (br->subOffset == 7) {
        br->subOffset = 0;
    } else {
        br->subOffset += 1;
    }
    return LibStream_ReadStatus_Ok;
}
