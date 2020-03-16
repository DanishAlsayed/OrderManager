//
// Created by Danish Al-sayed on 14-Mar-20.
//

#ifndef ECLIPSETEST_ORDERBOOK_H
#define ECLIPSETEST_ORDERBOOK_H


#include <unordered_map>
#include "Order.h"
#include "OrderReplacement.h"
#include "OrderBookInterface.h"

class OrderBook : public OrderBookInterface {
    std::unordered_map<int, Order> book;
    std::unordered_map<int, OrderReplacement> replaceRequests;
    std::unordered_map<int, int> orderToReplacement;

    bool replace(const std::unordered_map<int, OrderReplacement>::iterator &it);

public:

    double buyConfirmedVal;

    double sellConfirmedVal;

    double buyPendingVal;

    double sellPendingVal;

    OrderBook();

    std::unordered_map<int, Order>::const_iterator getOrderIterator(int id, bool &found);

    bool emplace(const Order &order) override;

    bool emplaceReplaceCandidate(const OrderReplacement &replacement) override;

    bool ackOrder(int id) override;

    bool reject(int id) override;

    //return remaining quantity, negative value in case of error
    int fill(int id, int quantityFilled, bool &side) override;

};


#endif //ECLIPSETEST_ORDERBOOK_H
