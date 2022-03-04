#pragma once

#include <vector>

#include "Filters.h"

struct FilterOptions {
    std::string filter_name;
    std::vector<std::string> params;
};

struct ParsedInput {
    std::string input_file;
    std::string output_file;
    std::vector<FilterOptions> filter_options;
};

ParsedInput ParseCommandLine(int argc, char **argv);

class ImageProcessor {
    std::vector<IFilter *> filters_;

public:
    void Initialize(const std::vector<FilterOptions> &filter_options);

    void ApplyFilters(Image &image) const;
};
