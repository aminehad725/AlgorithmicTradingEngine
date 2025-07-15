#include "../include/market_data.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

MarketDataLoader::MarketDataLoader(const std::string& filepath) : filepath(filepath) {}

std::string clean(const std::string& input) {
    std::string output = input;
    output.erase(remove(output.begin(), output.end(), '"'), output.end());
    output.erase(remove(output.begin(), output.end(), ','), output.end());
    return output;
}

std::vector<PriceData> MarketDataLoader::loadData() {
    std::vector<PriceData> data;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening data file: " << filepath << "\n";
        return data;
    }

    std::string line;
    std::getline(file, line); 

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        PriceData row;

        try {
            std::getline(ss, row.date, ',');

            std::getline(ss, token, ',');
            row.open = std::stod(clean(token));

            std::getline(ss, token, ',');
            row.high = std::stod(clean(token));

            std::getline(ss, token, ',');
            row.low = std::stod(clean(token));

            std::getline(ss, token, ',');
            row.close = std::stod(clean(token));

            std::getline(ss, token, ',');
            row.volume = std::stod(clean(token)); 

            row.symbol = "AAPL";
            data.push_back(row);
        } catch (const std::exception& e) {
            std::cerr << "Bad line: " << line << " (" << e.what() << ")\n";
        }
    }

    file.close();
    return data;
}