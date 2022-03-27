#pragma once

#include <vector>

class Image {
public:
    struct Pixel {
        float b;
        float g;
        float r;
    };

    Image(int width, int height) : width_(width), height_(height), pixels_(height_, std::vector<Pixel>(width_)) {
    }

    int32_t width_;
    int32_t height_;
    std::vector<std::vector<Pixel>> pixels_;
};