#ifndef CRC_CALCULATOR_H
#define CRC_CALCULATOR_H

#include <vector>
#include <cstdint>
#include <utility>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

class CRCCalculator {
public:
    static uint16_t calculateCRC16(const std::vector<uint8_t>& data);
    
    static std::pair<uint64_t, uint16_t> performTimedCalculation(
        const std::vector<uint8_t>& data, 
        int repetitions
    );

private:
    static const uint8_t auchCRCHi[];
    static const uint8_t auchCRCLo[];
};

#endif // CRC_CALCULATOR_H