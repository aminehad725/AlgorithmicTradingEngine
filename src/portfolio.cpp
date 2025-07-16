#include "../include/portfolio.hpp"
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

Portfolio::Portfolio(double initialCash, double costPerTrade) 
    : cash(initialCash), 
      shares(0), 
      initialCapital(initialCash), 
      transactionCost(costPerTrade) {}

void Portfolio::executeOrder(const Order& order) {
    bool executed = false;
    if (order.type == OrderType::BUY && cash >= order.quantity * order.price) {
        cash -= order.quantity * order.price;
        cash -= transactionCost;
        shares += order.quantity;
        executed = true;
    } else if (order.type == OrderType::SELL && shares >= order.quantity) {
        cash += order.quantity * order.price;
        cash -= transactionCost;
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
    double totalValue = cash + (shares * data.close);
    historicalValues.push_back(totalValue);
}

void Portfolio::printPerformanceReport() {
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
    for (const auto r : returns) {
        stdDev += (r - meanReturn) * (r - meanReturn);
    }
    stdDev = std::sqrt(stdDev / returns.size());

    double downsideDev = 0.0;
    int negativeCount = 0;
    for (const auto r : returns) {
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
    std::cout << "Final Shares: " << shares << "\n";
    std::cout << "Total Trades: " << orderHistory.size() << "\n";
    std::cout << "====================================\n";
}