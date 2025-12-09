#include "libstream/Reader.h"

#include <string.h>

#include "bitcast.h"

#define TRY(canFail)                             \
    do {                                         \
        LibStream_ReadStatus status = canFail;   \
        if (status != LibStream_ReadStatus_Ok) { \
            return status;                       \
        }                                        \
    } while (0)

LibStream_ReadStatus reader_peekSlice(Reader* reader, size_t n, Slice* out)
{
    return reader->peekSlice(reader, n, out);
}

LibStream_ReadStatus reader_skip(Reader* reader, size_t n)
{
    return reader->skip(reader, n);
}

LibStream_ReadStatus reader_peekInto(Reader* reader, size_t n, uint8_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, n, &slice));
    memcpy(out, slice.slice, n);
    return LibStream_ReadStatus_Ok;
}

LibStream_ReadStatus reader_takeSlice(Reader* reader, size_t n, Slice* out)
{
    TRY(reader_peekSlice(reader, n, out));
    TRY(reader_skip(reader, n));
    return LibStream_ReadStatus_Ok;
}

LibStream_ReadStatus reader_peekFourCC(Reader* reader, uint8_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    memcpy(out, slice.slice, 4);
    return LibStream_ReadStatus_Ok;
}

LibStream_ReadStatus reader_takeFourCC(Reader* reader, uint8_t* out)
{
    TRY(reader_peekFourCC(reader, out));

    return reader_skip(reader, 4);
}

#include "readNumber.inc"
