#pragma once

#include <vector>

class Image {
public:
    struct Pixel {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };

    Image(int width, int height) : width_(width), height_(height) {
        pixels_.resize(height_);
        for (int i = 0; i < height_; ++i) {
            pixels_[i].resize(width_);
        }
    }

    int32_t width_;
    int32_t height_;
    std::vector<std::vector<Pixel>> pixels_;
};