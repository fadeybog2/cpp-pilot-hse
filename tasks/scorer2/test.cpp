#include <catch.hpp>

#include "scorer.h"

TEST_CASE("Scorer") {
    Scorer scorer;

    scorer.OnCheckSuccess("Popov", "old");
    scorer.Reset();

    scorer.OnCheckSuccess("Ivanov", "utf8");
    scorer.OnCheckSuccess("Ivanov", "scorer");
    scorer.OnCheckSuccess("Petrov", "utf8");
    scorer.OnCheckFailed("Petrov", "scorer");
    scorer.OnCheckFailed("Sidorov", "scorer");

    const ScoreTable expected_result = {
        {"Ivanov", {"utf8", "scorer"}},
        {"Petrov", {"utf8"}},
    };

    REQUIRE(expected_result == scorer.GetScoreTable());
}
