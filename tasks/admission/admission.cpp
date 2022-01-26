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

    AdmissionTable adm_table;
    for (const auto& appl : appls) {
        for (const auto& wish_univer : appl->wish_list) {
            auto find_univ1 = [&wish_univer](auto arg) { return arg.first == wish_univer; };
            auto it = std::find_if(adm_table.begin(), adm_table.end(), find_univ1);
            if (it == adm_table.end()) {
                auto find_appl = [&appl](auto& arg) { return &arg.student == &appl->student; };
                std::vector<const Student*> stud_vector{
                    &std::find_if(applicants.begin(), applicants.end(), find_appl)->student};
                adm_table.insert(std::pair(wish_univer, stud_vector));
                break;
            } else {
                size_t av_places;
                auto find_univ2 = [&wish_univer](auto& arg) { return arg.name == wish_univer; };
                av_places = std::find_if(universities.begin(), universities.end(), find_univ2)->max_students;
                if (adm_table[wish_univer].size() < av_places) {
                    adm_table[wish_univer].push_back(&appl->student);
                    break;
                }
            }
        }
    }
    for (auto& univ : adm_table) {
        std::sort(univ.second.begin(), univ.second.end(), ComparatorStudent);
    }
    return adm_table;
}
