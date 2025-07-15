#include "../include/order.hpp"

Order::Order(const std::string& time, const std::string& sym, OrderType t, int qty, double prc)
    : timestamp(time), symbol(sym), type(t), quantity(qty), price(prc) {}
