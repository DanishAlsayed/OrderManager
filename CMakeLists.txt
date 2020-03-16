cmake_minimum_required(VERSION 3.14)
project(OrderManager)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14")

add_executable(OrderManager main.cpp Listener.h OrderBook.cpp OrderBook.h Order.cpp Order.h OrderReplacement.cpp OrderReplacement.h RequestRateListener.cpp RequestRateListener.h PositionsListener.cpp PositionsListener.h OrderBookInterface.h OrderBookPositions.cpp OrderBookPositions.h main.cpp)