#pragma once

#include <string>
#include <vector>

enum class Side {
    Top,
    Bottom,
};

struct StudentAction {
    std::string name;
    Side side = Side::Top;
};

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries);
