#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    int l = 0;
    int r = str.length() - 1;
    while (l <= r) {
        while (str[l] == ' ') {
            l++;
            if (l > r) {
                return true;
            }
        }
        while (str[r] == ' ') {
            r--;
            if (l > r) {
                return true;
            }
        }
        if (str[l] != str[r]) {
            return false;
        }
        l++;
        r--;
    }
    return true;
}
