#include "password.h"

bool ValidatePassword(const std::string& password) {
    const int min_length = 8;
    const int max_length = 14;
    const int min_ascii = 33;
    const int max_ascii = 126;
    bool has_upper = false;
    bool has_lower = false;
    bool has_digit = false;
    bool has_other = false;
    if (password.length() < min_length || password.length() > max_length) {
        return false;
    }
    for (auto c : password) {
        if (std::islower(c)) {
            has_lower = true;
        } else if (std::isupper(c)) {
            has_upper = true;
        } else if (std::isdigit(c)) {
            has_digit = true;
        } else {
            has_other = true;
        }
        if (int(c) < min_ascii || int(c) > max_ascii) {
            return false;
        }
    }
    if (has_upper + has_lower + has_digit + has_other < 3) {
        return false;
    }
    return true;
}
