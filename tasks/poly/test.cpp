#include <catch.hpp>

#include "poly.h"

#include <sstream>

TEST_CASE("Poly") {
    const Poly poly0;                    // y = 0
    Poly poly1({1, 0, 3});               // y = 1 + 0x + 3x^2
    const Poly poly2({{0, 1}, {2, 3}});  // y = 1 + 3x^2

    REQUIRE(0 == poly0(1));
    REQUIRE(301 == poly1(10));
    REQUIRE(3458764513820540929 == poly1(1 << 30));

    REQUIRE(poly0 != poly1);
    REQUIRE(poly1 == poly2);

    REQUIRE(poly0 == poly1 - poly2);
    REQUIRE(Poly({{0, 2}, {2, 6}}) == poly1 + poly2);

    poly1 += poly2;
    poly1 -= poly2;
    REQUIRE(poly1 == poly2);

    REQUIRE(Poly({{0, 1}, {2, 6}, {4, 9}}) == poly1 * poly2);

    REQUIRE(Poly({{0, -1}, {2, -3}}) == -poly2);

    std::stringstream string_stream;
    std::ostream& stream = string_stream;
    stream << poly1 << ", " << poly0;
    REQUIRE("y = 3x^2 + 1, y = 0" == string_stream.str());

    Poly poly3 = poly0;
    REQUIRE(poly0 == poly3);

    poly3 = poly1;
    REQUIRE(poly1 == poly3);
}
