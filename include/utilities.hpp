#pragma once

#include <string>

class Utilities {
public:
  static inline int binarytoDecimal(const std::string &binary) {
    return stoi(binary, 0, 2);
  }

  static inline std::string decimalToBinary(int num, size_t bitLength) {
    if (num == 0) {
      return std::string(bitLength, '0');
    }

    std::string binaryString;

    while (num > 0) {
      binaryString = static_cast<char>((num % 2) + '0') + binaryString;
      num /= 2;
    }

    // Pad with zeros to achieve the desired bit length
    size_t currentLength = binaryString.length();
    if (currentLength < bitLength) {
      binaryString = std::string(bitLength - currentLength, '0') + binaryString;
    }

    return binaryString;
  }
};