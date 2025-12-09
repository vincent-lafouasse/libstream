#include "MockReader.hpp"

#include <cstring>

MockReader::MockReader(const std::vector<Byte>& v) : data(v), pos(0) {}

MockReader::MockReader(const std::string& s) : data(), pos(0)
{
    for (Byte b : s) {
        this->data.push_back(b);
    }
}

LibStream_ReadStatus MockReader::peekSlice(size_t size, Slice* out) const
{
    if (this->pos + size > this->data.size()) {
        return LibStream_ReadStatus_UnexpectedEOF;
    }

    *out = {.slice = this->data.data() + this->pos, .len = size};
    return LibStream_ReadStatus_Ok;
}

LibStream_ReadStatus MockReader::skip(size_t size)
{
    if (this->pos + size > this->data.size()) {
        return LibStream_ReadStatus_UnexpectedEOF;
    } else {
        this->pos += size;
        return LibStream_ReadStatus_Ok;
    }
}

LibStream_ReadStatus mockReaderPeekSlice(Reader* reader, size_t n, Slice* out)
{
    MockReader* mockReader = static_cast<MockReader*>(reader->ctx);

    return mockReader->peekSlice(n, out);
}

LibStream_ReadStatus mockReaderSkip(Reader* reader, size_t n)
{
    MockReader* mockReader = static_cast<MockReader*>(reader->ctx);

    LibStream_ReadStatus err = mockReader->skip(n);
    if (err != LibStream_ReadStatus_Ok) {
        return err;
    } else {
        reader->offset += n;
        return LibStream_ReadStatus_Ok;
    }
}

Reader mockReaderInterface(MockReader* mockReader)
{
    return {
        .ctx = static_cast<void*>(mockReader),
        .peekSlice = mockReaderPeekSlice,
        .skip = mockReaderSkip,
        .offset = 0,
    };
}
