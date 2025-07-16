#pragma once

#include "order.hpp"

class PortfolioBase {
public:
    virtual ~PortfolioBase() = default;
    virtual void executeOrder(const Order& order) = 0;
    virtual double getCash() const = 0;
    virtual int getShares(const std::string& symbol) const = 0;
};