#pragma once

#include <vector>

#include "Image.h"

class IFilter {
public:
    virtual void Apply(Image &image) const = 0;
};

class IConvFilter : public IFilter {
public:
    virtual void Apply(Image &image) const {
        std::vector<std::vector<float>> kernel = GetKernel();
        // for
    }

    virtual std::vector<std::vector<float>> GetKernel() const = 0;
};

class NegFilter : public IFilter {
    void Apply(Image &image) const override {
        for (int i = 0; i < image.height_; ++i) {
            for (int j = 0; j < image.width_; ++j) {
                image.pixels_[i][j].blue = 255 - image.pixels_[i][j].blue;
                image.pixels_[i][j].green = 255 - image.pixels_[i][j].green;
                image.pixels_[i][j].red = 255 - image.pixels_[i][j].red;
            }
        }
    }
};

class CropFilter : public IFilter {
public:
    CropFilter(const std::vector<std::string> &params) {
        x_ = std::stoi(params[0]);
        y_ = std::stoi(params[1]);
        width_ = std::stoi(params[2]);
        height_ = std::stoi(params[3]);
    }

private:
    int x_, y_, width_, height_;
    void Apply(Image &image) const override {
        Image new_image(width_, height_);
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                new_image.pixels_[i][j] = image.pixels_[y_ + i][x_ + j];
            }
        }
        image = new_image;
    }
};

class BlurFilter : public IConvFilter {
public:
    std::vector<std::vector<float>> GetKernel() const override {
        return {{5, 1, 3}, {2, 5, 1}, {1, 1, 1}};
    }
};

class GrayScaleFilter : public IFilter {
    void Apply(Image &image) const override {
        for (int i = 0; i < image.height_; ++i) {
            for (int j = 0; j < image.width_; ++j) {
                image.pixels_[i][j].blue = image.pixels_[i][j].green = image.pixels_[i][j].red =
                    0.114 * image.pixels_[i][j].blue + 0.587 * image.pixels_[i][j].green +
                    0.299 * image.pixels_[i][j].red;
            }
        }
    }
};
