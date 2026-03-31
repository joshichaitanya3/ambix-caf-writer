#include <gtest/gtest.h>
#include <cstdint>
#include <bit>
#include "../src/big_endian.h"

class EndianTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

// Test compile-time endianness detection
TEST_F(EndianTest, CompileTimeEndianDetection) {
    // This test verifies that std::endian works correctly
    EXPECT_TRUE(std::endian::native == std::endian::little || 
                std::endian::native == std::endian::big);

    // Most development machines are little-endian
    if (std::endian::native == std::endian::little) {
        std::cout << "Running on little-endian system" << std::endl;
    } else {
        std::cout << "Running on big-endian system" << std::endl;
    }
}

// Test 16-bit byte swapping
TEST_F(EndianTest, ByteSwap16Bit) {
    uint16_t original = 0x1234;
    uint16_t expected_swapped = 0x3412;

    if constexpr (std::endian::native == std::endian::little) {
        // On little-endian, ToBigEndian should swap bytes
        EXPECT_EQ(big_endian::ToBigEndian(original), expected_swapped);
    } else {
        // On big-endian, ToBigEndian should be no-op
        EXPECT_EQ(big_endian::ToBigEndian(original), original);
    }
}

// Test 32-bit byte swapping
TEST_F(EndianTest, ByteSwap32Bit) {
    uint32_t original = 0x12345678;
    uint32_t expected_swapped = 0x78563412;

    if constexpr (std::endian::native == std::endian::little) {
        EXPECT_EQ(big_endian::ToBigEndian(original), expected_swapped);
    } else {
        EXPECT_EQ(big_endian::ToBigEndian(original), original);
    }
}

// Test signed integers work correctly
TEST_F(EndianTest, SignedIntegerHandling) {
    int16_t original = -1234;  // 0xFB2E in two's complement

    // The bit pattern should be swapped, but the value interpretation
    // depends on endianness
    int16_t result = big_endian::ToBigEndian(original);

    if constexpr (std::endian::native == std::endian::little) {
        // Verify the bytes were actually swapped
        uint16_t* orig_ptr = reinterpret_cast<uint16_t*>(&original);
        uint16_t* result_ptr = reinterpret_cast<uint16_t*>(&result);
        EXPECT_NE(*orig_ptr, *result_ptr);
    }
}

// Test double precision floating point
TEST_F(EndianTest, DoubleByteSwap) {
    double original = 48000.0;  // Common sample rate
    double result = big_endian::ToBigEndian(original);

    if constexpr (std::endian::native == std::endian::big) {
        EXPECT_EQ(result, original);
    } else {
        // On little-endian, the bit pattern should change
        // but we can't easily predict the exact value
        uint64_t* orig_bits = reinterpret_cast<uint64_t*>(&original);
        uint64_t* result_bits = reinterpret_cast<uint64_t*>(&result);
        EXPECT_NE(*orig_bits, *result_bits);

        // Double conversion should be reversible
        double double_converted = big_endian::ToBigEndian(result);
        EXPECT_DOUBLE_EQ(double_converted, original);
    }
}

// Test FourCC codes (critical for CAF format)
TEST_F(EndianTest, FourCCCodes) {
    uint32_t caff_code = big_endian::MakeFourCC('c', 'a', 'f', 'f'); // 'caff'
    
    // This should create the correct big-endian representation
    // regardless of host endianness
    EXPECT_EQ((caff_code >> 24) & 0xFF, 'c');
    EXPECT_EQ((caff_code >> 16) & 0xFF, 'a');
    EXPECT_EQ((caff_code >> 8) & 0xFF, 'f');
    EXPECT_EQ(caff_code & 0xFF, 'f');
}

// Test round-trip conversion
TEST_F(EndianTest, RoundTripConversion) {
    std::vector<uint32_t> test_values = {
        0x00000000, 0xFFFFFFFF, 0x12345678, 0x87654321,
        0x00FF00FF, 0xFF00FF00, 0x01020304
    };

    for (uint32_t original : test_values) {
        uint32_t converted = big_endian::ToBigEndian(original);
        uint32_t back = big_endian::ToBigEndian(converted);
        EXPECT_EQ(back, original) << "Round-trip failed for 0x" 
                                  << std::hex << original;
    }
}
