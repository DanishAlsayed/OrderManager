//
// Created by Danish Al-sayed on 15-Mar-20.
//

#include "OrderBookPositions.h"

OrderBookPositions::OrderBookPositions() : book(std::unordered_map<int, Order>()),
                                           replaceRequests(std::unordered_map<int, OrderReplacement>()),
                                           orderToReplacement(std::unordered_map<int, int>()),
                                           minBuyPendingVal(0.0),
                                           minSellPendingVal(0.0),
                                           maxBuyPendingVal(0.0),
                                           maxSellPendingVal(0.0),
                                           buyConfirmedVal(0.0),
                                           sellConfirmedVal(0.0) {}

bool OrderBookPositions::replace(const std::unordered_map<int, OrderReplacement>::iterator &it) {
    auto oldLoc = book.find(it->second.getOldId());
    oldLoc->second.setId(it->second.getNewId());
    int newQty = oldLoc->second.getQuantity() + it->second.getDeltaQuantity();
    oldLoc->second.setQuantity(newQty);
    emplace(Order(it->second.getNewId(), oldLoc->second.getPrice(), newQty, oldLoc->second.sideBuy(),
                  oldLoc->second.isAcked()));
    book.erase(oldLoc);
    replaceRequests.erase(it);
    orderToReplacement.erase(it->second.getOldId());
    return true;
}


double OrderBookPositions::orderValue(double price, double qty) {
    return price * qty;
}

void OrderBookPositions::updateConfirmedVal(double value, bool isBuy) {
    isBuy ? (buyConfirmedVal += value) : (sellConfirmedVal += value);
}

bool OrderBookPositions::emplace(const Order &order) {
    bool result = book.emplace(order.getId(), order).second;
    if (result) {
        if (order.sideBuy()) {
            maxBuyPendingVal += orderValue(order.getPrice(), order.getQuantity());
        } else {
            maxSellPendingVal += orderValue(order.getPrice(), order.getQuantity());
        }
    }
    return result;
}

void OrderBookPositions::updatePendingOnReplaceRequest(double price, int deltaQty, bool sideBuy) {
    if (sideBuy) {
        if (deltaQty > 0) {
            maxBuyPendingVal += orderValue(price, deltaQty);
        } else {
            minBuyPendingVal += orderValue(price, deltaQty);
        }
    } else {
        if (deltaQty > 0) {
            maxSellPendingVal += orderValue(price, deltaQty);
        } else {
            minSellPendingVal += orderValue(price, deltaQty);
        }
    }
}

bool OrderBookPositions::emplaceReplaceCandidate(const OrderReplacement &replacement) {
    auto oldOrderIt = book.find(replacement.getOldId());
    if (oldOrderIt == book.end()) {
        return false;
    }
    //update with new replace if a pending replace already exists with the same oldId
    //Only keeping track of the latest replace request
    int deltaQuantity = replacement.getDeltaQuantity();
    if ((oldOrderIt->second.getQuantity() + deltaQuantity) <= 0) {
        return false;
    }
    bool result = replaceRequests.emplace(replacement.getNewId(), replacement).second;
    int oldId = replacement.getOldId();
    double price = oldOrderIt->second.getPrice();
    bool side = oldOrderIt->second.sideBuy();
    auto existing = orderToReplacement.find(oldId);
    if (existing != orderToReplacement.end() && result) {
        auto oldReplace = replaceRequests.find(existing->second);
        int delta = deltaQuantity - oldReplace->second.getDeltaQuantity();
        updatePendingOnReplaceRequest(price, delta, side);
        replaceRequests.erase(existing->second);
        existing->second = replacement.getNewId();
    } else if (result) {
        orderToReplacement.emplace(replacement.getOldId(), replacement.getNewId());
        updatePendingOnReplaceRequest(price, deltaQuantity, side);
    }

    return result;
}

bool OrderBookPositions::ackOrder(int id) {
    auto loc = book.find(id);
    if (loc != book.end()) {
        loc->second.ack();
        bool side = loc->second.sideBuy();
        if (side) {
            minBuyPendingVal += orderValue(loc->second.getPrice(), loc->second.getQuantity());
        } else {
            minSellPendingVal += orderValue(loc->second.getPrice(), loc->second.getQuantity());
        }

        updateConfirmedVal(orderValue(loc->second.getPrice(), loc->second.getQuantity()), loc->second.sideBuy());
        return true;
    }
    auto replaceLoc = replaceRequests.find(id);
    if (replaceLoc == replaceRequests.end()) {
        //No such Id exists
        return false;
    }

    if (loc != book.end()) {
        bool sideBuy = loc->second.sideBuy();
        int deltaQty = replaceLoc->second.getDeltaQuantity();
        double price = loc->second.getPrice();
        updateConfirmedVal(orderValue(price, deltaQty), sideBuy);
        double value = orderValue(price, deltaQty * -1);
        if (sideBuy) {
            if (deltaQty < 0) {
                maxBuyPendingVal += value;
            } else {
                minBuyPendingVal += value;
            }
        } else {
            if (deltaQty < 0) {
                maxSellPendingVal += value;
            } else {
                minSellPendingVal += value;
            }
        }

        return replace(replaceLoc);
    }

    return false;
}

bool OrderBookPositions::reject(int id) {
    auto oldOrderIt = book.find(id);
    if (oldOrderIt != book.end() && !oldOrderIt->second.isAcked()) {
        if (oldOrderIt->second.sideBuy()) {
            maxBuyPendingVal -= orderValue(oldOrderIt->second.getPrice(), oldOrderIt->second.getQuantity());
        }
        book.erase(oldOrderIt);
        return true;
    }
    auto replaceIt = replaceRequests.find(id);
    if (replaceIt != replaceRequests.end()) {
        int oldId = replaceIt->second.getOldId();
        oldOrderIt = book.find(oldId);
        if (oldOrderIt != book.end()) {
            bool sideBuy = oldOrderIt->second.sideBuy();
            int deltaQty = replaceIt->second.getDeltaQuantity();
            double value = orderValue(oldOrderIt->second.getPrice(), deltaQty * -1);
            if (sideBuy) {
                if (deltaQty < 0) {
                    minBuyPendingVal += value;
                } else {
                    maxBuyPendingVal += value;
                }
            } else {
                if (deltaQty < 0) {
                    minSellPendingVal += value;
                } else {
                    maxSellPendingVal += value;
                }
            }
            replaceRequests.erase(replaceIt);
            return true;
        }
    }
    return false;
}

int OrderBookPositions::fill(int id, int quantityFilled, bool &side) {
    if (quantityFilled < 0) {
        return -1;
    }

    auto loc = book.find(id);
    int newQty = -1;
    if (loc != book.end() && loc->second.isAcked()) {
        newQty = loc->second.getQuantity() - quantityFilled;
        double price = loc->second.getPrice();
        if (newQty < 0) {
            return false;
        }
        side = loc->second.sideBuy();
        if (side) {
            minBuyPendingVal -= orderValue(price, quantityFilled);
            maxBuyPendingVal -= orderValue(price, quantityFilled);
        } else {
            maxSellPendingVal -= orderValue(price, quantityFilled);
            minSellPendingVal -= orderValue(price, quantityFilled);
        }

        updateConfirmedVal(orderValue(price, quantityFilled * -1), side);
        if (newQty == 0) {
            book.erase(loc);
        } else if (newQty > 0) {
            loc->second.setQuantity(newQty);
        }
    }
    return newQty;
}
