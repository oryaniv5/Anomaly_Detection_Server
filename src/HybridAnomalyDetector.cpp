/*
 * HybridAnomalyDetector.cpp
 *
 * Author: Or Yaniv 205444805
 */



#include "HybridAnomalyDetector.h"

#define MIN_CORRELATION 0.5


HybridAnomalyDetector::HybridAnomalyDetector() {
//    this->setCorrelationThreshold(MIN_CORRELATION);
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

// after found all correlation initial the center point field.
void HybridAnomalyDetector::foundCenter(TimeSeries ts) {
    for (int i = 0; i < cf.size(); i++) {

        // if correlation > 0.9 dont found center
        if (cf[i].corrlation >= this->threshold)
            continue;

        //make new point vector of all points in correlated field
        vector<float> xVector = ts.getVector(cf[i].feature1);
        vector<float> yVector = ts.getVector(cf[i].feature2);
        int n = xVector.size();
        Point **ps = new Point *[n];
        for (int i = 0; i < n; i++) {
            ps[i] = new Point(xVector[i], yVector[i]);
        }

        // use minCircle to find the center and threshold field
        Circle c = findMinCircle(ps,n);
        cf[i].center = &c.center;
        cf[i].threshold = c.radius*1.1;

        delete ps;
    }
}


/****
 * like the learn in simple anomaly detector,
 * also check for center points when correlation < 0.9
 * @param ts
 */
void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    TimeSeries ts1 = TimeSeries(ts);
    for (int i = 0; i < ts1.size(); i++) {
        highCorrelation(i, ts1);
    }
    linReg(ts1);
    foundCenter(ts1);
    treshHold(ts1);
}

/***
 * check anomalous by correlation value
 * @param p
 * @param c
 * @return
 */
bool HybridAnomalyDetector::isAnomalous(Point p, correlatedFeatures c){
    if (c.corrlation >=this->threshold){
        return dev(p, c.lin_reg) > c.threshold;
    }
    else{
        return dist(p,*c.center) > c.threshold;
    }
}

float HybridAnomalyDetector::getTreshold() {
    return this->threshold;
}

