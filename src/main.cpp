#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>

#include "../include/market_data.hpp"
#include "../include/bollinger_band_strategy.hpp"
#include "../include/BacktestPortfolio.hpp"
#include "../include/AlpacaData.hpp"
#include "../include/LivePortfolio.hpp"
#include "../include/order.hpp"


void runBacktest() {
    MarketDataLoader loader("data/AAPL.csv");
    auto data = loader.loadData();

    BollingerBandStrategy strategy(20, 2.0);
    BacktestPortfolio portfolio(100000, 1.00);

    for (const auto& dataPoint : data) {
        double cash = portfolio.getCash();
        int shares = portfolio.getShares(dataPoint.symbol);

        OrderType signal = strategy.generateSignal(dataPoint, cash, shares);

        if (signal != OrderType::HOLD) {
            Order order(dataPoint.date, dataPoint.symbol, signal, 100, dataPoint.close);
            portfolio.executeOrder(order);
        }
        std::map<std::string, PriceData> latestPrices;
        latestPrices[dataPoint.symbol] = dataPoint;
        portfolio.updateMarketValue(latestPrices);
    }
    portfolio.printPerformanceReport();
}


void runLive() {
    std::string apiKey;
    std::string secretKey;

    try {
        std::ifstream configFile("config.json");
        nlohmann::json config = nlohmann::json::parse(configFile);
        apiKey = config.at("alpaca_api_key");
        secretKey = config.at("alpaca_secret_key");
    } catch (const std::exception& e) {
        std::cerr << "Error reading config.json: " << e.what() << std::endl;
        return;
    }

    AlpacaDataHandler dataHandler(apiKey, secretKey);
    LivePortfolio portfolio(apiKey, secretKey);
    BollingerBandStrategy strategy(20, 2.0);
    std::string symbol = "AAPL";

    portfolio.printAccountSummary();

    while (true) {
        PriceData latestBar = dataHandler.getLatestBar(symbol);

        if (latestBar.volume > 0) {
            std::cout << "Fetched latest bar for " << symbol << ". Close: " << latestBar.close << std::endl;
            
            double cash = portfolio.getCash();
            int shares = portfolio.getShares(symbol);

            OrderType signal = strategy.generateSignal(latestBar, cash, shares);

            if (signal != OrderType::HOLD) {
                Order order(latestBar.date, symbol, signal, 10, latestBar.close);
                portfolio.executeOrder(order);
                portfolio.printAccountSummary();
            }
        }
        
        std::cout << "Waiting 60 seconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <mode>" << std::endl;
        std::cerr << "Modes: backtest, live" << std::endl;
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "backtest") {
        std::cout << "--- Running in Backtest Mode ---" << std::endl;
        runBacktest();
    } else if (mode == "live") {
        std::cout << "--- Running in Live Trading Mode ---" << std::endl;
        runLive();
    } else {
        std::cerr << "Invalid mode specified. Use 'backtest' or 'live'." << std::endl;
        return 1;
    }

    return 0;
}