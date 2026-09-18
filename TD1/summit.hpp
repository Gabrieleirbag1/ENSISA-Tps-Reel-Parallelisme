#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Summit {
    public:
        Summit();
        
        int getNumber() const;
        void setNumber(int _number);
        vector<Summit> getSummits() const;
        void setSummits(vector<Summit> _summits);

    private:
        int number;
        vector<Summit> summits;

};

#include "summit.tpp"

#endif