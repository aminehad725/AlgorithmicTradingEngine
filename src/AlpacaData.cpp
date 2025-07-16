#include "../include/AlpacaData.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

AlpacaDataHandler::AlpacaDataHandler(const std::string& key, const std::string& secret)
    : apiKey(key), secretKey(secret) {}

PriceData AlpacaDataHandler::getLatestBar(const std::string& symbol) {
    PriceData bar = {};
    bar.symbol = symbol;

    cpr::Response r = cpr::Get(cpr::Url{dataApiUrl + "/v2/stocks/" + symbol + "/bars/latest"},
                               cpr::Header{{"APCA-API-KEY-ID", apiKey},
                                           {"APCA-API-SECRET-KEY", secretKey}});

    if (r.status_code == 200) {
        try {
            nlohmann::json response = nlohmann::json::parse(r.text);
            if (response.contains("bar")) {
                auto barData = response["bar"];
                bar.date = barData["t"];
                bar.open = barData["o"];
                bar.high = barData["h"];
                bar.low = barData["l"];
                bar.close = barData["c"];
                bar.volume = barData["v"];
            }
        } catch (const nlohmann::json::parse_error& e) {
            
        }
    }
    return bar;
}