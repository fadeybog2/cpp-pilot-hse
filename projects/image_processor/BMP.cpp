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
    std::vector<uint8_t> padding_row((4 - (info_header.width * 3) % 4) % 4);
    for (int i = info_header.height - 1; i >= 0; --i) {
        file.read(reinterpret_cast<char *>(image.pixels_[i].data()), 3 * info_header.width);
        file.read(reinterpret_cast<char *>(padding_row.data()), padding_row.size());
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
    int width_in_bytes = 3 * image.width_ + (4 - (image.width_ * 3) % 4) % 4;
    info_header.image_size = image.height_ * width_in_bytes;
    file_header.file_size = file_header.offset_data + info_header.image_size;

    file.write(reinterpret_cast<char *>(&file_header), sizeof(BMPFileHeader));
    file.write(reinterpret_cast<char *>(&info_header), sizeof(BMPInfoHeader));
    for (int i = image.height_ - 1; i >= 0; --i) {
        file.write(reinterpret_cast<char *>(image.pixels_[i].data()), width_in_bytes);
    }
    file.close();
}