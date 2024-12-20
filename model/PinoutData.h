#ifndef PINOUTDATA_H
#define PINOUTDATA_H

#include <vector>
#include <string>
#include "Pin.h"

class PinoutData {
public:
    virtual ~PinoutData() = default;

    virtual void addPin(const Pin& pin) = 0;
    virtual std::vector<Pin>::iterator begin() = 0;
    virtual std::vector<Pin>::iterator end() = 0;
    virtual std::vector<Pin>::const_iterator begin() const = 0;
    virtual std::vector<Pin>::const_iterator end() const = 0;

    virtual std::string toString() const = 0;
};

#endif // PINOUTDATA_H
