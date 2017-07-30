#include "binarization.h"

#include "opencv2/highgui/highgui.hpp"

//  CV_8U  = uchar
//  CV_8S  = schar
//  CV_16U = ushort
//  CV_16S = short
//  CV_32S = int
//  CV_32F = float
//  CV_64F = double

double calcLocalStats(Mat &im, Mat &map_m, Mat &map_s, int winX, int winY) {

    // *************************************************************
    // glide a window across the image and
    // create two maps: mean and standard deviation.
    //
    // Version patched by Thibault Yohan (using opencv integral images)
    // *************************************************************

    Mat im_sum, im_sum_sq;
    cv::integral(im, im_sum, im_sum_sq, CV_64F);

    double m, s, max_s, sum, sum_sq;
    int wxh = winX / 2;
    int wyh = winY / 2;
    int x_firstth = wxh;
    int y_lastth = im.rows - wyh - 1;
    int y_firstth = wyh;
    double winarea = winX*winY;

    max_s = 0;
    for (int j = y_firstth; j <= y_lastth; j++){
        sum = sum_sq = 0;

        sum = im_sum.at<double>(j - wyh + winY, winX) - im_sum.at<double>(j - wyh, winX) - im_sum.at<double>(j - wyh + winY, 0) + im_sum.at<double>(j - wyh, 0);
        sum_sq = im_sum_sq.at<double>(j - wyh + winY, winX) - im_sum_sq.at<double>(j - wyh, winX) - im_sum_sq.at<double>(j - wyh + winY, 0) + im_sum_sq.at<double>(j - wyh, 0);

        m = sum / winarea;
        s = sqrt((sum_sq - m*sum) / winarea);
        if (s > max_s) max_s = s;

        map_m.fset(x_firstth, j, m);
        map_s.fset(x_firstth, j, s);

        // Shift the window, add and remove	new/old values to the histogram
        for (int i = 1; i <= im.cols - winX; i++) {

            // Remove the left old column and add the right new column
            sum -= im_sum.at<double>(j - wyh + winY, i) - im_sum.at<double>(j - wyh, i) - im_sum.at<double>(j - wyh + winY, i - 1) + im_sum.at<double>(j - wyh, i - 1);
            sum += im_sum.at<double>(j - wyh + winY, i + winX) - im_sum.at<double>(j - wyh, i + winX) - im_sum.at<double>(j - wyh + winY, i + winX - 1) + im_sum.at<double>(j - wyh, i + winX - 1);

            sum_sq -= im_sum_sq.at<double>(j - wyh + winY, i) - im_sum_sq.at<double>(j - wyh, i) - im_sum_sq.at<double>(j - wyh + winY, i - 1) + im_sum_sq.at<double>(j - wyh, i - 1);
            sum_sq += im_sum_sq.at<double>(j - wyh + winY, i + winX) - im_sum_sq.at<double>(j - wyh, i + winX) - im_sum_sq.at<double>(j - wyh + winY, i + winX - 1) + im_sum_sq.at<double>(j - wyh, i + winX - 1);

            m = sum / winarea;
            s = sqrt((sum_sq - m*sum) / winarea);
            if (s > max_s) max_s = s;

            map_m.fset(i + wxh, j, m);
            map_s.fset(i + wxh, j, s);
        }
    }

    return max_s;
}

void NiblackSauvolaWolfJolion(Mat inputMat, Mat outputMat, enumBinarization method, int winX, int winY, double k, double dR) {

    // Christian Wolf, Jean-Michel Jolion and Francoise Chassaing.
    // Text Localization, Enhancement and Binarization in Multimedia Documents.
    // International Conference on Pattern Recognition (ICPR),
    // volume 4, pages 1037-1040, 2002.

    double m, s, max_s;
    double th = 0;
    double min_I, max_I;
    int wxh = winX / 2;
    int wyh = winY / 2;
    int x_firstth = wxh;
    int x_lastth = inputMat.cols - wxh - 1;
    int y_lastth = inputMat.rows - wyh - 1;
    int y_firstth = wyh;

    // Create local statistics and store them in a double matrices
    Mat map_m = Mat::zeros(inputMat.rows, inputMat.cols, CV_32F);
    Mat map_s = Mat::zeros(inputMat.rows, inputMat.cols, CV_32F);
    max_s = calcLocalStats(inputMat, map_m, map_s, winX, winY);

    minMaxLoc(inputMat, &min_I, &max_I);

    Mat thsurf(inputMat.rows, inputMat.cols, CV_32F);

    // Create the threshold surface, including border processing
    // ----------------------------------------------------

    for (int j = y_firstth; j <= y_lastth; j++) {

        // NORMAL, NON-BORDER AREA IN THE MIDDLE OF THE WINDOW:
        for (int i = 0; i <= inputMat.cols - winX; i++) {

            m = map_m.fget(i + wxh, j);
            s = map_s.fget(i + wxh, j);

            // Calculate the threshold
            switch (method) {

                case binarization_NIBLACK:
                    th = m + k*s;
                    break;

                case binarization_SAUVOLA:
                    th = m * (1 + k*(s / dR - 1));
                    break;

                case binarization_WOLFJOLION:
                    th = m + k * (s / max_s - 1) * (m - min_I);
                    break;
            }

            thsurf.fset(i + wxh, j, th);

            if (i == 0) {
                // LEFT BORDER
                for (int i = 0; i <= x_firstth; ++i)
                    thsurf.fset(i, j, th);

                // LEFT-UPPER CORNER
                if (j == y_firstth)
                    for (int u = 0; u<y_firstth; ++u)
                        for (int i = 0; i <= x_firstth; ++i)
                            thsurf.fset(i, u, th);

                // LEFT-LOWER CORNER
                if (j == y_lastth)
                    for (int u = y_lastth + 1; u<inputMat.rows; ++u)
                        for (int i = 0; i <= x_firstth; ++i)
                            thsurf.fset(i, u, th);
            }

            // UPPER BORDER
            if (j == y_firstth)
                for (int u = 0; u<y_firstth; ++u)
                    thsurf.fset(i + wxh, u, th);

            // LOWER BORDER
            if (j == y_lastth)
                for (int u = y_lastth + 1; u<inputMat.rows; ++u)
                    thsurf.fset(i + wxh, u, th);
        }

        // RIGHT BORDER
        for (int i = x_lastth; i<inputMat.cols; ++i)
            thsurf.fset(i, j, th);

        // RIGHT-UPPER CORNER
        if (j == y_firstth)
            for (int u = 0; u<y_firstth; ++u)
                for (int i = x_lastth; i<inputMat.cols; ++i)
                    thsurf.fset(i, u, th);

        // RIGHT-LOWER CORNER
        if (j == y_lastth)
            for (int u = y_lastth + 1; u<inputMat.rows; ++u)
                for (int i = x_lastth; i<inputMat.cols; ++i)
                    thsurf.fset(i, u, th);
    }

    for (int y = 0; y<inputMat.rows; ++y)
        for (int x = 0; x<inputMat.cols; ++x)
        {
            if (inputMat.uget(x, y) >= thsurf.fget(x, y))
            {
                outputMat.uset(x, y, 255);
            }
            else
            {
                outputMat.uset(x, y, 0);
            }
        }
}

void bradley_Binarization(Mat inputMat, Mat outputMat, float T){

    int S = (inputMat.cols / 8);
    float Z = (1.0f - T);
    int x, y;
    long sum = 0;
    int count = 0;
    int index;
    int x1, y1, x2, y2;
    int s2 = S / 2;

    Mat resultMat = Mat(inputMat.size(), CV_8U);
    Mat integralMat = Mat(inputMat.size(), CV_32F);

    auto * input = (uchar *) inputMat.data;
    auto * output = (uchar *) resultMat.data;
    auto * integral = (float *) integralMat.data;

    //Creates the integral image
    for (x = 0; x < inputMat.cols; x++)
    {
        // reset this column sum
        sum = 0;

        for (y = 0; y < inputMat.rows; y++)
        {
            index = y * inputMat.cols + x;

            sum += input[index];

            if (y == 0)
                integral[index] = sum;
            else
                integral[index] = integral[index - 1] + sum;
        }
    }

    //perform thresholding
    for (x = 0; x < inputMat.cols; x++)
    {
        for (y = 0; y < inputMat.rows; y++)
        {
            index = y * inputMat.cols + x;

            //set the SxS region
            x1 = x - s2; x2 = x + s2;
            y1 = y - s2; y2 = y + s2;

            //check the border
            if (x1 < 0) x1 = 0;
            if (x2 >= inputMat.cols) x2 = inputMat.cols - 1;
            if (y1 < 0) y1 = 0;
            if (y2 >= inputMat.rows) y2 = inputMat.rows - 1;

            count = (x2 - x1)*(y2 - y1);

            //I(x,y) = s(x2,y2) - s(x1,y2) - s(x2,y1) + s(x1,x1)
            sum = integral[y2 * inputMat.cols + x2] -
                  integral[y1 * inputMat.cols + x2] -
                  integral[y2 * inputMat.cols + x1] +
                  integral[y1 * inputMat.cols + x1];

            if ((long)(input[index] * count) < (long)(sum * Z))
                output[index] = 0;
            else
                output[index] = 255;
        }
    }

    //Retorna o Mat binário
    resultMat.copyTo(outputMat);
}

bool binarize(Mat sourceMat, Mat destMat, enumBinarization method){

    int winX = 0;
    int winY = 0;
    double K = 0;

    //Calculate the window size
    if (winX == 0 || winY == 0) {

        winY = (int) (2.0 * sourceMat.rows - 1) / 3;
        winX = (int) sourceMat.cols - 1 < winY ? sourceMat.cols - 1 : winY;

        //Limit the window size
        if (winX > 100)
            winX = winY = 40;
    }

    switch (method) {

        case binarization_TRESHOLD:{
            threshold(sourceMat, destMat, 127, 255, THRESH_BINARY);
        }
        break;

        case binarization_MEAN:{
            adaptiveThreshold(sourceMat, destMat, 255, ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,11,2);
        }
        break;

        case binarization_GAUSSIAN:{
            adaptiveThreshold(sourceMat, destMat, 255, ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,11,2);
        }
        break;

        case binarization_CLAHE:
        {
            K = 0.17;
            Ptr<CLAHE> clahe = createCLAHE();
            clahe->setClipLimit(3);
            clahe->apply(sourceMat, destMat);
        }
        break;

        case binarization_NIBLACK:
            K = 0.2;
            NiblackSauvolaWolfJolion(~sourceMat, destMat, binarization_NIBLACK, winX, winY, K, 128);
            destMat = ~destMat;
            break;

        case binarization_SAUVOLA:
            K = 0.5;
            NiblackSauvolaWolfJolion(sourceMat, destMat, binarization_SAUVOLA, winX, winY, K, 128);
            break;

        case binarization_WOLFJOLION:
            K = 0.5;
            NiblackSauvolaWolfJolion(sourceMat, destMat, binarization_WOLFJOLION, winX, winY, K, 128);
            break;

        case binarization_BRADLEY:
            K = 0.15;
            if (sourceMat.channels() == 1)
                bradley_Binarization(sourceMat, destMat, (float) K);
            break;
    }

    if(destMat.rows > 0 && destMat.cols > 0 && destMat.channels() == 1)
        return true;

    return false;

}

