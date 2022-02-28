#pragma once

#include <vector>

#include "Image.h"

class IFilter {
public:
    virtual void Apply(Image &image) const = 0;
};

class IConvFilter : public IFilter {
public:
    void Apply(Image &image) const override {
        std::vector<std::vector<float>> kernel = GetKernel();
        int kernel_width = kernel.front().size();
        int kernel_height = kernel.size();
        Image new_image(image);
        for (int i = 0; i < image.height_; ++i) {
            for (int j = 0; j < image.width_; ++j) {
                float red = 0;
                float green = 0;
                float blue = 0;
                for (int k = 0; k < kernel_height; ++k) {
                    for (int l = 0; l < kernel_width; ++l) {
                        int x = std::clamp(j + l - kernel_width / 2, 0, image.width_ - 1);
                        int y = std::clamp(i + k - kernel_height / 2, 0, image.height_ - 1);
                        red += image.pixels_[y][x].red * kernel[k][l];
                        green += image.pixels_[y][x].green * kernel[k][l];
                        blue += image.pixels_[y][x].blue * kernel[k][l];
                    }
                }
                new_image.pixels_[i][j].red = std::min(255, std::max(0, static_cast<int>(red)));
                new_image.pixels_[i][j].green = std::min(255, std::max(0, static_cast<int>(green)));
                new_image.pixels_[i][j].blue = std::min(255, std::max(0, static_cast<int>(blue)));
            }
        }
        image = new_image;
    }

    virtual std::vector<std::vector<float>> GetKernel() const = 0;
};

class NegFilter : public IFilter {
public:
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
    explicit CropFilter(const std::vector<std::string> &params) {
        x_ = std::stoi(params[0]);
        y_ = std::stoi(params[1]);
        width_ = std::stoi(params[2]);
        height_ = std::stoi(params[3]);
    }
    void Apply(Image &image) const override {
        Image new_image(width_, height_);
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                new_image.pixels_[i][j] = image.pixels_[y_ + i][x_ + j];
            }
        }
        image = new_image;
    }

private:
    unsigned int x_;
    unsigned int y_;
    unsigned int width_;
    unsigned int height_;
};

class SharpFilter : public IConvFilter {
public:
    std::vector<std::vector<float>> GetKernel() const override {
        return {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    }
};

class GrayScaleFilter : public IFilter {
public:
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

class EdgeFilter : public IConvFilter {
public:
    explicit EdgeFilter(std::vector<std::string> params) : threshold_(std::stoi(params[0])) {
    }
    std::vector<std::vector<float>> GetKernel() const override {
        return {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    }
    void Apply(Image &image) const override {
        GrayScaleFilter().Apply(image);
        IConvFilter::Apply(image);
        for (int i = 0; i < image.height_; ++i) {
            for (int j = 0; j < image.width_; ++j) {
                if (image.pixels_[i][j].blue > threshold_) {
                    image.pixels_[i][j].blue = image.pixels_[i][j].green = image.pixels_[i][j].red = 255;
                } else {
                    image.pixels_[i][j].blue = image.pixels_[i][j].green = image.pixels_[i][j].red = 0;
                }
            }
        }
    }

private:
    float threshold_;
};
