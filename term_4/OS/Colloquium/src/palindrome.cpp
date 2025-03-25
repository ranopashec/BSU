//
// Created by Simon on 25/03/2025.
//

#include "../include/palindrome.h"
#include <string>
#include <algorithm>

bool isPalindrome(int number) {
    if (number < 0) {
        return false;
    }

    std::string str = std::to_string(number);
    std::string reversedStr = str;
    std::reverse(reversedStr.begin(), reversedStr.end());

    return str == reversedStr;
}
