#include <iostream>
#include <unistd.h>
#include "Order.h"
#include "OrderBook.h"
#include "RequestRateListener.h"
#include "PositionsListener.h"

//TODO: add everything in main for submission?
//TODO: INCLUDE GUARDS

/*
void testRequestListener() {
    RequestRate requestRate = RequestRate(5,2);
    requestRate.OnInsertOrderRequest(1, 'B', 100.1, 100);
    requestRate.OnInsertOrderRequest(2, 'B', 100.2, 101);
    requestRate.OnInsertOrderRequest(3, 'O', 100.3, 102);
    requestRate.OnInsertOrderRequest(4, 'B', 100.4, 103);
    try {
        requestRate.OnInsertOrderRequest(4, 'B', 100.5, 103);
    } catch (std::exception &e) {
        std::cout << e.what() << '\n';
    }

    requestRate.OnRequestAcknowledged(1);
    requestRate.OnRequestAcknowledged(2);
    requestRate.OnRequestAcknowledged(3);
    requestRate.OnRequestAcknowledged(4);
    requestRate.OnOrderFilled(1, 51);

    requestRate.OnReplaceOrderRequest(1, 5, -3);
    requestRate.OnRequestAcknowledged(5);

    requestRate.OnRequestRejected(3);
}
*/
void testRequestRate() {
    RequestRateListener requestRate = RequestRateListener(5, 7.0);
    //sleep(8);
    requestRate.OnInsertOrderRequest(1, 'B', 100.1, 100);
    requestRate.OnInsertOrderRequest(3, 'B', 100.1, 100);
    requestRate.OnInsertOrderRequest(5, 'B', 100.1, 100);
    std::cout << "Time till next request: " << requestRate.timeTillNextRequest() << std::endl;
    requestRate.OnInsertOrderRequest(6, 'B', 100.1, 100);
    //sleep(1);
    requestRate.OnInsertOrderRequest(7, 'B', 100.1, 100);
    //LAST REQUEST
    requestRate.OnReplaceOrderRequest(1, 2, 3);
    //requestRate.OnInsertOrderRequest(8, 'B', 100.1, 100);
    if (requestRate.requestRateExceeded()) {
        std::cout << "REQUEST RATE EXCEEDED: Time till next request: " << requestRate.timeTillNextRequest()
                  << std::endl;
    }
    //sleep(7);
    //requestRate.OnReplaceOrderRequest(1, 2, 3);
    //std::cout << "here"<<std::endl;
    //sleep(1);
    //requestRate.OnReplaceOrderRequest(1, 2, 3);
    /*requestRate.OnReplaceOrderRequest(1, 2, 3);
    requestRate.OnReplaceOrderRequest(1, 2, 3);*/
    std::cout << "Time till next request: " << requestRate.timeTillNextRequest() << std::endl;
}

void testBookCoherence() {
    RequestRateListener requestRate = RequestRateListener(100, 7.0);
    //requestRate.orderBook.testPrintAllMaps();
    requestRate.OnInsertOrderRequest(1, 'B', 1.1, 100);
    requestRate.OnRequestAcknowledged(1);
    requestRate.OnReplaceOrderRequest(1, 4, -3);
    //requestRate.orderBook.testPrintAllMaps();
    //requestRate.OnReplaceOrderRequest(2, 5, 3);
    //requestRate.orderBook.testPrintAllMaps();
    requestRate.OnRequestAcknowledged(1);
    requestRate.OnOrderFilled(1, 20);
    requestRate.OnRequestAcknowledged(4);
    requestRate.OnOrderFilled(4, 10);
    requestRate.OnOrderFilled(4, 67);
    //requestRate.orderBook.testPrintAllMaps();
    //requestRate.orderBook.testPrintAllMaps();
}

void testPositionsListener() {
    PositionsListener positionsListener = PositionsListener();
    positionsListener.OnInsertOrderRequest(1, 'B', 10.0, 10);
    std::cout << positionsListener.getMinBuyPendingVal() << "\n";
    std::cout << positionsListener.getMaxBuyPendingVal() << "\n";
    std::cout << positionsListener.getMinSellPendingVal() << "\n";
    std::cout << positionsListener.getMaxSellPendingVal() << "\n";
    positionsListener.OnRequestAcknowledged(1);
    std::cout << "=============" << "\n";
    std::cout << positionsListener.getMinBuyPendingVal() << "\n";
    std::cout << positionsListener.getMaxBuyPendingVal() << "\n";
    std::cout << positionsListener.getMinSellPendingVal() << "\n";
    std::cout << positionsListener.getMaxSellPendingVal() << "\n";
    //positionsListener.OnOrderFilled(1, 10);
    positionsListener.OnInsertOrderRequest(2, 'O', 15.0, 25);
    std::cout << "=============" << "\n";
    std::cout << positionsListener.getMinBuyPendingVal() << "\n";
    std::cout << positionsListener.getMaxBuyPendingVal() << "\n";
    std::cout << positionsListener.getMinSellPendingVal() << "\n";
    std::cout << positionsListener.getMaxSellPendingVal() << "\n";
    positionsListener.OnRequestAcknowledged(2);
    std::cout << "=============" << "\n";
    std::cout << positionsListener.getMinBuyPendingVal() << "\n";
    std::cout << positionsListener.getMaxBuyPendingVal() << "\n";
    std::cout << positionsListener.getMinSellPendingVal() << "\n";
    std::cout << positionsListener.getMaxSellPendingVal() << "\n";
    positionsListener.OnOrderFilled(1, 5);
    std::cout << "=============" << "\n";
    std::cout << positionsListener.getMinBuyPendingVal() << "\n";
    std::cout << positionsListener.getMaxBuyPendingVal() << "\n";
    std::cout << positionsListener.getMinSellPendingVal() << "\n";
    std::cout << positionsListener.getMaxSellPendingVal() << "\n";
    positionsListener.OnOrderFilled(1, 5);
    std::cout << "=============" << "\n";
    std::cout << positionsListener.getMinBuyPendingVal() << "\n";
    std::cout << positionsListener.getMaxBuyPendingVal() << "\n";
    std::cout << positionsListener.getMinSellPendingVal() << "\n";
    std::cout << positionsListener.getMaxSellPendingVal() << "\n";
    positionsListener.OnReplaceOrderRequest(2, 3, 10);
    std::cout << "=============" << "\n";
    std::cout << positionsListener.getMinBuyPendingVal() << "\n";
    std::cout << positionsListener.getMaxBuyPendingVal() << "\n";
    std::cout << positionsListener.getMinSellPendingVal() << "\n";
    std::cout << positionsListener.getMaxSellPendingVal() << "\n";
    positionsListener.OnOrderFilled(2, 25);
    std::cout << "=============" << "\n";
    std::cout << positionsListener.getMinBuyPendingVal() << "\n";
    std::cout << positionsListener.getMaxBuyPendingVal() << "\n";
    std::cout << positionsListener.getMinSellPendingVal() << "\n";
    std::cout << positionsListener.getMaxSellPendingVal() << "\n";
    positionsListener.OnRequestRejected(3);
    std::cout << "=============" << "\n";
    std::cout << positionsListener.getMinBuyPendingVal() << "\n";
    std::cout << positionsListener.getMaxBuyPendingVal() << "\n";
    std::cout << positionsListener.getMinSellPendingVal() << "\n";
    std::cout << positionsListener.getMaxSellPendingVal() << "\n";
    //std::cout << positionsListener.getBuyConfirmedVal() << "\n";
    //std::cout << positionsListener.getSellConfirmedVal() << "\n";

}

int main() {
    std::cout << "Order Manager\n";
    //testBookCoherence();
    testRequestRate();
    testPositionsListener();
    return 0;
}