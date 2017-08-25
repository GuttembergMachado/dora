//
// Created by gutto on 16/08/17.
//
#include "class.h"

//Class::Class(string label) {
//
//    //Initialize internal variables
//    mLabel = label;
//    samples.clear();
//    mAverageSampleWidth;
//    mAverageSampleHeight;
//
//}

string Class::getLabel() {
    return mLabel;
}

void Class::setLabel(string label){
    mLabel = label;
}

void Class::calculateAverageSampleWidth(){

    if(samples.size() > 0){

        int sum = 0;
        for (int i = 0; i < samples.size(); i++) {
            sum = sum + samples[i].originalMat.cols;
        }

        mAverageSampleWidth =  sum / (int) samples.size();
    }

}

void Class::calculateAverageSampleHeight(){

    if(samples.size() > 0){

        int sum = 0;
        for (int i = 0; i < samples.size(); i++) {
            sum = sum + samples[i].originalMat.rows;
        }

        mAverageSampleHeight = sum / (int) samples.size();
    }

}

int Class::getAverageSampleWidth() {
    return mAverageSampleWidth;
}

int Class::getAverageSampleHeight(){
    return mAverageSampleHeight;
}
