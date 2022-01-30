#include "scorer.h"

#include <unordered_map>

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    ScoreTable score_table;
    std::unordered_map<StudentName, std::unordered_map<TaskName, time_t>> tasks_time;
    for (const auto& event : events) {
        if (event.time > score_time || event.time < tasks_time[event.student_name][event.task_name]) {
            continue;
        }
        switch (event.event_type) {
            case EventType::CheckFailed:
            case EventType::MergeRequestOpen:
                score_table[event.student_name].erase(event.task_name);
                break;
            case EventType::CheckSuccess:
            case EventType::MergeRequestClosed:
                score_table[event.student_name].insert(event.task_name);
                break;
        }
        tasks_time[event.student_name][event.task_name] = event.time;
        if (score_table[event.student_name].empty()) {
            score_table.erase(event.student_name);
        }
    }
    return score_table;
}
