

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <string.h>
#include <string>
#include <vector>
#include <map>


using namespace std;

class TimeSeries{

    map<int,string> categoryNumber;
    map<string, vector<float>> ts;

public:

    TimeSeries(const char* CSVfileName);
    TimeSeries(const TimeSeries& oldTs);
    vector<float> getVector(string s);
    vector<float> getVector(int i);
    void print();
    int size();
    void addLine(string line);
    int tsLineSize();


    string getCategoryName(int i);

    void set(TimeSeries ts);
};



#endif /* TIMESERIES_H_ */
