#include <catch.hpp>

#include "scorer.h"

TEST_CASE("GetScoredStudents") {
    Events events{
        {.student_name = "Ivanov", .task_name = "utf8", .time = 1600000001, .event_type = EventType::CheckSuccess},
        {.student_name = "Ivanov", .task_name = "scorer", .time = 1600000002, .event_type = EventType::CheckSuccess},
        {.student_name = "Petrov", .task_name = "utf8", .time = 1600000003, .event_type = EventType::CheckSuccess},
        {.student_name = "Petrov", .task_name = "scorer", .time = 1600000004, .event_type = EventType::CheckFailed},
        {.student_name = "Sidorov", .task_name = "scorer", .time = 1600000005, .event_type = EventType::CheckFailed},
    };

    ScoreTable expected_result = {
        {"Ivanov", {"utf8", "scorer"}},
        {"Petrov", {"utf8"}},
    };

    REQUIRE(expected_result == GetScoredStudents(events, 1600000010));
}
