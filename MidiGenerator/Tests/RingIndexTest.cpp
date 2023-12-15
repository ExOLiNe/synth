#include <catch2/catch_test_macros.hpp>
#include "../Source/RingIndex.h"

TEST_CASE("Test RingIndex operator+") {
    RingIndex<int, 10> ringIndex(0);
    ringIndex = ringIndex + 11;
    REQUIRE(ringIndex.index == 1);
}

TEST_CASE("Test RingIndex operator-") {
    RingIndex<int, 10> ringIndex(0);
    ringIndex = ringIndex - 1;
    REQUIRE(ringIndex.index == 9);
}

TEST_CASE("Test RingIndex operator++") {
    RingIndex<int, 10> ringIndex(0);
    ++ringIndex;
    REQUIRE(ringIndex.index == 1);
}

TEST_CASE("Test RingIndex operator--") {
    RingIndex<int, 10> ringIndex(0);
    --ringIndex;
    REQUIRE(ringIndex.index == 9);
}