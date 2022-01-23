#include <catch.hpp>

#include "tests_checking.h"

TEST_CASE("StudentsOrder") {
    const std::vector<StudentAction> students_actions = {
        {.name = "Ivanov", .side = Side::Top},
        {.name = "Petrov", .side = Side::Top},
        {.name = "Sidorov", .side = Side::Bottom},
    };

    const std::vector<size_t> queries = {1, 3};

    const std::vector<std::string> expected = {"Petrov", "Sidorov"};

    REQUIRE(expected == StudentsOrder(students_actions, queries));
}
