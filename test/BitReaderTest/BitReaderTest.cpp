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
    MockReader mockReader("ABC"); 
    Reader reader = mockReaderInterface(&mockReader);

    BitReader br = bitreader_init(&reader);

    ASSERT_EQ(br.subOffset, 0u);
    ASSERT_TRUE(bitreader_isByteAligned(&br));

    ASSERT_EQ(bitreader_getByteOffset(&br), 0u);
    ASSERT_EQ(bitreader_getBitOffset(&br), 0u);
}

TEST(BitReader, GetByteOffset_MatchesReaderSkip)
{
    MockReader mockReader("ABCDEFGHIJ"); 
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);

    // simulate a read
    reader_skip(&reader, 5); 

    ASSERT_EQ(bitreader_getByteOffset(&br), 5u);
    ASSERT_EQ(bitreader_getBitOffset(&br), 40u); // 5 bytes
}
