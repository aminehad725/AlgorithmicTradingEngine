#include "../include/BacktestPortfolio.hpp"
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

BacktestPortfolio::BacktestPortfolio(double initialCash, double costPerTrade)
    : cash(initialCash),
      initialCapital(initialCash),
      transactionCost(costPerTrade) {}

void BacktestPortfolio::executeOrder(const Order& order) {
    int currentShares = getShares(order.symbol);

    if (order.type == OrderType::BUY && cash >= (order.quantity * order.price) + transactionCost) {
        cash -= (order.quantity * order.price) + transactionCost;
        positions[order.symbol] = currentShares + order.quantity;
        orderHistory.push_back(order);
    } else if (order.type == OrderType::SELL && currentShares >= order.quantity) {
        cash += (order.quantity * order.price) - transactionCost;
        positions[order.symbol] = currentShares - order.quantity;
        orderHistory.push_back(order);
    }
}

void BacktestPortfolio::updateMarketValue(const std::map<std::string, PriceData>& latestPrices) {
    double marketValue = 0.0;
    for (const auto& pair : positions) {
        std::string symbol = pair.first;
        int shares = pair.second;
        if (latestPrices.count(symbol)) {
            marketValue += shares * latestPrices.at(symbol).close;
        }
    }
    double totalValue = cash + marketValue;
    historicalValues.push_back(totalValue);
}

double BacktestPortfolio::getCash() const {
    return cash;
}

int BacktestPortfolio::getShares(const std::string& symbol) const {
    if (positions.count(symbol)) {
        return positions.at(symbol);
    }
    return 0;
}

void BacktestPortfolio::printPerformanceReport() {
    if (historicalValues.size() < 2) {
        std::cout << "Not enough data for performance report.\n";
        return;
    }

    std::vector<double> returns;
    for (size_t i = 1; i < historicalValues.size(); ++i) {
        returns.push_back((historicalValues[i] - historicalValues[i - 1]) / historicalValues[i - 1]);
    }

    const int tradingDays = 252;
    const double riskFreeRate = 0.02;

    double meanReturn = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    
    double stdDev = 0.0;
    for (const auto& r : returns) {
        stdDev += (r - meanReturn) * (r - meanReturn);
    }
    stdDev = std::sqrt(stdDev / returns.size());

    double downsideDev = 0.0;
    int negativeCount = 0;
    for (const auto& r : returns) {
        if (r < 0) {
            downsideDev += r * r;
            negativeCount++;
        }
    }
    downsideDev = (negativeCount > 0) ? std::sqrt(downsideDev / negativeCount) : 0;
    
    double annualizedReturn = meanReturn * tradingDays;
    double annualizedVolatility = stdDev * std::sqrt(tradingDays);
    double annualizedDownsideDev = downsideDev * std::sqrt(tradingDays);

    double sharpeRatio = (annualizedVolatility > 0) ? (annualizedReturn - riskFreeRate) / annualizedVolatility : 0.0;
    double sortinoRatio = (annualizedDownsideDev > 0) ? (annualizedReturn - riskFreeRate) / annualizedDownsideDev : 0.0;

    double peak = historicalValues[0];
    double maxDrawdown = 0.0;
    for (const auto val : historicalValues) {
        if (val > peak) {
            peak = val;
        }
        double drawdown = (peak - val) / peak;
        if (drawdown > maxDrawdown) {
            maxDrawdown = drawdown;
        }
    }

    double endingValue = historicalValues.back();
    double years = static_cast<double>(historicalValues.size()) / tradingDays;
    double cagr = (years > 0) ? std::pow(endingValue / initialCapital, 1.0 / years) - 1.0 : 0.0;

    std::cout << "\n📈 === Performance Report === 📈\n";
    std::cout << "------------------------------------\n";
    std::cout << "Ending Equity: $" << endingValue << "\n";
    std::cout << "Total Return: " << ((endingValue / initialCapital) - 1.0) * 100.0 << "%\n";
    std::cout << "CAGR: " << cagr * 100.0 << "%\n";
    std::cout << "------------------------------------\n";
    std::cout << "Annualized Volatility: " << annualizedVolatility * 100.0 << "%\n";
    std::cout << "Max Drawdown: " << maxDrawdown * 100.0 << "%\n";
    std::cout << "------------------------------------\n";
    std::cout << "Sharpe Ratio: " << sharpeRatio << "\n";
    std::cout << "Sortino Ratio: " << sortinoRatio << "\n";
    std::cout << "------------------------------------\n";
    std::cout << "Final Cash: $" << cash << "\n";
    std::cout << "Total Trades: " << orderHistory.size() << "\n";
    std::cout << "====================================\n";
}