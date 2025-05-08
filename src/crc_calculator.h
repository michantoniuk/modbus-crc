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

// CRC-16 Calculator for Modbus RTU
class CRCCalculator {
public:
    // Calculate CRC for a vector of bytes
    static uint16_t calculateCRC16(const std::vector<uint8_t>& data);
    
    // Calculate CRC multiple times and measure time
    // Returns a pair of <total time in ms, CRC result>
    static std::pair<uint64_t, uint16_t> performTimedCalculation(
        const std::vector<uint8_t>& data, 
        int repetitions
    );

private:
    // Lookup tables for CRC calculation
    static const uint8_t auchCRCHi[];
    static const uint8_t auchCRCLo[];
};

#endif // CRC_CALCULATOR_H