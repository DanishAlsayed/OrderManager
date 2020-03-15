//
// Created by Danish Al-sayed on 14-Mar-20.
//

#include <time.h>
#include <sstream>
#include "RequestRateListener.h"

RequestRateListener::RequestRateListener(int x, double y) : requests(x), seconds(y), counter(0), firstTime(clock()),
                                                            lastTime(clock()), rateExceeded(false),
                                                            orderBook(OrderBook()) {}

void RequestRateListener::OnOrderFilled(int id, int quantityFilled) {
    bool sideBuy = false;
    if (orderBook.fill(id, quantityFilled, sideBuy) < 0) {
        std::ostringstream ss;
        ss << id;
        throwException("OnOrderFilled failed for order id " + ss.str());
    }
}

void RequestRateListener::OnRequestRejected(int id) {
    if (!orderBook.reject(id)) {
        std::ostringstream ss;
        ss << id;
        throwException("OnRequestRejected failed for order id " + ss.str());
    }
}

void RequestRateListener::OnRequestAcknowledged(int id) {
    if (!orderBook.ackOrder(id)) {
        std::ostringstream ss;
        ss << id;
        throwException("OnRequestAcknowledged failed for order id " + ss.str());
    }
}

void RequestRateListener::OnInsertOrderRequest(int id, char side, double price, int quantity) {
    clock_t currentTime = clock();
    double currDiff = double((currentTime - lastTime)) / (double) CLOCKS_PER_SEC;
    double window = timeWindow();
    if (rateExceeded && currDiff <= window) {
        throw std::runtime_error("Request Rate exceeded, not adding new insert order request");
        return;
    }
    if (!orderBook.emplace(Order(id, price, quantity, side))) {
        std::ostringstream ss;
        ss << id;
        throwException("OnInsertOrderRequest failed for order id " + ss.str());
    }
    updateRate(currentTime);
}

void RequestRateListener::OnReplaceOrderRequest(int oldId, int newId, int deltaQuantity) {
    clock_t currentTime = clock();
    double currDiff = double((currentTime - lastTime)) / (double) CLOCKS_PER_SEC;
    double window = timeWindow();
    if (rateExceeded && currDiff <= window) {
        throw std::runtime_error("Request Rate exceeded, not adding new replace order request");
        return;
    }
    if (!orderBook.emplaceReplaceCandidate(OrderReplacement(oldId, newId, deltaQuantity))) {
        std::ostringstream ss;
        ss << oldId;
        throwException("OnReplaceOrderRequest failed for order id " + ss.str());
    }
    updateRate(currentTime);
}

void RequestRateListener::updateRate(clock_t current) {
    double currDiff = double((current - lastTime)) / (double) CLOCKS_PER_SEC;
    double window = timeWindow();

    counter++;
    if (currDiff <= window && counter <= requests) {
        lastTime = current;
        rateExceeded = false;
    } else if (currDiff >= window && counter <= requests) {
        counter = 1;
        reset(current);
    } else if (counter > requests && currDiff <= window) {
        rateExceeded = true;
    } else if (counter > requests && currDiff >= window) {
        reset(current);
    }
}


bool RequestRateListener::requestRateExceeded() {
    return rateExceeded;
}

double RequestRateListener::timeTillNextRequest() {
    if (rateExceeded) {
        return timeWindow();
    }
    return 0.0;
}

double RequestRateListener::timeDifference() {
    return double(lastTime - firstTime) / (double) CLOCKS_PER_SEC;
}

void RequestRateListener::reset(clock_t current) {
    lastTime = current;
    firstTime = current;
    rateExceeded = false;
}

double RequestRateListener::timeWindow() {
    return seconds - timeDifference();
}

void RequestRateListener::throwException(const std::string &message) {
    throw std::runtime_error(message);
}