/*
 * animaly_detection_util.cpp
 *
 * Author: Or Yaniv 205444805
 */

#include <cmath>
#include "anomaly_detection_util.h"

float avg(float* x, int size){
    float sum = 0;
    for (int i = 0; i < size; i++)
        sum += x[i];
    return sum / size;}

// returns the variance of X and Y
float var(float* x, int size){
    float average = avg(x, size);
    float var = 0;

    for (int i = 0; i < size; i++) {
        var += (x[i] - average) * (x[i] - average);
    }

    return (var / size);
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float avg_x = avg(x, size);
    float avg_y = avg(y, size);
    float cov = 0;
    for (int i = 0; i < size; i++) {
        cov += ((x[i] - avg_x) * (y[i] - avg_y)) / size;
    }
    return cov;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    float covariance  = cov(x, y, size);
    float var_x = sqrtf(var(x, size));
    float var_y = sqrtf(var(y, size));
    return covariance  / (var_x * var_y);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){

    int N = size;
    float *x = new float [size];
    float *y = new float [size];
    for (int i = 0; i < size; ++i) {
        x[i]=points[i]->x;
        y[i]=points[i]->y;
    }
    float a = cov(x,y,size)/var(x,size);
    float b = avg(y,size)-a*avg(x,size);
    return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    Line l = linear_reg(points,size);
    return dev(p,l);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    float line_y = l.a*p.x+l.b;
    return std::abs(line_y-p.y);
}




