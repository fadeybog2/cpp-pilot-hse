#include "tests_checking.h"

#include <list>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::vector<std::string> answer;
    std::list<std::string> stack;
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
    std::vector<std::string> stack_vec(stack.begin(), stack.end());
    for (const auto& query : queries) {
        answer.push_back(stack_vec[query - 1]);
    }
    return answer;
}
