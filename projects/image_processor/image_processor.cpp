#include <string>
#include <vector>

#include "BMP.h"
#include "Filters.h"
#include "Image.h"

struct FilterOptions {
    std::string filter_name;
    std::vector<std::string> params;
};

struct ParsedInput {
    std::string input_file;
    std::string output_file;
    std::vector<FilterOptions> filter_options;
};

ParsedInput ParseCommandLine(int argc, char **argv) {
    ParsedInput parsed_input;
    parsed_input.input_file = std::string(argv[1]);
    parsed_input.output_file = std::string(argv[2]);
    for (int i = 3; i < argc; ++i) {
        std::string str(argv[i]);
        if (str.starts_with('-')) {
            parsed_input.filter_options.push_back({str, {}});
        } else {
            parsed_input.filter_options.back().params.push_back(str);
        }
    }
    return parsed_input;
}

class ImageProcessor {
    std::vector<IFilter *> filters_;

public:
    ImageProcessor(const std::vector<FilterOptions> &filter_options) {
        for (const auto &[filter_name, params] : filter_options) {
            if (filter_name == "-neg") {
                filters_.push_back(new NegFilter());
            } else if (filter_name == "-blur") {
                filters_.push_back(new BlurFilter());
            } else if (filter_name == "-gs") {
                filters_.push_back(new GrayScaleFilter());
            } else if (filter_name == "-crop") {
                filters_.push_back(new CropFilter(params));
            } else {
                throw std::runtime_error("Unknown filter!");
            }
        }
    }

    void ApplyFilters(Image &image) {
        for (const auto &filter : filters_) {
            filter->Apply(image);
        }
    }
};

int main(int argc, char **argv) {
    const auto parsed_input = ParseCommandLine(argc, argv);

    Image image = BMPReader(parsed_input.input_file);

    ImageProcessor image_processor(parsed_input.filter_options);
    image_processor.ApplyFilters(image);

    BMPWriter(image, parsed_input.output_file);
    return 0;
}