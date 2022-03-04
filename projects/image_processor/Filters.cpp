#include "Filters.h"

#include <cmath>
#include <iostream>

void IConvFilter::Apply(Image &image) const {
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
            new_image.pixels_[i][j].red = std::clamp(static_cast<int>(red), 0, 255);
            new_image.pixels_[i][j].green = std::clamp(static_cast<int>(green), 0, 255);
            new_image.pixels_[i][j].blue = std::clamp(static_cast<int>(blue), 0, 255);
        }
        std::cout << i << std::endl;
    }
    image = new_image;
}

void NegFilter::Apply(Image &image) const {
    for (int i = 0; i < image.height_; ++i) {
        for (int j = 0; j < image.width_; ++j) {
            image.pixels_[i][j].blue = 255 - image.pixels_[i][j].blue;
            image.pixels_[i][j].green = 255 - image.pixels_[i][j].green;
            image.pixels_[i][j].red = 255 - image.pixels_[i][j].red;
        }
    }
}

CropFilter::CropFilter(const std::vector<std::string> &params) {
    x_ = std::stoi(params[0]);
    y_ = std::stoi(params[1]);
    width_ = std::stoi(params[2]);
    height_ = std::stoi(params[3]);
}

void CropFilter::Apply(Image &image) const {
    Image new_image(width_, height_);
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            new_image.pixels_[i][j] = image.pixels_[y_ + i][x_ + j];
        }
    }
    image = new_image;
}

std::vector<std::vector<float>> SharpFilter::GetKernel() const {
    return {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
}

void GrayScaleFilter::Apply(Image &image) const {
    for (int i = 0; i < image.height_; ++i) {
        for (int j = 0; j < image.width_; ++j) {
            image.pixels_[i][j].blue = image.pixels_[i][j].green = image.pixels_[i][j].red =
                0.114 * image.pixels_[i][j].blue + 0.587 * image.pixels_[i][j].green + 0.299 * image.pixels_[i][j].red;
        }
    }
}

EdgeFilter::EdgeFilter(std::vector<std::string> params) : threshold_(std::stoi(params[0])) {
}

std::vector<std::vector<float>> EdgeFilter::GetKernel() const {
    return {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
}

void EdgeFilter::Apply(Image &image) const {
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

BlurFilter::BlurFilter(std::vector<std::string> params) : sigma_(std::stof(params[0])) {
}

std::vector<std::vector<float>> BlurFilter::GetKernel() const {
    int size = 6 * sigma_;
    std::vector<std::vector<float>> kernel(size, std::vector<float>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            kernel[i][j] =
                1 / (2 * M_PI * sigma_ * sigma_) *
                exp(-((size / 2 - j) * (size / 2 - j) + (size / 2 - i) * (size / 2 - i)) / (2 * sigma_ * sigma_));
        }
    }
    return kernel;
}
