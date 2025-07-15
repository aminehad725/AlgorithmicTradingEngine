#pragma once
#include <string>

enum class OrderType { BUY, SELL, HOLD};

struct Order {
    std::string timestamp;
    std::string symbol;
    OrderType type;
    int quantity;
    double price;

    Order(const std::string& time, const std::string& sym, OrderType t, int qty, double prc);

};