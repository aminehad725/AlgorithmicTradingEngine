#include "../include/LivePortfolio.hpp"
#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

LivePortfolio::LivePortfolio(const std::string& key, const std::string& secret)
    : apiKey(key), secretKey(secret) {}

void LivePortfolio::executeOrder(const Order& order) {
    nlohmann::json orderData = {
        {"symbol", order.symbol},
        {"qty", std::to_string(order.quantity)},
        {"side", (order.type == OrderType::BUY ? "buy" : "sell")},
        {"type", "market"},
        {"time_in_force", "day"}
    };

    cpr::Response r = cpr::Post(cpr::Url{tradingApiUrl + "/v2/orders"},
                                cpr::Header{{"APCA-API-KEY-ID", apiKey},
                                            {"APCA-API-SECRET-KEY", secretKey}},
                                cpr::Body{orderData.dump()},
                                cpr::Header{{"Content-Type", "application/json"}});

    if (r.status_code == 200) {
        std::cout << "Order placed successfully: " << r.text << std::endl;
    } else {
        std::cout << "Order failed: " << r.status_code << " - " << r.text << std::endl;
    }
}

double LivePortfolio::getCash() const {
    return 100000.0; 
}

int LivePortfolio::getShares(const std::string& symbol) const {
    return 0;
}

void LivePortfolio::printAccountSummary() {
    cpr::Response r = cpr::Get(cpr::Url{tradingApiUrl + "/v2/account"},
                               cpr::Header{{"APCA-API-KEY-ID", apiKey},
                                           {"APCA-API-SECRET-KEY", secretKey}});
    std::cout << "\n--- Alpaca Account Summary ---\n" << r.text << std::endl;
}