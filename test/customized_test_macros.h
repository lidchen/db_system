#pragma once
#include <catch2/catch_test_macros.hpp>
#define REQUIRE_THROWS_WITH_TYPE(expr, expected_type) \
    try {                                             \
        expr;                                         \
        FAIL("Expected DbException to be thrown");    \
    } catch (const DbException& ex) {                 \
        REQUIRE(ex.type() == expected_type);          \
    }
