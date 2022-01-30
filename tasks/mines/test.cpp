#include <catch.hpp>

#include "minesweeper.h"

TEST_CASE("Minesweeper") {
    // *211....
    // 12*1....
    // .111....

    Minesweeper minesweeper(8, 3, {{.x = 0, .y = 0}, {.x = 2, .y = 1}});

    {
        const Minesweeper::RenderedField expected = {
            "--------",
            "--------",
            "--------",
        };

        REQUIRE(expected == minesweeper.RenderField());
    }
    {
        minesweeper.OpenCell({.x = 2, .y = 0});

        const Minesweeper::RenderedField expected = {
            "--1-----",
            "--------",
            "--------",
        };

        REQUIRE(expected == minesweeper.RenderField());
    }
    {
        minesweeper.MarkCell({.x = 5, .y = 1});
        minesweeper.OpenCell({.x = 7, .y = 2});

        const Minesweeper::RenderedField expected = {
            "--11....",
            "---1.?..",
            "---1....",
        };

        REQUIRE(expected == minesweeper.RenderField());
    }
    {
        minesweeper.OpenCell({.x = 0, .y = 2});

        const Minesweeper::RenderedField expected = {
            "--11....",
            "12-1.?..",
            ".1-1....",
        };

        REQUIRE(expected == minesweeper.RenderField());
    }
    {
        minesweeper.MarkCell({.x = 0, .y = 0});
        minesweeper.OpenCell({.x = 1, .y = 0});
        minesweeper.OpenCell({.x = 2, .y = 2});
        minesweeper.MarkCell({.x = 5, .y = 1});
        minesweeper.OpenCell({.x = 5, .y = 1});

        const Minesweeper::RenderedField expected = {
            "?211....",
            "12-1....",
            ".111....",
        };

        REQUIRE(expected == minesweeper.RenderField());
        REQUIRE(Minesweeper::GameStatus::VICTORY == minesweeper.GetGameStatus());
    }
    {
        minesweeper.NewGame(1000, 1000, 1'000'000);
        minesweeper.OpenCell({.x = 0, .y = 0});

        REQUIRE(Minesweeper::GameStatus::DEFEAT == minesweeper.GetGameStatus());

        for (const auto& line : minesweeper.RenderField()) {
            REQUIRE(std::all_of(line.begin(), line.end(), [](auto ch) { return ch == '*'; }));
        }
    }
}
