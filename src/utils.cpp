#include "../include/utils.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <chrono>
#include "../include/order.hpp" 
#include <cmath>

namespace Utils {

    std::string currentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    void log(const std::string& message) {
        std::cout << "[" << currentTimestamp() << "] " << message << std::endl;
    }

    double roundToTwoDecimal(double value) {
        return std::round(value * 100.0) / 100.0;
    }

    std::string orderTypeToString(OrderType orderType) { 
        switch (orderType) {
            case OrderType::BUY:
                return "BUY";
            case OrderType::SELL:
                return "SELL";
            case OrderType::HOLD:
                return "HOLD";
            default:
                return "UNKNOWN";
        }
    }

}