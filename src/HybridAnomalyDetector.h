/*
 * HybridAnomalyDetector.h
 *
 * Author: Or Yaniv 205444805
 */

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "minCircle.h"
#include "SimpleAnomalyDetector.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {

public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();

    void learnNormal(const TimeSeries &ts);
    bool isAnomalous(Point p, correlatedFeatures c);

    void foundCenter(TimeSeries ts);

    float getTreshold();
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
