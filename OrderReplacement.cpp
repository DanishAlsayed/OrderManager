//
// Created by Danish Al-sayed on 14-Mar-20.
//

#include <stdexcept>
#include "OrderReplacement.h"

OrderReplacement::OrderReplacement(int oldId, int newId, int deltaQuantity) : oldId(oldId), newId(newId),
                                                                              deltaQuantity(deltaQuantity) {
    if (oldId < 0 || newId < 0) {
        throw std::invalid_argument("OrderReplacement oldId and newId cannot be less than 0");
    }
    if(deltaQuantity == 0){
        throw std::invalid_argument("deltaQuantity cannot be equal to 0");
    }
}

int OrderReplacement::getOldId() const {
    return oldId;
}

int OrderReplacement::getNewId() const {
    return newId;
}

int OrderReplacement::getDeltaQuantity() const {
    return deltaQuantity;
}
