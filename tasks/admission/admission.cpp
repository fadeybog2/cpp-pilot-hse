#include "admission.h"

#include <algorithm>

bool ComparatorApplicant(const Applicant* lhs, const Applicant* rhs) {
    int lhs_points = -lhs->points;
    int rhs_points = -rhs->points;
    auto lhs_tuple = std::tie(lhs_points, lhs->student.birth_date.year, lhs->student.birth_date.month,
                              lhs->student.birth_date.day, lhs->student.name);
    auto rhs_tuple = std::tie(rhs_points, rhs->student.birth_date.year, rhs->student.birth_date.month,
                              rhs->student.birth_date.day, rhs->student.name);

    return lhs_tuple < rhs_tuple;
}

bool ComparatorStudent(const Student* lhs, const Student* rhs) {
    auto lhs_tuple = std::tie(lhs->name, lhs->birth_date.year, lhs->birth_date.month, lhs->birth_date.day);
    auto rhs_tuple = std::tie(rhs->name, rhs->birth_date.year, rhs->birth_date.month, rhs->birth_date.day);

    return lhs_tuple < rhs_tuple;
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> appls;
    for (const auto& appl : applicants) {
        appls.push_back(&appl);
    }
    std::sort(appls.begin(), appls.end(), ComparatorApplicant);

    std::unordered_map<std::string, size_t> max_students;
    for (const auto& univ : universities) {
        max_students[univ.name] = univ.max_students;
    }

    AdmissionTable adm_table;
    for (const auto& appl : appls) {
        for (const auto& wish_univer : appl->wish_list) {
            if (adm_table[wish_univer].size() < max_students[wish_univer]) {
                adm_table[wish_univer].push_back(&appl->student);
                break;
            }
        }
    }
    for (auto& univ : adm_table) {
        std::sort(univ.second.begin(), univ.second.end(), ComparatorStudent);
    }
    return adm_table;
}
