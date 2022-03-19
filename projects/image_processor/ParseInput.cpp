#include "ParseInput.h"

#include <stdexcept>
#include <string>

ParsedInput ParseCommandLine(int argc, char **argv) {
    ParsedInput parsed_input;
    parsed_input.input_file = std::string(argv[1]);
    if (!parsed_input.input_file.ends_with(".bmp")) {
        throw std::runtime_error("Error! Input file should be with .bmp extension.");
    }
    parsed_input.output_file = std::string(argv[2]);
    if (!parsed_input.output_file.ends_with(".bmp")) {
        throw std::runtime_error("Error! Output file should be with .bmp extension.");
    }
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

void ImageProcessor::Initialize(const std::vector<FilterOptions> &filter_options) {
    for (const auto &[filter_name, params] : filter_options) {
        if (filter_name == "-neg") {
            filters_.push_back(new NegFilter());
        } else if (filter_name == "-sharp") {
            filters_.push_back(new SharpFilter());
        } else if (filter_name == "-gs") {
            filters_.push_back(new GrayScaleFilter());
        } else if (filter_name == "-crop") {
            filters_.push_back(new CropFilter(params));
        } else if (filter_name == "-edge") {
            filters_.push_back(new EdgeFilter(params));
        } else if (filter_name == "-blur") {
            filters_.push_back(new BlurFilter(params));
        } else {
            throw std::runtime_error("Unknown filter!");
        }
    }
}

void ImageProcessor::ApplyFilters(Image &image) const {
    for (const auto &filter : filters_) {
        filter->Apply(image);
    }
}

ImageProcessor::~ImageProcessor() {
    for (const auto &ptr : filters_) {
        delete ptr;
    }
}
