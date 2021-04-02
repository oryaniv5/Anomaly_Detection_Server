/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Or Yaniv 205444805
 */

#include "SimpleAnomalyDetector.h"

#define MIN_CORRELATION 0.9

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    threshold = MIN_CORRELATION;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

//found highest correlation between category
// and initial feature1, feature2, and
// correlation field.
void SimpleAnomalyDetector::highCorrelation(int i, TimeSeries ts) {
    vector<float> x = ts.getVector(i);
    int vectorSize = x.size();
    float highCorrelation = 0;
    int highCorrelationCategory;

    //find the highest correlation between the
    // i categry and all other categry's.
    for (int j = i + 1; j < ts.size(); j++) {
        vector<float> y = ts.getVector(j);
        float XYcorrelation = pearson(x.data(), y.data(), vectorSize);
        if (highCorrelation < XYcorrelation) {
            highCorrelation = XYcorrelation;
            highCorrelationCategory = j;
        }
    }

    //only if the high correlation over MIN_CORRELATION value.
    if (highCorrelation > 0.5) {
        correlatedFeatures newCf = correlatedFeatures();
        newCf.feature1 = ts.getCategoryName(i);
        newCf.feature2 = ts.getCategoryName(highCorrelationCategory);
        newCf.corrlation = highCorrelation;
        cf.push_back(newCf);
    }
}

// after found all correlation initial the lin_reg field.
void SimpleAnomalyDetector::linReg(TimeSeries ts) {
    for (int i = 0; i < cf.size(); i++) {
        if (cf[i].corrlation < this->threshold)
            continue;
        vector<float> xVector = ts.getVector(cf[i].feature1);
        vector<float> yVector = ts.getVector(cf[i].feature2);
        int n = xVector.size();
        Point **ps = new Point *[n];
        for (int i = 0; i < n; i++) {
            ps[i] = new Point(xVector[i], yVector[i]);
        }

        cf[i].lin_reg = linear_reg(ps, xVector.size());
        delete ps;
    }
}

// after found all correlation initial the tresHold field.
void SimpleAnomalyDetector::treshHold(TimeSeries ts) {
    for (int i = 0; i < cf.size(); i++) {
        if (cf[i].corrlation < this->threshold)
            continue;
        vector<float> xVector = ts.getVector(cf[i].feature1);
        vector<float> yVector = ts.getVector(cf[i].feature2);
        Line linReg = cf[i].lin_reg;
        float highDev = 0;
        for (int j = 0; j < xVector.size(); j++) {
            Point p = Point(xVector[j], yVector[j]);
            float pDev = dev(p, linReg);
            if (pDev > highDev) {
                highDev = pDev;
            }
        }
        cf[i].threshold = highDev * 1.1;
    }
}

//learn all correlation and initial all the fields.
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    TimeSeries ts1 = TimeSeries(ts);
    for (int i = 0; i < ts1.size(); i++) {
        highCorrelation(i, ts1);
    }
    linReg(ts1);
    treshHold(ts1);
}

bool SimpleAnomalyDetector::isAnomalous(Point p, correlatedFeatures c) {
    return dev(p, c.lin_reg) > c.threshold;
}

// after learn about data, get new data and return anomaly report.
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> ar;
    TimeSeries ts1 = TimeSeries(ts);

    //for every correlation check new points for deviation.
    for (auto element : cf) {
        vector<float> xVector = ts1.getVector(element.feature1);
        vector<float> yVector = ts1.getVector(element.feature2);
        for (int i = 0; i < xVector.size(); i++) {
            Point p(xVector[i], yVector[i]);
            // if(dev(p,element.lin_reg)>element.threshold){
            if (isAnomalous(p, element)) {
                string arDescription = element.feature1 + "-" + element.feature2;
                AnomalyReport ar1(arDescription, i + 1);
                ar.push_back(ar1);
            }
        }
    }
    return ar;
}