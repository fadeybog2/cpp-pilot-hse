#include "tests_checking.h"

#include <list>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::vector<std::string> answer;
    std::vector<std::string> stack;
    for (const auto& action : student_actions) {
        switch (action.side) {
            case Side::Bottom:
                stack.push_back(action.name);
                break;
            case Side::Top:
                stack.insert(stack.begin(), action.name);
                break;
        }
    }
    for (const auto& query : queries) {
        answer.push_back(stack[query - 1]);
    }
    return answer;
}
