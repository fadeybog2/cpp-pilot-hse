#include <string>

#include "Image.h"

#pragma pack(push, 1)

struct BMPFileHeader {
    uint16_t file_type{0x4d42};
    uint32_t file_size;
    uint16_t reserved;
    uint16_t reserved2;
    uint32_t offset_data{54};
};

struct BMPInfoHeader {
    uint32_t size{40};
    int32_t width;
    int32_t height;
    uint16_t planes{1};
    uint16_t bit_count{24};
    uint32_t compression{0};
    uint32_t image_size;
    int32_t x_pixels_per_meter{0};
    int32_t y_pixels_per_meter{0};
    uint32_t colors_used{0};
    uint32_t colors_important{0};
};

#pragma pack(pop)

Image BMPReader(const std::string &filename);

void BMPWriter(Image &image, const std::string &filename);