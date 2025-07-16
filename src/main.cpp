#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include "../include/AlpacaData.hpp"

int main() {
    std::string apiKey;
    std::string secretKey;

    try {
        std::ifstream configFile("config.json");
        nlohmann::json config = nlohmann::json::parse(configFile);
        apiKey = config.at("alpaca_api_key");
        secretKey = config.at("alpaca_secret_key");
    } catch (const std::exception& e) {
        std::cerr << "Error reading config.json: " << e.what() << std::endl;
        return 1;
    }

    AlpacaDataHandler dataHandler(apiKey, secretKey);
    PriceData latestBar = dataHandler.getLatestBar("AAPL");

    if (latestBar.volume > 0) {
        std::cout << "Successfully fetched latest bar for AAPL:" << std::endl;
        std::cout << "Timestamp: " << latestBar.date << std::endl;
        std::cout << "Close: " << latestBar.close << std::endl;
        std::cout << "Volume: " << latestBar.volume << std::endl;
    } else {
        std::cout << "Failed to fetch latest bar for AAPL." << std::endl;
    }

    return 0;
}