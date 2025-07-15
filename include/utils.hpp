#pragma once
#include <string>
#include <chrono>
#include "order.hpp"

namespace Utils {
    std::string currentTimestamp();
    void log(const std::string& message);
    double roundToTwoDecimal(double value);
    std::string orderTypeToString(OrderType orderType);
}