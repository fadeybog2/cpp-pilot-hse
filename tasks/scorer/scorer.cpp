#include "scorer.h"

#include <unordered_map>

struct Task {
    time_t time_check;
    time_t time_merge;
    bool check_success = false;
    bool merge_closed = true;
};

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    ScoreTable score_table;
    std::unordered_map<StudentName, std::unordered_map<TaskName, Task>> tasks;
    for (const auto& event : events) {
        if (event.time > score_time) {
            continue;
        }
        switch (event.event_type) {
            case EventType::CheckFailed:
                if (event.time > tasks[event.student_name][event.task_name].time_check) {
                    tasks[event.student_name][event.task_name].time_check = event.time;
                    tasks[event.student_name][event.task_name].check_success = false;
                }
                break;
            case EventType::MergeRequestOpen:
                if (event.time > tasks[event.student_name][event.task_name].time_merge) {
                    tasks[event.student_name][event.task_name].time_merge = event.time;
                    tasks[event.student_name][event.task_name].merge_closed = false;
                }
                break;
            case EventType::CheckSuccess:
                if (event.time > tasks[event.student_name][event.task_name].time_check) {
                    tasks[event.student_name][event.task_name].time_check = event.time;
                    tasks[event.student_name][event.task_name].check_success = true;
                }
                break;
            case EventType::MergeRequestClosed:
                if (event.time > tasks[event.student_name][event.task_name].time_merge) {
                    tasks[event.student_name][event.task_name].time_merge = event.time;
                    tasks[event.student_name][event.task_name].merge_closed = true;
                }
                break;
        }
    }
    for (const auto& st_pair : tasks) {
        for (const auto& task_pair : st_pair.second) {
            if (task_pair.second.merge_closed && task_pair.second.check_success) {
                score_table[st_pair.first].insert(task_pair.first);
            }
        }
    }
    return score_table;
}
