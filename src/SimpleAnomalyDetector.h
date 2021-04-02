/*
 * SimpleAnomalyDetector.h
 *
 * Author: Or Yaniv 205444805
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "timeseries.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    Point* center;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
    vector<correlatedFeatures> cf;
    float threshold;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();

    virtual void highCorrelation(int i, TimeSeries ts);
    virtual void treshHold(TimeSeries ts);
    virtual void linReg(TimeSeries ts);
    virtual bool isAnomalous(Point p, correlatedFeatures c);

    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
    void setCorrelationThreshold(float threshold){
        this->threshold=threshold;
    }


};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
