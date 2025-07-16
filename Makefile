CXX=g++
CXXFLAGS=-std=c++17 -Wall -Iinclude
SRC=src/main.cpp src/bollinger_band_strategy.cpp src/market_data.cpp src/order.cpp src/BacktestPortfolio.cpp src/LivePortfolio.cpp src/sma_crossover_strategy.cpp src/AlpacaData.cpp
OUT=build/trading_engine
LIBS=-lcpr -lcurl

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f build/*