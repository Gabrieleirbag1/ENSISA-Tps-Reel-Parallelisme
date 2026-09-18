#include "summit.hpp"
#include <vector>

Summit::Summit() {}

int Summit::getNumber() const {
    return number;
}

void Summit::setNumber(int _number) {
    number = _number;
}

vector<Summit> Summit::getSummits() const {
    return summits;
}

void Summit::setSummits(vector<Summit> _summits) {
    summits = _summits;
}
