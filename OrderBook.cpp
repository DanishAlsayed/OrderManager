//
// Created by Danish Al-sayed on 14-Mar-20.
//

#include "OrderBook.h"

//TODO: change to initialization list
OrderBook::OrderBook() {
    book = std::unordered_map<int, Order>();
    replaceRequests = std::unordered_map<int, OrderReplacement>();
    orderToReplacement = std::unordered_map<int, int>();
}

//TODO: repeated ID?

bool OrderBook::emplace(const Order &order) {
    return book.emplace(order.getId(), order).second;
}

std::unordered_map<int, Order>::const_iterator OrderBook::getOrderIterator(int id, bool &found) {
    const std::unordered_map<int, Order>::const_iterator it = book.find(id);
    if (it != book.end()) {
        found = true;
        return it;
    }
    found = false;
    return it;
}

//TODO: move as a concrete function in the parent class?
bool OrderBook::replace(const std::unordered_map<int, OrderReplacement>::iterator &it) {
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

bool OrderBook::emplaceReplaceCandidate(const OrderReplacement &replacement) {
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
    auto existing = orderToReplacement.find(oldId);
    if (existing != orderToReplacement.end() && result) {
        replaceRequests.emplace(replacement.getNewId(), replacement);
        replaceRequests.erase(existing->second);
        existing->second = replacement.getNewId();
    } else if (result) {
        orderToReplacement.emplace(replacement.getOldId(), replacement.getNewId());
    }

    return result;
}

bool OrderBook::ackOrder(int id) {
    auto loc = book.find(id);
    if (loc != book.end()) {
        loc->second.ack();
        return true;
    }

    auto replaceLoc = replaceRequests.find(id);
    if (replaceLoc == replaceRequests.end()) {
        //No such Id exists
        return false;
    }
    loc = book.find(replaceLoc->second.getOldId());
    if (loc != book.end()) {
        return replace(replaceLoc);
    }
    return false;
}

bool OrderBook::reject(int id) {
    auto oldOrderIt = book.find(id);
    if (oldOrderIt != book.end() && !oldOrderIt->second.isAcked()) {
        book.erase(oldOrderIt);
        return true;
    }
    auto replaceIt = replaceRequests.find(id);
    if (replaceIt != replaceRequests.end()) {
        oldOrderIt = book.find(replaceIt->second.getOldId());
        if (oldOrderIt != book.end()) {
            replaceRequests.erase(replaceIt);
            return true;
        }
    }
    return false;
}

int OrderBook::fill(int id, int quantityFilled, bool &side) {
    if (quantityFilled < 0) {
        return -1;
    }
    auto loc = book.find(id);
    int newQty = -1;
    if (loc != book.end() && loc->second.isAcked()) {
        newQty = loc->second.getQuantity() - quantityFilled;
        if (newQty < 0) {
            return false;
        }
        side = loc->second.sideBuy();
        if (newQty == 0) {
            book.erase(loc);
        } else if (newQty > 0) {
            loc->second.setQuantity(newQty);
        }
    }
    return newQty;
}
