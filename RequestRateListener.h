//
// Created by Danish Al-sayed on 14-Mar-20.
//

#ifndef ECLIPSETEST_REQUESTRATELISTENER_H
#define ECLIPSETEST_REQUESTRATELISTENER_H


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
    //OrderBook orderBook;

    void throwException(const std::string &message);
public:
    //TODO: MOVE BACK TO PRIVATE, THIS IS ONLY FOR TESTING
    OrderBook orderBook;
    RequestRateListener(int x, double y);
    bool requestRateExceeded();

    //TODO: is double a good choice?
    double timeTillNextRequest();

    void OnInsertOrderRequest(int id, char side, double price, int quantity) override;

    void OnReplaceOrderRequest(int oldId, int newId, int deltaQuantity) override;

    void OnRequestAcknowledged(int id) override;

    void OnRequestRejected(int id) override;

    void OnOrderFilled(int id, int quantityFilled) override;
};


#endif //ECLIPSETEST_REQUESTRATELISTENER_H