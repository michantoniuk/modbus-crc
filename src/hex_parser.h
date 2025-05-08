#ifndef HEX_PARSER_H
#define HEX_PARSER_H

#include <vector>
#include <cstdint>
#include <QString>

// Class for parsing hexadecimal string input
class HexParser {
public:
    // Parse a hexadecimal string into a vector of bytes
    // Supports both space-separated and continuous formats
    static std::vector<uint8_t> parseHexString(const QString& hexString);
};

#endif // HEX_PARSER_H