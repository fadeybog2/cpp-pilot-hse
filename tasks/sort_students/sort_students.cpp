#include "sort_students.h"
#include <algorithm>

bool ComparatorDate(const Student& lhs, const Student& rhs) {
    auto lhs_tuple = std::tie(lhs.birth_date.year, lhs.birth_date.month, lhs.birth_date.day, lhs.last_name, lhs.name);
    auto rhs_tuple = std::tie(rhs.birth_date.year, rhs.birth_date.month, rhs.birth_date.day, rhs.last_name, rhs.name);

    return lhs_tuple < rhs_tuple;
}
bool ComparatorName(const Student& lhs, const Student& rhs) {
    auto lhs_tuple = std::tie(lhs.last_name, lhs.name, lhs.birth_date.year, lhs.birth_date.month, lhs.birth_date.day);
    auto rhs_tuple = std::tie(rhs.last_name, rhs.name, rhs.birth_date.year, rhs.birth_date.month, rhs.birth_date.day);

    return lhs_tuple < rhs_tuple;
}
void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    switch (sortKind) {
        case SortKind::Date:
            std::sort(students.begin(), students.end(), ComparatorDate);
            break;
        case SortKind::Name:
            std::sort(students.begin(), students.end(), ComparatorName);
            break;
    }
}
