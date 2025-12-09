#include <cstdint>
#include <string>

#include "gtest/gtest.h"

#include "MockReader.hpp"
#include "libstream/BitReader.h"
#include "libstream/Reader.h"

#define ASSERT_IS_OK(status) ASSERT_EQ(status, LibStream_ReadStatus_Ok)
#define ASSERT_IS_EOF(status) \
    ASSERT_EQ(status, LibStream_ReadStatus_UnexpectedEOF)

TEST(BitReader, Dummy)
{
    MockReader mockReader("\x44");
    Reader byteReader = mockReaderInterface(&mockReader);
    BitReader reader = bitreader_init(&byteReader);
}
