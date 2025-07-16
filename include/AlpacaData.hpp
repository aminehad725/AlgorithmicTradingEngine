#pragma once

#include <string>
#include "market_data.hpp"

class AlpacaDataHandler {
public:
    AlpacaDataHandler(const std::string& apiKey, const std::string& secretKey);
    PriceData getLatestBar(const std::string& symbol);

private:
    std::string apiKey;
    std::string secretKey;
    std::string dataApiUrl = "https://data.alpaca.markets";
};