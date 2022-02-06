#include "minesweeper.h"

#include <random>

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    NewGame(width, height, mines_count);
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    NewGame(width, height, cells_with_mines);
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    FreeMemory();
    game_status_ = GameStatus::NOT_STARTED;
    mines_count_ = mines_count;
    opened_count_ = 0;
    height_ = height;
    width_ = width;
    AllocateMemory();

    SetMinesRandomly();
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    FreeMemory();
    game_status_ = GameStatus::NOT_STARTED;
    mines_count_ = cells_with_mines.size();
    opened_count_ = 0;
    height_ = height;
    width_ = width;
    AllocateMemory();
    for (const auto& cell : cells_with_mines) {
        field_[cell.y][cell.x].is_mined = true;
    }
}

void Minesweeper::OpenCell(const Minesweeper::Cell& cell) {
    if (game_status_ == GameStatus::NOT_STARTED) {
        start_time_ = time(nullptr);
        game_status_ = GameStatus::IN_PROGRESS;
    }
    if (field_[cell.y][cell.x].is_flagged || field_[cell.y][cell.x].is_opened) {
        return;
    }
    if (game_status_ == GameStatus::VICTORY || game_status_ == GameStatus::DEFEAT) {
        return;
    }
    field_[cell.y][cell.x].is_opened = true;
    ++opened_count_;
    if (field_[cell.y][cell.x].is_mined) {
        game_status_ = GameStatus::DEFEAT;
        end_time_ = time(nullptr);
        for (size_t i = 0; i < height_; ++i) {
            for (size_t j = 0; j < width_; ++j) {
                field_[i][j].is_opened = true;
            }
        }
        return;
    }
    if (opened_count_ == width_ * height_ - mines_count_) {
        game_status_ = GameStatus::VICTORY;
        end_time_ = time(nullptr);
        return;
    }

    if (!CountMines(cell.x, cell.y)) {
        for (size_t k = cell.y; k <= cell.y + 2; ++k) {
            for (size_t l = cell.x; l <= cell.x + 2; ++l) {
                if (0 < k && k <= height_ && 0 < l && l <= width_) {
                    if (!field_[k - 1][l - 1].is_opened) {
                        cells_to_open_.push_back(Cell{.x = l - 1, .y = k - 1});
                    }
                }
            }
        }
    }
    while (!cells_to_open_.empty()) {
        auto cur_cell = cells_to_open_.front();
        cells_to_open_.pop_front();
        OpenCell(cur_cell);
    }
}

void Minesweeper::MarkCell(const Minesweeper::Cell& cell) {
    if (game_status_ == GameStatus::NOT_STARTED) {
        start_time_ = time(nullptr);
        game_status_ = GameStatus::IN_PROGRESS;
    }
    if (game_status_ == GameStatus::VICTORY || game_status_ == GameStatus::DEFEAT || field_[cell.y][cell.x].is_opened) {
        return;
    }
    if (field_[cell.y][cell.x].is_flagged) {
        field_[cell.y][cell.x].is_flagged = false;
        return;
    }
    field_[cell.y][cell.x].is_flagged = true;
}

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return game_status_;
}

time_t Minesweeper::GetGameTime() const {
    switch (game_status_) {
        case GameStatus::NOT_STARTED:
            return 0;
            break;
        case GameStatus::IN_PROGRESS:
            return time(nullptr) - start_time_;
            break;
        case GameStatus::VICTORY:
        case GameStatus::DEFEAT:
            return end_time_ - start_time_;
            break;
    }
}

Minesweeper::RenderedField Minesweeper::RenderField() const {
    RenderedField rendered_field(height_, std::string(width_, '-'));
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            if (field_[i][j].is_flagged) {
                rendered_field[i][j] = '?';
                continue;
            }
            if (field_[i][j].is_opened) {
                if (field_[i][j].is_mined) {
                    rendered_field[i][j] = '*';
                } else {
                    int mines_around_count = CountMines(j, i);
                    if (mines_around_count == 0) {
                        rendered_field[i][j] = '.';
                    } else {
                        rendered_field[i][j] = '0' + mines_around_count;
                    }
                }
                continue;
            }
        }
    }
    return rendered_field;
}

Minesweeper::~Minesweeper() {
    FreeMemory();
}

int Minesweeper::CountMines(size_t x, size_t y) const {
    int mines_around_count = 0;
    for (size_t k = y; k <= y + 2; ++k) {
        for (size_t l = x; l <= x + 2; ++l) {
            if (0 < k && k <= height_ && 0 < l && l <= width_) {
                mines_around_count += field_[k - 1][l - 1].is_mined;
            }
        }
    }
    return mines_around_count;
}

void Minesweeper::FreeMemory() {
    for (size_t i = 0; i < height_; ++i) {
        delete[] field_[i];
    }
    delete[] field_;
}

void Minesweeper::AllocateMemory() {
    field_ = new CellData*[height_];
    for (size_t i = 0; i < height_; ++i) {
        field_[i] = new CellData[width_];
    }
}

void Minesweeper::SetMinesRandomly() {
    std::vector<Cell> indexes;
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            indexes.push_back(Cell{.x = j, .y = i});
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indexes.begin(), indexes.end(), g);
    for (size_t i = 0; i < mines_count_; ++i) {
        field_[indexes[i].y][indexes[i].x].is_mined = true;
    }
}
