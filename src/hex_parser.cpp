#include "hex_parser.h"
#include <QStringList>

// Parse a hex string into a vector of bytes
std::vector<uint8_t> HexParser::parseHexString(const QString& hexString) {
    std::vector<uint8_t> result;
    result.reserve(256); // Pre-allocate for maximum expected size
    
    // Fast path for empty input
    if (hexString.isEmpty()) {
        return result;
    }
    
    // Determine if input has spaces
    bool hasSpaces = hexString.contains(' ');
    
    if (hasSpaces) {
        // Input with spaces - split by spaces
        QStringList hexBytes = hexString.split(' ', Qt::SkipEmptyParts);
        
        // Reserve exact size needed
        result.reserve(hexBytes.size());
        
        for (const QString& hexByte : hexBytes) {
            bool ok;
            uint8_t byte = hexByte.toUInt(&ok, 16);
            if (ok) {
                result.push_back(byte);
            }
        }
    } else {
        // No spaces - process 2 characters at a time
        const int len = hexString.length();
        for (int i = 0; i < len; i += 2) {
            // Handle end of string
            if (i + 1 >= len) {
                // Last single character
                QString hexByte = hexString.mid(i, 1);
                bool ok;
                uint8_t byte = hexByte.toUInt(&ok, 16);
                if (ok) {
                    result.push_back(byte);
                }
                break;
            }
            
            // Normal case - 2 characters
            QString hexByte = hexString.mid(i, 2);
            bool ok;
            uint8_t byte = hexByte.toUInt(&ok, 16);
            if (ok) {
                result.push_back(byte);
            }
        }
    }
    
    return result;
}