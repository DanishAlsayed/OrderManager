//
// Created by Danish Al-sayed on 15-Mar-20.
//

#ifndef POSITIONSLISTENER_H
#define POSITIONSLISTENER_H


#include "OrderBook.h"
#include "Listener.h"
#include "OrderBookPositions.h"

class PositionsListener : public Listener {
private:

    OrderBookPositions orderBook;

    void throwException(const std::string &message);

    int netFilledQuantity;

public:
    double getBuyConfirmedVal() const;

    double getSellConfirmedVal() const;

    PositionsListener();

    int getNetFilledQuantity() const;

    void OnInsertOrderRequest(int id, char side, double price, int quantity) override;

    void OnReplaceOrderRequest(int oldId, int newId, int deltaQuantity) override;

    void OnRequestAcknowledged(int id) override;

    void OnRequestRejected(int id) override;

    void OnOrderFilled(int id, int quantityFilled) override;

    double getMaxSellPendingVal() const;

    double getMaxBuyPendingVal() const;

    double getMinSellPendingVal() const;

    double getMinBuyPendingVal() const;
};


#endif //POSITIONSLISTENER_H
