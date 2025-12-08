ReadStatus reader_peekU16_LE(Reader* reader, uint16_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 2, &slice));

    *out = bitcastU16_LE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeU16_LE(Reader* reader, uint16_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 2, &slice));

    *out = bitcastU16_LE(slice.slice);
    return reader_skip(reader, 2);
}

ReadStatus reader_peekU16_BE(Reader* reader, uint16_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 2, &slice));

    *out = bitcastU16_BE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeU16_BE(Reader* reader, uint16_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 2, &slice));

    *out = bitcastU16_BE(slice.slice);
    return reader_skip(reader, 2);
}

ReadStatus reader_peekI16_LE(Reader* reader, int16_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 2, &slice));

    *out = bitcastI16_LE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeI16_LE(Reader* reader, int16_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 2, &slice));

    *out = bitcastI16_LE(slice.slice);
    return reader_skip(reader, 2);
}

ReadStatus reader_peekI16_BE(Reader* reader, int16_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 2, &slice));

    *out = bitcastI16_BE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeI16_BE(Reader* reader, int16_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 2, &slice));

    *out = bitcastI16_BE(slice.slice);
    return reader_skip(reader, 2);
}

ReadStatus reader_peekU32_LE(Reader* reader, uint32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    *out = bitcastU32_LE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeU32_LE(Reader* reader, uint32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    *out = bitcastU32_LE(slice.slice);
    return reader_skip(reader, 4);
}

ReadStatus reader_peekU32_BE(Reader* reader, uint32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    *out = bitcastU32_BE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeU32_BE(Reader* reader, uint32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    *out = bitcastU32_BE(slice.slice);
    return reader_skip(reader, 4);
}

ReadStatus reader_peekI32_LE(Reader* reader, int32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    *out = bitcastI32_LE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeI32_LE(Reader* reader, int32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    *out = bitcastI32_LE(slice.slice);
    return reader_skip(reader, 4);
}

ReadStatus reader_peekI32_BE(Reader* reader, int32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    *out = bitcastI32_BE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeI32_BE(Reader* reader, int32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    *out = bitcastI32_BE(slice.slice);
    return reader_skip(reader, 4);
}

ReadStatus reader_peekU64_LE(Reader* reader, uint64_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 8, &slice));

    *out = bitcastU64_LE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeU64_LE(Reader* reader, uint64_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 8, &slice));

    *out = bitcastU64_LE(slice.slice);
    return reader_skip(reader, 8);
}

ReadStatus reader_peekU64_BE(Reader* reader, uint64_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 8, &slice));

    *out = bitcastU64_BE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeU64_BE(Reader* reader, uint64_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 8, &slice));

    *out = bitcastU64_BE(slice.slice);
    return reader_skip(reader, 8);
}

ReadStatus reader_peekI64_LE(Reader* reader, int64_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 8, &slice));

    *out = bitcastI64_LE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeI64_LE(Reader* reader, int64_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 8, &slice));

    *out = bitcastI64_LE(slice.slice);
    return reader_skip(reader, 8);
}

ReadStatus reader_peekI64_BE(Reader* reader, int64_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 8, &slice));

    *out = bitcastI64_BE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeI64_BE(Reader* reader, int64_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 8, &slice));

    *out = bitcastI64_BE(slice.slice);
    return reader_skip(reader, 8);
}

ReadStatus reader_peekI24_LE(Reader* reader, int32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 3, &slice));

    *out = bitcastI24_LE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeI24_LE(Reader* reader, int32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 3, &slice));

    *out = bitcastI24_LE(slice.slice);
    return reader_skip(reader, 3);
}

ReadStatus reader_peekI24_BE(Reader* reader, int32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 3, &slice));

    *out = bitcastI24_BE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeI24_BE(Reader* reader, int32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 3, &slice));

    *out = bitcastI24_BE(slice.slice);
    return reader_skip(reader, 3);
}

ReadStatus reader_peekU24_LE(Reader* reader, uint32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 3, &slice));

    *out = bitcastU24_LE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeU24_LE(Reader* reader, uint32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 3, &slice));

    *out = bitcastU24_LE(slice.slice);
    return reader_skip(reader, 3);
}

ReadStatus reader_peekU24_BE(Reader* reader, uint32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 3, &slice));

    *out = bitcastU24_BE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeU24_BE(Reader* reader, uint32_t* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 3, &slice));

    *out = bitcastU24_BE(slice.slice);
    return reader_skip(reader, 3);
}

ReadStatus reader_peekF32_LE(Reader* reader, float* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    *out = bitcastF32_LE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeF32_LE(Reader* reader, float* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    *out = bitcastF32_LE(slice.slice);
    return reader_skip(reader, 4);
}

ReadStatus reader_peekF32_BE(Reader* reader, float* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    *out = bitcastF32_BE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeF32_BE(Reader* reader, float* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 4, &slice));

    *out = bitcastF32_BE(slice.slice);
    return reader_skip(reader, 4);
}

ReadStatus reader_peekF64_LE(Reader* reader, double* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 8, &slice));

    *out = bitcastF64_LE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeF64_LE(Reader* reader, double* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 8, &slice));

    *out = bitcastF64_LE(slice.slice);
    return reader_skip(reader, 8);
}

ReadStatus reader_peekF64_BE(Reader* reader, double* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 8, &slice));

    *out = bitcastF64_BE(slice.slice);
    return ReadStatus_Ok;
}

ReadStatus reader_takeF64_BE(Reader* reader, double* out)
{
    Slice slice;
    TRY(reader_peekSlice(reader, 8, &slice));

    *out = bitcastF64_BE(slice.slice);
    return reader_skip(reader, 8);
}
