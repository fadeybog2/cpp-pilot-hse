#include "scorer.h"

void Scorer::OnCheckFailed(const StudentName &student_name, const TaskName &task_name) {
    student_tasks_[student_name][task_name].check_success = false;
    if (student_tasks_[student_name][task_name].merge_closed) {
        student_tasks_[student_name].erase(task_name);
        if (student_tasks_[student_name].empty()) {
            student_tasks_.erase(student_name);
        }
    }
}

void Scorer::OnCheckSuccess(const StudentName &student_name, const TaskName &task_name) {
    student_tasks_[student_name][task_name].check_success = true;
}

void Scorer::OnMergeRequestOpen(const StudentName &student_name, const TaskName &task_name) {
    student_tasks_[student_name][task_name].merge_closed = false;
}

void Scorer::OnMergeRequestClosed(const StudentName &student_name, const TaskName &task_name) {
    student_tasks_[student_name][task_name].merge_closed = true;
    if (!student_tasks_[student_name][task_name].check_success) {
        student_tasks_[student_name].erase(task_name);
        if (student_tasks_[student_name].empty()) {
            student_tasks_.erase(student_name);
        }
    }
}

void Scorer::Reset() {
    student_tasks_.clear();
}

ScoreTable Scorer::GetScoreTable() const {
    ScoreTable score_table;
    for (const auto &[student_name, tasks] : student_tasks_) {
        for (const auto &[task_name, task_state] : tasks) {
            if (task_state.merge_closed && task_state.check_success) {
                score_table[student_name].insert(task_name);
            }
        }
    }
    return score_table;
}