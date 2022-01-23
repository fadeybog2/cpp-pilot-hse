#include <catch.hpp>

#include "admission.h"

TEST_CASE("FillUniversities") {
    const std::vector<University> universities = {
        {.name = "MSU", .max_students = 1}, {.name = "HSE", .max_students = 2}, {.name = "MIPT", .max_students = 100}};

    const std::vector<Applicant> applicants = {
        Applicant{.student = {.name = "Ivan Ivanov", .birth_date = {.day = 1, .month = 1, .year = 1900}},
                  .points = 100,
                  .wish_list = {"MSU", "HSE"}},
        Applicant{.student = {.name = "Petr Petrov", .birth_date = {.day = 2, .month = 1, .year = 1900}},
                  .points = 90,
                  .wish_list = {"MSU", "HSE"}},
        Applicant{.student = {.name = "Alexander Sidorov", .birth_date = {.day = 3, .month = 1, .year = 1900}},
                  .points = 110,
                  .wish_list = {"MIPT", "HSE"}},
        Applicant{.student = {.name = "Ivan Petrov", .birth_date = {.day = 3, .month = 1, .year = 1900}},
                  .points = 100,
                  .wish_list = {"HSE", "MSU", "MIPT"}},
        Applicant{.student = {.name = "Petr Ivanov", .birth_date = {.day = 4, .month = 1, .year = 1900}},
                  .points = 80,
                  .wish_list = {"HSE"}}};

    const AdmissionTable expected = {{"HSE", {&applicants[3].student, &applicants[1].student}},
                                     {"MIPT", {&applicants[2].student}},
                                     {"MSU", {&applicants[0].student}}};

    REQUIRE(expected == FillUniversities(universities, applicants));
}
