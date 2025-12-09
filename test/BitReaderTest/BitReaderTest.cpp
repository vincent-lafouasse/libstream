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
    MockReader mockReader("ABC");  // irrelevant
    Reader reader = mockReaderInterface(&mockReader);

    BitReader br = bitreader_init(&reader);

    ASSERT_EQ(bitreader_getByteOffset(&br), 0u);
    ASSERT_EQ(br.subOffset, 0u);
    ASSERT_TRUE(bitreader_isByteAligned(&br));
}

TEST(BitReader, IsByteAligned_Correctness)
{
    MockReader mockReader("\xFF");
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);
    uint32_t bit;

    ASSERT_TRUE(bitreader_isByteAligned(&br));

    bitreader_takeSingleBit(&br, &bit);
    ASSERT_FALSE(bitreader_isByteAligned(&br));
    ASSERT_EQ(br.subOffset, 1u);

    for (int i = 0; i < 7; ++i) {
        bitreader_takeSingleBit(&br, &bit);
    }
    ASSERT_TRUE(bitreader_isByteAligned(&br));

    // we read the entire buffer (1 byte)
    ASSERT_IS_EOF(bitreader_takeSingleBit(&br, &bit));
}

TEST(BitReader, TakeSingleBit_BitExtractionAndSubOffset)
{
    //  0xAA = 0b10101010
    MockReader mockReader("\xAA");
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);
    uint32_t bit;

    // Initial state
    ASSERT_EQ(br.subOffset, 0u);
    ASSERT_EQ(reader_offset(&reader), 0u);

    // --- MSB:   1 ---
    ASSERT_IS_OK(bitreader_takeSingleBit(&br, &bit));
    ASSERT_EQ(bit, 1u);
    ASSERT_EQ(br.subOffset, 1u);
    ASSERT_EQ(reader_offset(&reader), 1u);  // Byte taken, offset advanced

    // --- bit 2: 0 ---
    ASSERT_IS_OK(bitreader_takeSingleBit(&br, &bit));
    ASSERT_EQ(bit, 0u);
    ASSERT_EQ(br.subOffset, 2u);
    ASSERT_EQ(reader_offset(&reader), 1u);  // still on same byte

    // --- bit 7: 1 ---
    for (int i = 0; i < 4; ++i) {  // Skip bits 3-6
        bitreader_takeSingleBit(&br, &bit);
    }
    ASSERT_IS_OK(
        bitreader_takeSingleBit(&br, &bit));  // Bit 7 (index 6 in currentByte)
    ASSERT_EQ(bit, 1u);
    ASSERT_EQ(br.subOffset, 7u);
    ASSERT_EQ(reader_offset(&reader), 1u);

    // --- Read 8th Bit (0) - Boundary Condition ---
    ASSERT_IS_OK(bitreader_takeSingleBit(&br, &bit));
    ASSERT_EQ(bit, 0u);
    ASSERT_EQ(br.subOffset, 0u);  // SubOffset resets
    ASSERT_EQ(reader_offset(&reader), 1u);
    // Reader offset remains at 1, no second read
}

TEST(BitReader, TakeSingleBit_LoadNextByte)
{
    MockReader mockReader("\xAA\xFF");
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);
    uint32_t bit;

    // Consume first 8 bits (to put br.subOffset=0 and reader_offset=1)
    for (int i = 0; i < 8; ++i) {
        bitreader_takeSingleBit(&br, &bit);
    }
    ASSERT_EQ(reader_offset(&reader), 1u);
    ASSERT_EQ(br.subOffset, 0u);

    // --- Read 9th Bit (1) - Triggers New Byte Load ---
    ASSERT_IS_OK(bitreader_takeSingleBit(&br, &bit));
    ASSERT_EQ(bit, 1u);                     // MSB of 0xFF
    ASSERT_EQ(br.subOffset, 1u);            // SubOffset advances to 1
    ASSERT_EQ(reader_offset(&reader), 2u);  // Reader offset advances to 2
    ASSERT_EQ(br.currentByte, 0xFFu);
}

TEST(BitReader, ByteAlign_UnalignedState)
{
    MockReader mockReader("\xAA");
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);
    uint32_t bit;

    for (int i = 0; i < 3; ++i) {
        bitreader_takeSingleBit(&br, &bit);
    }

    // Initial state for align
    ASSERT_EQ(br.subOffset, 3u);
    ASSERT_EQ(reader_offset(&reader), 1u);

    size_t skipped = bitreader_byteAlign(&br);

    ASSERT_EQ(skipped, 5u);  // 8 - 3 = 5 bits skipped
    ASSERT_EQ(br.subOffset, 0u);
    ASSERT_EQ(reader_offset(&reader), 1u);  // no read has been triggered
    ASSERT_TRUE(bitreader_isByteAligned(&br));
}

TEST(BitReader, ByteAlign_AlreadyAligned)
{
    MockReader mockReader("A");
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);

    size_t skipped = bitreader_byteAlign(&br);

    ASSERT_EQ(skipped, 0u);
    ASSERT_EQ(br.subOffset, 0u);
    ASSERT_EQ(reader_offset(&reader), 0u);
}

TEST(BitReader, SkipBytes_UnalignedStart)
{
    MockReader mockReader("\xAA\xBB\xCC\xDD");
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);
    uint32_t bit;

    // Read 2 bits (subOffset=2). Reader offset=1.
    bitreader_takeSingleBit(&br, &bit);
    bitreader_takeSingleBit(&br, &bit);
    ASSERT_EQ(br.subOffset, 2u);
    ASSERT_EQ(reader_offset(&reader), 1u);  // Byte 0xAA taken

    // skip 2 bytes (0xBB and 0xCC).
    // this MUST: 1) align (skip 6 bits of 0xAA). 2) reader_skip(2).
    ASSERT_IS_OK(bitreader_skipBytes(&br, 2));

    ASSERT_EQ(br.subOffset, 0u);  // Must be aligned
    ASSERT_EQ(reader_offset(&reader), 3u);

    // verify next read starts on 0xDD (Byte 3)
    // the next takeSingleBit will load 0xDD (MSB=1)
    ASSERT_IS_OK(bitreader_takeSingleBit(&br, &bit));
    ASSERT_EQ(bit, 1u);
    ASSERT_EQ(reader_offset(&reader), 4u);  // 4th byte has been read
}

TEST(BitReader, TakeBits_SingleBit)
{
    // 0b10101010
    MockReader mockReader("\xAA");
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);
    uint32_t result;

    // read MSB (1)
    ASSERT_IS_OK(bitreader_takeBits(&br, 1, &result));

    ASSERT_EQ(result, 1u);
    ASSERT_EQ(br.subOffset, 1u);
    ASSERT_EQ(bitreader_getByteOffset(&br), 1u);
}

TEST(BitReader, TakeBits_ExactlyOneByte)
{
    MockReader mockReader("\xAB\xCD");
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);
    uint32_t result;

    ASSERT_IS_OK(bitreader_takeBits(&br, 8, &result));

    ASSERT_EQ(result, 0xABu);
    ASSERT_EQ(br.subOffset, 0u);
    ASSERT_EQ(bitreader_getByteOffset(&br), 1u);
    ASSERT_TRUE(bitreader_isByteAligned(&br));
}

TEST(BitReader, TakeBits_AcrossBoundary_FragmentOnly)
{
    // 0xAA (10101010) 0xBB (10111011)
    MockReader mockReader("\xAA\xBB");
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);
    uint32_t result;

    // Put into unaligned state: Read 5 bits (10101). subOffset=5, ByteOffset=1.
    // Remaining in 0xAA: 010 (3 bits).
    ASSERT_IS_OK(bitreader_takeBits(&br, 5, &result));
    ASSERT_EQ(result, 0b10101u);

    // Read 5 more bits. Should take 3 from 0xAA, then 2 from 0xBB (Total 5).
    // Expected result: 010 | 10 = 0b01010
    ASSERT_IS_OK(bitreader_takeBits(&br, 5, &result));

    ASSERT_EQ(result, 0b01010u);
    ASSERT_EQ(br.subOffset, 2u);  // 2 bits consumed from 0xBB
    ASSERT_EQ(bitreader_getByteOffset(&br),
              2u);  // 0xBB is now the current byte
}

TEST(BitReader, TakeBits_AcrossBoundary_8BitsTotal)
{
    // Stream 0xAA 0xBB. Read 4 bits (1010). subOffset=4, ByteOffset=1.
    // Remaining in 0xAA: 1010 (4 bits).
    MockReader mockReader("\xAA\xBB");
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);
    uint32_t result;

    ASSERT_IS_OK(bitreader_takeBits(&br, 4, &result));
    ASSERT_EQ(br.subOffset, 4u);

    // Read 8 bits. Should take 4 from 0xAA and 4 from 0xBB.
    // 0xAA fragment: 1010. 0xBB fragment: 1011.
    // Expected: 1010 | 1011 = 0b10101011
    ASSERT_IS_OK(bitreader_takeBits(&br, 8, &result));

    ASSERT_EQ(result, 0b10101011u);
    ASSERT_EQ(br.subOffset, 4u);  // 4 bits consumed from 0xBB
    ASSERT_EQ(bitreader_getByteOffset(&br), 2u);
}
