#include "BitReader.h"

#define TRY(canFail) \
do { \
    LibStream_ReadStatus err = canFail; \
    if (err != LibStream_ReadStatus_Ok) { \
        return err; \
    } \
} while (0);

BitReader bitreader_init(Reader* reader)
{
    return (BitReader){.byteReader = reader, .offset = 0, .subOffset = 0};
}

LibStream_ReadStatus bitreader_readBit(BitReader* br, uint32_t* out)
{
    if (br->subOffset == 0) {
        TRY(reader_skip(br->byteReader, 1));
    }

    uint8_t byte;
    TRY(reader_peekInto(br->byteReader, 1, &byte));

    uint8_t bit = byte & (1 << (7 - br->subOffset));
    *out = (bit != 0);
    if (br->subOffset == 7) {
        br->subOffset = 0;
        br->offset += 1;
    } else {
        br->subOffset += 1;
    }
    return LibStream_ReadStatus_Ok;
}
