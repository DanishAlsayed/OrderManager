//
// Created by Danish Al-sayed on 15-Mar-20.
//

#ifndef ORDERBOOKPOSITIONS_H
#define ORDERBOOKPOSITIONS_H


#include <unordered_map>
#include "OrderBookInterface.h"

class OrderBookPositions : public OrderBookInterface {
    std::unordered_map<int, Order> book;
    std::unordered_map<int, OrderReplacement> replaceRequests;
    std::unordered_map<int, int> orderToReplacement;

    bool replace(const std::unordered_map<int, OrderReplacement>::iterator &it);

    static double orderValue(double price, double qty);

    void updateConfirmedVal(double value, bool isBuy);

    void updatePendingOnReplaceRequest(double price, int deltaQty, bool sideBuy);

public:
    double buyConfirmedVal;

    double sellConfirmedVal;

    double maxBuyPendingVal;

    double minBuyPendingVal;

    double minSellPendingVal;

    double maxSellPendingVal;

    OrderBookPositions();

    bool emplace(const Order &order) override;

    bool emplaceReplaceCandidate(const OrderReplacement &replacement) override;

    bool ackOrder(int id) override;

    bool reject(int id) override;

    //return remaining quantity, negative value in case of error
    int fill(int id, int quantityFilled, bool &side) override;
};


#endif //ORDERBOOKPOSITIONS_H
