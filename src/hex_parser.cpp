#include "hex_parser.h"
#include <QStringList>

std::vector<uint8_t> HexParser::parseHexString(const QString& hexString) {
    std::vector<uint8_t> result;
    result.reserve(256);
    
    if (hexString.isEmpty()) {
        return result;
    }
    
    bool hasSpaces = hexString.contains(' ');
    
    if (hasSpaces) {
        QStringList hexBytes = hexString.split(' ', Qt::SkipEmptyParts);
        
        result.reserve(hexBytes.size());
        
        for (const QString& hexByte : hexBytes) {
            bool ok;
            uint8_t byte = hexByte.toUInt(&ok, 16);
            if (ok) {
                result.push_back(byte);
            }
        }
    } else {
        const int len = hexString.length();
        for (int i = 0; i < len; i += 2) {
            if (i + 1 >= len) {
                QString hexByte = hexString.mid(i, 1);
                bool ok;
                uint8_t byte = hexByte.toUInt(&ok, 16);
                if (ok) {
                    result.push_back(byte);
                }
                break;
            }
            
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