#pragma once

#include <deque>
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
    ~Minesweeper();

private:
    GameStatus game_status_ = GameStatus::NOT_STARTED;
    time_t start_time_;
    time_t end_time_;
    size_t mines_count_;
    size_t opened_count_;
    struct CellData {
        bool is_mined = false;
        bool is_flagged = false;
        bool is_opened = false;
    };
    size_t width_ = 0;
    size_t height_ = 0;
    CellData** field_ = nullptr;
    int CountMines(size_t x, size_t y) const;
    void FreeMemory();
    void AllocateMemory();
    void SetMinesRandomly();
    std::deque<Cell> cells_to_open_;
};
