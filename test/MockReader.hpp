#pragma once

#include <string>
#include <vector>

#include "Reader.h"

using Byte = uint8_t;

class MockReader {
   private:
    std::vector<Byte> data;
    size_t pos;

   public:
    MockReader(const std::vector<Byte>& v);
    MockReader(const std::string& s);

    LibStream_ReadStatus peekSlice(size_t size, Slice* out) const;
    LibStream_ReadStatus skip(size_t size);
};

LibStream_ReadStatus mockReaderPeekSlice(Reader* reader, size_t n, Slice* out);
LibStream_ReadStatus mockReaderSkip(Reader* reader, size_t n);
Reader mockReaderInterface(MockReader* memoryReader);
