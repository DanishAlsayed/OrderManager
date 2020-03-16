//
// Created by Danish Al-sayed on 14-Mar-20.
//

#ifndef ORDERREPLACEMENT_H
#define ORDERREPLACEMENT_H


class OrderReplacement {
    int oldId, newId, deltaQuantity;
public:
    OrderReplacement(int oldId, int newId, int deltaQuantity);

    int getOldId() const;

    int getNewId() const;

    int getDeltaQuantity() const;

};


#endif //ORDERREPLACEMENT_H
