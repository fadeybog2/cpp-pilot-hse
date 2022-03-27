#include "Filters.h"

#include <cmath>

void IConvFilter::Apply(Image &image) const {
    std::vector<std::vector<float>> kernel = GetKernel();
    int kernel_width = kernel.front().size();
    int kernel_height = kernel.size();
    Image new_image(image.width_, image.height_);
    for (int i = 0; i < image.height_; ++i) {
        for (int j = 0; j < image.width_; ++j) {
            float r = 0;
            float g = 0;
            float b = 0;
            for (int k = 0; k < kernel_height; ++k) {
                for (int l = 0; l < kernel_width; ++l) {
                    int x = std::clamp(j + l - kernel_width / 2, 0, image.width_ - 1);
                    int y = std::clamp(i + k - kernel_height / 2, 0, image.height_ - 1);
                    r += image.pixels_[y][x].r * kernel[k][l];
                    g += image.pixels_[y][x].g * kernel[k][l];
                    b += image.pixels_[y][x].b * kernel[k][l];
                }
            }
            new_image.pixels_[i][j].r = std::clamp(r, 0.f, 1.f);
            new_image.pixels_[i][j].g = std::clamp(g, 0.f, 1.f);
            new_image.pixels_[i][j].b = std::clamp(b, 0.f, 1.f);
        }
    }
    image = new_image;
}

void NegFilter::Apply(Image &image) const {
    for (auto &line : image.pixels_) {
        for (auto &pixel : line) {
            pixel.b = 1 - pixel.b;
            pixel.g = 1 - pixel.g;
            pixel.r = 1 - pixel.r;
        }
    }
}

CropFilter::CropFilter(const std::vector<std::string> &params) {
    x_ = std::stoul(params[0]);
    y_ = std::stoul(params[1]);
    width_ = std::stoul(params[2]);
    height_ = std::stoul(params[3]);
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
    for (auto &line : image.pixels_) {
        for (auto &pixel : line) {
            pixel.b = pixel.g = pixel.r = 0.114 * pixel.b + 0.587 * pixel.g + 0.299 * pixel.r;
        }
    }
}

EdgeFilter::EdgeFilter(const std::vector<std::string> &params) : threshold_(std::stof(params[0])) {
}

std::vector<std::vector<float>> EdgeFilter::GetKernel() const {
    return {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
}

void EdgeFilter::Apply(Image &image) const {
    GrayScaleFilter().Apply(image);
    IConvFilter::Apply(image);
    for (auto &line : image.pixels_) {
        for (auto &pixel : line) {
            if (pixel.b > threshold_) {
                pixel.b = pixel.g = pixel.r = 1;
            } else {
                pixel.b = pixel.g = pixel.r = 0;
            }
        }
    }
}

BlurFilter::BlurFilter(const std::vector<std::string> &params) : sigma_(std::stof(params[0])) {
}

std::vector<std::vector<float>> BlurFilter::GetKernel() const {
    size_t size = 6 * sigma_;
    std::vector<std::vector<float>> kernel(size, std::vector<float>(size));
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            kernel[i][j] = 1 / (2 * M_PI * pow(sigma_, 2)) *
                           exp(-(pow(size / 2 - j, 2) + pow(size / 2 - i, 2)) / (2 * pow(sigma_, 2)));
        }
    }
    return kernel;
}
