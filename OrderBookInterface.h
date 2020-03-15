//
// Created by Danish Al-sayed on 15-Mar-20.
//

#ifndef ECLIPSETEST_ORDERBOOKINTERFACE_H
#define ECLIPSETEST_ORDERBOOKINTERFACE_H

#include "OrderReplacement.h"
#include "Order.h"

class OrderBookInterface{
public:
    virtual bool emplace(const Order &order) = 0;

    virtual bool emplaceReplaceCandidate(const OrderReplacement &replacement) = 0;

    virtual bool ackOrder(int id) = 0;

    virtual bool reject(int id) = 0;

    //return remaining quantity, negative value in case of error
    virtual int fill(int id, int quantityFilled, bool &side) = 0;
};
#endif //ECLIPSETEST_ORDERBOOKINTERFACE_H
