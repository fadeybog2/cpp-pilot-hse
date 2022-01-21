#include "password.h"

bool ValidatePassword(const std::string& password) {
    const int MIN_LENGTH = 8;
    const int MAX_LENGTH = 14;
    const int MIN_ASCII = 33;
    const int MAX_ASCII = 126;
    bool HasUpper = false;
    bool HasLower = false;
    bool HasDigit = false;
    bool HasOther = false;
    if (password.length() < MIN_LENGTH || password.length() > MAX_LENGTH) {
        return false;
    }
    for (auto c : password) {
        if (std::islower(c)) {
            HasLower = true;
        } else if (std::isupper(c)) {
            HasUpper = true;
        } else if (std::isdigit(c)) {
            HasDigit = true;
        } else {
            HasOther = true;
        }
        if (int(c) < MIN_ASCII || int(c) > MAX_ASCII) {
            return false;
        }
    }
    if (HasUpper + HasLower + HasDigit + HasOther < 3) {
        return false;
    }
    return true;
}
