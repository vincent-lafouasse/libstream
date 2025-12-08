#include "Reader.h"

#include <string.h>

#include "bitcast.h"

#define TRY(canFail)                   \
    do {                               \
        ReadStatus status = canFail;   \
        if (status != ReadStatus_Ok) { \
            return status;             \
        }                              \
    } while (0)

ReadStatus reader_peekSlice(Reader* reader, size_t n, Slice* out)
{
    return reader->peekSlice(reader, n, out);
}

ReadStatus reader_skip(Reader* reader, size_t n)
{
    return reader->skip(reader, n);
}

ReadStatus reader_peekInto(Reader* reader, size_t n, uint8_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, n, &slice));
    memcpy(out, slice.slice, n);
    return ReadStatus_Ok;
}

ReadStatus reader_takeSlice(Reader* reader, size_t n, Slice* out)
{
    TRY(reader_peekSlice(reader, n, out));
    TRY(reader_skip(reader, n));
    return ReadStatus_Ok;
}

ReadStatus reader_peekFourCC(Reader* reader, uint8_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    memcpy(out, slice.slice, 4);
    return ReadStatus_Ok;
}

ReadStatus reader_takeFourCC(Reader* reader, uint8_t* out)
{
    TRY(reader_peekFourCC(reader, out));

    return reader_skip(reader, 4);
}

#include "readNumber.c"
