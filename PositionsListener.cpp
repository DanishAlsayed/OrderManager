//
// Created by Danish Al-sayed on 15-Mar-20.
//

#include <sstream>
#include "PositionsListener.h"

PositionsListener::PositionsListener() : orderBook(OrderBookPositions()), netFilledQuantity(0) {}


void PositionsListener::OnOrderFilled(int id, int quantityFilled) {
    bool sideBuy = false;
    if (orderBook.fill(id, quantityFilled, sideBuy) < 0) {
        std::ostringstream ss;
        ss << id;
        throwException("OnOrderFilled failed for order id " + ss.str());
        return;
    }
    int multiplier = sideBuy ? 1 : -1;
    netFilledQuantity += quantityFilled * multiplier;
}

void PositionsListener::OnRequestRejected(int id) {
    if (!orderBook.reject(id)) {
        std::ostringstream ss;
        ss << id;
        throwException("OnRequestRejected failed for order id " + ss.str());
    }
}

void PositionsListener::OnRequestAcknowledged(int id) {
    if (!orderBook.ackOrder(id)) {
        std::ostringstream ss;
        ss << id;
        throwException("OnRequestAcknowledged failed for order id " + ss.str());
    }
}

void PositionsListener::OnInsertOrderRequest(int id, char side, double price, int quantity) {
    if (!orderBook.emplace(Order(id, price, quantity, side))) {
        std::ostringstream ss;
        ss << id;
        throwException("OnInsertOrderRequest failed for order id " + ss.str());
    }
}

void PositionsListener::OnReplaceOrderRequest(int oldId, int newId, int deltaQuantity) {
    if (!orderBook.emplaceReplaceCandidate(OrderReplacement(oldId, newId, deltaQuantity))) {
        std::ostringstream ss;
        ss << oldId;
        throwException("OnReplaceOrderRequest failed for order id " + ss.str());
    }
}

void PositionsListener::throwException(const std::string &message) {
    throw std::runtime_error(message);
}

int PositionsListener::getNetFilledQuantity() const {
    return netFilledQuantity;
}

double PositionsListener::getBuyConfirmedVal() const {
    return orderBook.buyConfirmedVal;
}

double PositionsListener::getSellConfirmedVal() const {
    return orderBook.sellConfirmedVal;
}

double PositionsListener::getMaxBuyPendingVal() const {
    return orderBook.maxBuyPendingVal;
}

double PositionsListener::getMinBuyPendingVal() const {
    return orderBook.minBuyPendingVal;
}

double PositionsListener::getMinSellPendingVal() const {
    return orderBook.minSellPendingVal;
}

double PositionsListener::getMaxSellPendingVal() const {
    return orderBook.maxSellPendingVal;
}
