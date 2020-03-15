//
// Created by Danish Al-sayed on 14-Mar-20.
//

#include <stdexcept>
#include "Order.h"

//TODO: how to catch this exception so module does not crash? Surround by named constructor?
Order::Order(const int id, const double price, const int quantity, const char side) : id(id), price(price),
                                                                                      quantity(quantity),
                                                                                      acked(false) {
    ensurePositive(id, price, quantity);
    if (side == 'O') {
        buy = false;
    } else if (side == 'B') {
        buy = true;
    } else {
        throw std::invalid_argument(R"(Order side can only be 'O' or 'B')");
    }
}

void Order::ensurePositive(int i, double px, int qty) {
    if (i < 0 || px < 0.0 || qty < 0) {
        throw std::invalid_argument("Order id, price and quantity cannot be less than 0");
    }
}

Order::Order(int id, double price, int quantity, bool sideBuy, bool ack) : id(id), price(price),
                                                                 quantity(quantity), buy(sideBuy),acked(ack) {
    ensurePositive(id, price, quantity);
}

//TODO: const correctness? Do I need to return const type?
int Order::getId() const {
    return id;
}

double Order::getPrice() const {
    return price;
}

void Order::setId(int newId) {
    id = newId;
}

int Order::getQuantity() const {
    return quantity;
}

bool Order::sideBuy() const {
    return buy;
}

bool Order::setQuantity(int qty) {
    if (qty > 0) {
        quantity = qty;
        return true;
    }
    return false;
}

void Order::ack() {
    acked = true;
}

bool Order::isAcked() const {
    return acked;
}
