#pragma once

#include <string>
#include "PortfolioBase.hpp"

class LivePortfolio : public PortfolioBase {
public:
    LivePortfolio(const std::string& apiKey, const std::string& secretKey);
    void executeOrder(const Order& order) override;
    double getCash() const override;
    int getShares(const std::string& symbol) const override;
    void printAccountSummary();

private:
    std::string apiKey;
    std::string secretKey;
    std::string tradingApiUrl = "https://paper-api.alpaca.markets";
};