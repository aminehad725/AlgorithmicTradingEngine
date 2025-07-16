CXX=g++
CXXFLAGS=-std=c++17 -Wall -Iinclude
SRC=src/main.cpp src/bollinger_band_strategy.cpp src/market_data.cpp src/order.cpp src/portfolio.cpp src/sma_crossover_strategy.cpp
OUT=build/trading_engine

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f build/*
