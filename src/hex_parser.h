#ifndef HEX_PARSER_H
#define HEX_PARSER_H

#include <vector>
#include <cstdint>
#include <QString>

class HexParser {
public:
    static std::vector<uint8_t> parseHexString(const QString& hexString);
};

#endif // HEX_PARSER_H