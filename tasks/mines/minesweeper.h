#pragma once

#include <string>
#include <vector>

class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

private:
    GameStatus game_status_ = GameStatus::NOT_STARTED;
    time_t start_time_;
    time_t end_time_;
    size_t mines_count_;
    size_t opened_count_;
    enum class CellState {
        FLAGGED,
        OPENED,
        CLOSED,
    };
    struct FieldCell {
        bool IsMine = false;
        CellState cell_state = CellState::CLOSED;
    };
    size_t width_;
    size_t height_;
    FieldCell** field_;
};
