#include "minesweeper.h"

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    NewGame(width, height, mines_count);
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    NewGame(width, height, cells_with_mines);
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    game_status_ = GameStatus::NOT_STARTED;
    mines_count_ = mines_count;
    opened_count_ = 0;
    height_ = height;
    width_ = width;
    field_ = new FieldCell*[height_];
    for (size_t i = 0; i < height_; ++i) {
        field_[i] = new FieldCell[width_];
    }
    srand(time(nullptr));
    for (int i = 0; i < mines_count_;) {
        size_t x = rand() % height_;
        size_t y = rand() % width_;
        if (!field_[x][y].IsMine) {
            field_[x][y].IsMine = true;
            ++i;
        }
    }
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    game_status_ = GameStatus::NOT_STARTED;
    mines_count_ = cells_with_mines.size();
    opened_count_ = 0;
    height_ = height;
    width_ = width;
    field_ = new FieldCell*[height_];
    for (size_t i = 0; i < height_; ++i) {
        field_[i] = new FieldCell[width_];
    }
    for (const auto& cell : cells_with_mines) {
        field_[cell.y][cell.x].IsMine = true;
    }
}

void Minesweeper::OpenCell(const Minesweeper::Cell& cell) {
    if (game_status_ == GameStatus::NOT_STARTED) {
        start_time_ = time(nullptr);
        game_status_ = GameStatus::IN_PROGRESS;
    }
    if (field_[cell.y][cell.x].cell_state == CellState::FLAGGED) {
        return;
    }
    if (game_status_ == GameStatus::IN_PROGRESS) {
        field_[cell.y][cell.x].cell_state = CellState::OPENED;
        ++opened_count_;
        if (field_[cell.y][cell.x].IsMine) {
            game_status_ = GameStatus::DEFEAT;
            end_time_ = time(nullptr);
            for (size_t i = 0; i < height_; ++i) {
                for (size_t j = 0; j < width_; ++j) {
                    field_[i][j].cell_state = CellState::OPENED;
                }
            }
            RenderField();
        } else {
            if (opened_count_ == width_ * height_ - mines_count_) {
                game_status_ = GameStatus::VICTORY;
                end_time_ = time(nullptr);
            }
            bool are_mines_around = false;
            for (size_t k = cell.y; k <= cell.y + 2; ++k) {
                for (size_t l = cell.x; l <= cell.x + 2; ++l) {
                    if (0 < k && k <= height_ && 0 < l && l <= width_) {
                        if (field_[k - 1][l - 1].IsMine) {
                            are_mines_around = true;
                        }
                    }
                }
            }
            if (!are_mines_around) {
                for (size_t k = cell.y; k <= cell.y + 2; ++k) {
                    for (size_t l = cell.x; l <= cell.x + 2; ++l) {
                        if (0 < k && k <= height_ && 0 < l && l <= width_) {
                            if (field_[k - 1][l - 1].cell_state == CellState::CLOSED) {
                                OpenCell(Cell{.x = l - 1, .y = k - 1});
                            }
                        }
                    }
                }
            }
        }
    }
}

void Minesweeper::MarkCell(const Minesweeper::Cell& cell) {
    if (game_status_ == GameStatus::NOT_STARTED) {
        start_time_ = time(nullptr);
        game_status_ = GameStatus::IN_PROGRESS;
    }
    if (game_status_ == GameStatus::IN_PROGRESS) {
        switch (field_[cell.y][cell.x].cell_state) {
            case CellState::FLAGGED:
                field_[cell.y][cell.x].cell_state = CellState::OPENED;
                break;
            case CellState::CLOSED:
                field_[cell.y][cell.x].cell_state = CellState::FLAGGED;
                break;
            case CellState::OPENED:
                break;
        }
    }
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
    RenderedField rendered_field(height_);
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            switch (field_[i][j].cell_state) {
                case CellState::FLAGGED:
                    rendered_field[i].push_back('?');
                    break;
                case CellState::OPENED:
                    if (field_[i][j].IsMine) {
                        rendered_field[i].push_back('*');
                    } else {
                        int mines_around_count = 0;
                        for (size_t k = i; k <= i + 2; ++k) {
                            for (size_t l = j; l <= j + 2; ++l) {
                                if (0 < k && k <= height_ && 0 < l && l <= width_) {
                                    mines_around_count += field_[k - 1][l - 1].IsMine;
                                }
                            }
                        }
                        if (mines_around_count == 0) {
                            rendered_field[i].push_back('.');
                        } else {
                            rendered_field[i].push_back('0' + mines_around_count);
                        }
                    }
                    break;
                case CellState::CLOSED:
                    rendered_field[i].push_back('-');
                    break;
            }
        }
    }
    return rendered_field;
}
