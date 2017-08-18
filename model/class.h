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
    int mAverageSampleWidth = 0;
    int mAverageSampleHeight = 0;
public:
    Class(string label);
    vector<Sample> samples;
    string getLabel();
    int getAverageSampleWidth();
    int getAverageSampleHeight();
    void calculateAverageSampleWidth();
    void calculateAverageSampleHeight();
};

#endif
