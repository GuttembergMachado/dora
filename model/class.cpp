//
// Created by gutto on 16/08/17.
//
#include "class.h"

Class::Class(string label) {

    //Initialize internal variables
    mLabel = label;
    samples.clear();
}

string Class::getLabel() {
    return mLabel;
}

long Class::getAverageSampleWidth() {

    if(samples.size() == 0)
        return 0;

    int sum = 0;
    for (int i = 0; i < samples.size(); i++) {
        sum = sum + samples[i].getWidth();
    }

    return sum / samples.size();

}

long Class::getAverageSampleHeight(){

    if(samples.size() == 0)
        return 0;

    int sum = 0;
    for (int i = 0; i < samples.size(); i++) {
        sum = sum + samples[i].getHeight();
    }

    return sum / samples.size();
}
