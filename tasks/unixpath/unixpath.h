#pragma once

#include <string>
#include <string_view>

std::string NormalizePath(std::string_view current_working_dir, std::string_view path);
