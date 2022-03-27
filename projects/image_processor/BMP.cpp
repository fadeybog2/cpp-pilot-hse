#include "BMP.h"

#include <fstream>
#include <stdexcept>
#include <vector>

Image BMPReader(const std::string &filename) {
    std::ifstream file(filename, std::fstream::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error! Unable to open the input image file.");
    }
    BMPFileHeader file_header;
    BMPInfoHeader info_header;
    file.read(reinterpret_cast<char *>(&file_header), sizeof(BMPFileHeader));
    if (file_header.file_type != 0x4d42) {
        throw std::runtime_error("Error! Wrong file format.");
    }
    file.read(reinterpret_cast<char *>(&info_header), sizeof(BMPInfoHeader));
    if (info_header.bit_count != 24) {
        throw std::runtime_error("Error! This program supports only 24bit images.");
    }
    Image image(info_header.width, info_header.height);
    auto padding = (4 - (info_header.width * 3) % 4) % 4;
    uint8_t r, g, b;
    for (ssize_t i = info_header.height - 1; i >= 0; --i) {
        for (int j = 0; j < info_header.width; ++j) {
            file.read(reinterpret_cast<char *>(&b), 1);
            file.read(reinterpret_cast<char *>(&g), 1);
            file.read(reinterpret_cast<char *>(&r), 1);
            image.pixels_[i][j].b = b / 255.;
            image.pixels_[i][j].g = g / 255.;
            image.pixels_[i][j].r = r / 255.;
        }
        file.seekg(padding, file.cur);
    }
    file.close();
    return image;
}

void BMPWriter(Image &image, const std::string &filename) {
    std::ofstream file{filename, std::fstream::binary};
    if (!file.is_open()) {
        throw std::runtime_error("Error! Unable to open the output image file.");
    }

    BMPFileHeader file_header;
    BMPInfoHeader info_header;

    info_header.width = image.width_;
    info_header.height = image.height_;
    auto padding = (4 - (image.width_ * 3) % 4) % 4;
    info_header.image_size = 3 * image.height_ * (image.width_ + padding);
    file_header.file_size = file_header.offset_data + info_header.image_size;

    file.write(reinterpret_cast<char *>(&file_header), sizeof(BMPFileHeader));
    file.write(reinterpret_cast<char *>(&info_header), sizeof(BMPInfoHeader));
    uint8_t r, g, b;
    for (ssize_t i = image.height_ - 1; i >= 0; --i) {
        for (int j = 0; j < info_header.width; ++j) {
            b = static_cast<uint8_t>(image.pixels_[i][j].b * 255);
            g = static_cast<uint8_t>(image.pixels_[i][j].g * 255);
            r = static_cast<uint8_t>(image.pixels_[i][j].r * 255);
            file.write(reinterpret_cast<char *>(&b), 1);
            file.write(reinterpret_cast<char *>(&g), 1);
            file.write(reinterpret_cast<char *>(&r), 1);
        }
        file.seekp(padding, file.cur);
    }
    file.close();
}