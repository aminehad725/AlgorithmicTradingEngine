#pragma once
#include <string>
#include <vector>

struct PriceData {
    std::string date;
    std::string symbol;
    double open, high, low, close, volume;
};

class MarketDataLoader {
public:
    MarketDataLoader(const std::string& filepath);
    std::vector<PriceData> loadData();
private:
    std::string filepath;
};
