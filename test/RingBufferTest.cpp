#include "RingBuffer.h"

#include "gtest/gtest.h"

TEST(RingBuffer, all)
{
    RingBuffer rb(10);

    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(0, rb.avail());
    EXPECT_EQ(10, rb.free());

    char output[12] = "###########";

    size_t res = rb.read(output, 5);

    EXPECT_EQ(0, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(0, rb.avail());
    EXPECT_EQ(10, rb.free());

    res = rb.write("hello", 6);

    EXPECT_EQ(6, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(6, rb.avail());
    EXPECT_EQ(4, rb.free());

    res = rb.read(output, 6);

    EXPECT_EQ(6, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(0, rb.avail());
    EXPECT_EQ(10, rb.free());
    EXPECT_STREQ("hello", output);

    res = rb.write("goodbye", 8);

    EXPECT_EQ(8, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(8, rb.avail());
    EXPECT_EQ(2, rb.free());

    res = rb.read(output, 8);

    EXPECT_EQ(8, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(0, rb.avail());
    EXPECT_EQ(10, rb.free());
    EXPECT_STREQ("goodbye", output);

    res = rb.write("longeststringeveronearth", 25);

    EXPECT_EQ(10, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(10, rb.avail());
    EXPECT_EQ(0, rb.free());

    res = rb.read(output, 25);

    EXPECT_EQ(10, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(0, rb.avail());
    EXPECT_EQ(10, rb.free());
    EXPECT_STREQ("longeststr#", output);

    res = rb.write("abcdef", 6);

    EXPECT_EQ(6, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(6, rb.avail());
    EXPECT_EQ(4, rb.free());

    res = rb.read(output, 4);

    EXPECT_EQ(4, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(2, rb.avail());
    EXPECT_EQ(8, rb.free());

    res = rb.write("ninechars", 9);
    EXPECT_EQ(8, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(10, rb.avail());
    EXPECT_EQ(0, rb.free());

    res = rb.read(output, 10);

    EXPECT_EQ(10, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(0, rb.avail());
    EXPECT_EQ(10, rb.free());

    int8_t i8 = 88;
    int16_t i16 = 16161;
    int32_t i32 = 323232323;

    int8_t o8 = 0;
    int16_t o16 = 0;
    int32_t o32 = 0;

    res = rb.write(i8);
    EXPECT_EQ(1, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(1, rb.avail());
    EXPECT_EQ(9, rb.free());

    res = rb.read(o16);
    EXPECT_EQ(0, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(1, rb.avail());
    EXPECT_EQ(9, rb.free());

    res = rb.write(i16);
    EXPECT_EQ(2, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(3, rb.avail());
    EXPECT_EQ(7, rb.free());

    res = rb.write(i32);
    EXPECT_EQ(4, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(7, rb.avail());
    EXPECT_EQ(3, rb.free());

    res = rb.write(i32);
    EXPECT_EQ(0, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(7, rb.avail());
    EXPECT_EQ(3, rb.free());

    res = rb.read(o8);
    EXPECT_EQ(1, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(6, rb.avail());
    EXPECT_EQ(4, rb.free());
    EXPECT_EQ(i8, o8);

    res = rb.read(o16);
    EXPECT_EQ(2, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(4, rb.avail());
    EXPECT_EQ(6, rb.free());
    EXPECT_EQ(i16, o16);

    res = rb.read(o32);
    EXPECT_EQ(4, res);
    EXPECT_EQ(10, rb.size());
    EXPECT_EQ(0, rb.avail());
    EXPECT_EQ(10, rb.free());
    EXPECT_EQ(i32, o32);

    RingBuffer big(4096);
    char buffer[4096];
    big.write(buffer, 4096);
    big.read(buffer, 4095);
    res = big.write(buffer, 1000);
    res = big.write(buffer, 1000);
    EXPECT_EQ(1000, res);
    EXPECT_EQ(4096, big.size());
    EXPECT_EQ(2001, big.avail());
    EXPECT_EQ(2095, big.free());
}





