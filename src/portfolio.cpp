#include "../include/portfolio.hpp"
#include <iostream>

Portfolio::Portfolio(double initialCash) 
    : cash(initialCash), shares(0), marketValue(0) {}

void Portfolio::executeOrder(const Order& order) {
    bool executed = false;
    if (order.type == OrderType::BUY && cash >= order.quantity * order.price) {
        cash -= order.quantity * order.price;
        shares += order.quantity;
        executed = true;
    } else if (order.type == OrderType::SELL && shares >= order.quantity) {
        cash += order.quantity * order.price;
        shares -= order.quantity;
        executed = true;
    }

    if (executed) {
        orderHistory.push_back(order);
    } else {
        std::cerr << "Trade failed: " << (order.type == OrderType::BUY ? "BUY" : "SELL") 
                  << " " << order.quantity << " " << order.symbol << " at $" << order.price 
                  << " (Insufficient funds/shares)\n";
    }
}

void Portfolio::updateMarketValue(const PriceData& data) {
    marketValue = shares * data.close;
}

void Portfolio::printSummary() {
    double total = cash + marketValue;
    std::cout << "=== Portfolio Summary ===\n";
    std::cout << "Cash: $" << cash << "\n";
    std::cout << "Shares: " << shares << "\n";
    std::cout << "Market Value: $" << marketValue << "\n";
    std::cout << "Total Equity: $" << total << "\n";
    std::cout << "Total Trades: " << orderHistory.size() << "\n";
}