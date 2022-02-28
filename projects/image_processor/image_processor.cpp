#include "BMP.h"
#include "Image.h"
#include "ParseInput.h"

int main(int argc, char **argv) {
    const auto [input_file, output_file, filter_options] = ParseCommandLine(argc, argv);

    Image image = BMPReader(input_file);

    ImageProcessor image_processor;
    image_processor.Initialize(filter_options);
    image_processor.ApplyFilters(image);

    BMPWriter(image, output_file);
    return 0;
}