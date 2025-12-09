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

TEST(BitReader, TakeBits_AllThreePhases)
{
    // 0xFF 0x11 0x22 0x33 0x44
    // 0b1111'1111  0xff
    // 0b0001'0001  0x11
    // 0b0010'0010  0x22
    // 0b0011'0011  0x33
    // 0b0100'0100  0x44
    MockReader mockReader("\xFF\x11\x22\x33\x44");
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);
    uint32_t result;

    // 1. Put into unaligned state: Read 3 bits (111). subOffset=3,
    // ByteOffset=1. Remaining in 0xFF: 5 bits (11111).
    // 0b___1'1111  0xff
    // 0b0001'0001  0x11
    // 0b0010'0010  0x22
    // 0b0011'0011  0x33
    // 0b0100'0100  0x44
    ASSERT_IS_OK(bitreader_takeBits(&br, 3, &result));
    ASSERT_EQ(result, 0b111u);

    // 2. read 24 bits from the stream so those bits
    // 0b___1'1111  0xff
    // 0b0001'0001  0x11
    // 0b0010'0010  0x22
    // 0b001_'____  0x33
    // 0b____'____  0x44
    ASSERT_IS_OK(bitreader_takeBits(&br, 24, &result));
    ASSERT_EQ(result, 0b11111'00010001'00100010'001u);
    // now remains
    // 0b____'____  0xff
    // 0b____'____  0x11
    // 0b____'____  0x22
    // 0b___1'0011  0x33
    // 0b0100'0100  0x44

    // 3. read 2 bits bc why not
    ASSERT_IS_OK(bitreader_takeBits(&br, 2, &result));
    ASSERT_EQ(result, 0b10u);
    // now remains
    // 0b____'____  0xff
    // 0b____'____  0x11
    // 0b____'____  0x22
    // 0b____'_011  0x33
    // 0b0100'0100  0x44

    // 3. read 9 bits bc why not
    ASSERT_IS_OK(bitreader_takeBits(&br, 9, &result));
    ASSERT_EQ(result, 0b011'0100'01u);
    // now remains
    // 0b____'____  0xff
    // 0b____'____  0x11
    // 0b____'____  0x22
    // 0b____'____  0x33
    // 0b____'__00  0x44

    // 4. empty out the stream
    ASSERT_IS_OK(bitreader_takeBits(&br, 2, &result));
    ASSERT_EQ(result, 0u);

    // 5. done
    ASSERT_IS_EOF(bitreader_takeBits(&br, 1, &result));
}

TEST(BitReader, TakeBits_FullBytes_NoFragments)
{
    // Stream 0x12 0x34 0x56 0x78. Start aligned.
    MockReader mockReader("\x12\x34\x56\x78");
    Reader reader = mockReaderInterface(&mockReader);
    BitReader br = bitreader_init(&reader);
    uint32_t result;

    // Read 24 bits (3 full bytes)
    ASSERT_IS_OK(bitreader_takeBits(&br, 24, &result));

    ASSERT_EQ(result, 0x123456u);
    ASSERT_EQ(br.subOffset, 0u);
    ASSERT_EQ(bitreader_getByteOffset(&br), 3u);  // 3 bytes taken
}
