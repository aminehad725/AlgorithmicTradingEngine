#include "../include/LivePortfolio.hpp"
#include <iostream>
#include <string>
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
        std::cout << "Order successfully placed: " << r.text << std::endl;
    } else {
        std::cout << "Order failed: " << r.status_code << " - " << r.text << std::endl;
    }
}

double LivePortfolio::getCash() const {
    cpr::Response r = cpr::Get(cpr::Url{tradingApiUrl + "/v2/account"},
                               cpr::Header{{"APCA-API-KEY-ID", apiKey},
                                           {"APCA-API-SECRET-KEY", secretKey}});

    if (r.status_code == 200) {
        try {
            nlohmann::json account = nlohmann::json::parse(r.text);
            return std::stod(account["cash"].get<std::string>());
        } catch (const std::exception& e) {
            std::cerr << "Error parsing account cash: " << e.what() << std::endl;
        }
    }
    return 0.0;
}

int LivePortfolio::getShares(const std::string& symbol) const {
    cpr::Response r = cpr::Get(cpr::Url{tradingApiUrl + "/v2/positions/" + symbol},
                               cpr::Header{{"APCA-API-KEY-ID", apiKey},
                                           {"APCA-API-SECRET-KEY", secretKey}});

    if (r.status_code == 200) {
        try {
            nlohmann::json position = nlohmann::json::parse(r.text);
            return std::stoi(position["qty"].get<std::string>());
        } catch (const std::exception& e) {
        }
    }
    return 0;
}

void LivePortfolio::printAccountSummary() {
    cpr::Response r = cpr::Get(cpr::Url{tradingApiUrl + "/v2/account"},
                               cpr::Header{{"APCA-API-KEY-ID", apiKey},
                                           {"APCA-API-SECRET-KEY", secretKey}});
    if (r.status_code == 200) {
        std::cout << "\n--- Alpaca Account Summary ---\n" << r.text << std::endl;
    } else {
        std::cout << "\n--- Failed to fetch account summary ---" << std::endl;
    }
}