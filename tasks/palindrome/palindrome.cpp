#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    int l = 0;
    int r = str.length() - 1;
    while (l <= r) {
        while(str[l] == ' '){
            l++;
        }
        while(str[r] == ' '){
            r--;
        }
        if (str[l] != str[r]){
            return false;
        }
        l++;
        r--;
    }
    return true;
}
