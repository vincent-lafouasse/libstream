#include <string>

#include "gtest/gtest.h"

#include "MockReader.hpp"
#include "libstream/BitReader.h"
#include "libstream/Reader.h"

#define ASSERT_IS_OK(status) ASSERT_EQ(status, LibStream_ReadStatus_Ok)
#define ASSERT_IS_EOF(status) \
    ASSERT_EQ(status, LibStream_ReadStatus_UnexpectedEOF)

TEST(BitReader, Init_StateAndOffset)
{
    MockReader mockReader("ABC"); // irrelevant
    Reader reader = mockReaderInterface(&mockReader);

    BitReader br = bitreader_init(&reader);

    ASSERT_EQ(bitreader_getByteOffset(&br), 0u);
    ASSERT_EQ(br.subOffset, 0u);
    ASSERT_TRUE(bitreader_isByteAligned(&br));
}
