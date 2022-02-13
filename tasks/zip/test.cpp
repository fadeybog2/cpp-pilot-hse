#include <catch.hpp>

#include "zip.h"

#include <sstream>

TEST_CASE("Zip") {
    const std::forward_list<Value> a = {"1", "2", "3", "4"};
    const std::forward_list<Value> b = {"one", "two", "three"};
    std::stringstream stream;

    for (const auto& pair : Zip(a.begin(), a.end(), b.begin(), b.end())) {
        stream << pair.first << ":" << pair.second << " ";
    }

    REQUIRE("1:one 2:two 3:three " == stream.str());
}
