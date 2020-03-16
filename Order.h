//
// Created by Danish Al-sayed on 14-Mar-20.
//

#ifndef ORDER_H
#define ORDER_H


class Order {
    int id;
    int quantity;
    double price;
    bool buy;
    bool acked;

    static void ensurePositive(int i, double px, int qty);

public:
    Order(int id, double price, int quantity, char side);

    Order(int id, double price, int quantity, bool sideBuy, bool ack);

    int getId() const;

    double getPrice() const;

    int getQuantity() const;

    bool sideBuy() const;

    bool setQuantity(int quantity);

    void ack();

    bool isAcked() const;

    void setId(int id);
};


#endif //ORDER_H
