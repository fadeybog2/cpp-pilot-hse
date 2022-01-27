#include "scorer.h"

#include <algorithm>

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    ScoreTable score_table;
    auto sorted_events = events;
    auto comp = [](const Event& lhs, const Event& rhs) { return lhs.time < rhs.time; };
    std::sort(sorted_events.begin(), sorted_events.end(), comp);
    for (size_t i = 0; i < sorted_events.size() && sorted_events[i].time <= score_time; ++i) {
        const auto& event = sorted_events[i];
        switch (event.event_type) {
            case EventType::CheckFailed:
                score_table[event.student_name].erase(event.task_name);
                break;
            case EventType::CheckSuccess:
                score_table[event.student_name].insert(event.task_name);
                break;
            case EventType::MergeRequestOpen:
                score_table[event.student_name].erase(event.task_name);
                break;
            case EventType::MergeRequestClosed:
                score_table[event.student_name].insert(event.task_name);
                break;
        }
        if (score_table[event.student_name].empty()) {
            score_table.erase(event.student_name);
        }
    }
    return score_table;
}
