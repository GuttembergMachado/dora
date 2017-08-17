//
// Created by gutto on 16/08/17.
//

#ifndef CLASS_H
#define CLASS_H

#include "../tools/helper.h"
#include "sample.h"

using namespace std;

class Class{
    string mLabel;
public:
    Class(string label);
    vector<Sample> samples;
    string getLabel();
    long getAverageSampleWidth();
    long getAverageSampleHeight();
};

#endif
