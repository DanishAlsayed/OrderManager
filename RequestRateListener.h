//
// Created by Danish Al-sayed on 14-Mar-20.
//

#ifndef REQUESTRATELISTENER_H
#define REQUESTRATELISTENER_H


#include "Listener.h"
#include "OrderBook.h"
#include <time.h>

class RequestRateListener : public Listener {
    const int requests;
    const double seconds;
    int counter;
    clock_t firstTime, lastTime;
    bool rateExceeded;
    void updateRate(clock_t current);
    double timeDifference();
    void reset(clock_t current);
    double timeWindow();

    void throwException(const std::string &message);
public:

    OrderBook orderBook;
    RequestRateListener(int x, double y);
    bool requestRateExceeded();

    double timeTillNextRequest();

    void OnInsertOrderRequest(int id, char side, double price, int quantity) override;

    void OnReplaceOrderRequest(int oldId, int newId, int deltaQuantity) override;

    void OnRequestAcknowledged(int id) override;

    void OnRequestRejected(int id) override;

    void OnOrderFilled(int id, int quantityFilled) override;
};


#endif //REQUESTRATELISTENER_H
