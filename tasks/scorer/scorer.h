#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

enum class EventType {
    CheckFailed,
    CheckSuccess,
    MergeRequestOpen,
    MergeRequestClosed,
};

using StudentName = std::string;
using TaskName = std::string;

struct Event {
    StudentName student_name;
    TaskName task_name;
    time_t time;
    EventType event_type;
};

using Events = std::vector<Event>;
using ScoreTable = std::map<StudentName, std::set<TaskName>>;

ScoreTable GetScoredStudents(const Events& events, time_t score_time);
