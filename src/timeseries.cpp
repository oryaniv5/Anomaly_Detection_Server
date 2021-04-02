/*
 * timeseries.cpp
 *
 * Author: Or Yaniv 205444805
 */


#include "timeseries.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <iterator>

//constructor for timeSeries, get only
//csv file name.
TimeSeries::TimeSeries(const char *CSVfileName) {
    string line, category;
    int categoryNum = 0;
    ifstream fin;
    fin.open(CSVfileName);
    getline(fin, line);
    stringstream ss1(line);


    //init category field
    while (getline(ss1, category, ',')) {
        categoryNumber.insert({categoryNum, category});
        ts.insert({category, vector<float>{}});
        categoryNum++;
    }

    //init veoctor of values
    while (getline(fin, line)) {
        stringstream ss2(line);
        float colInfo;
        string colName;
        for (int i = 0; i < categoryNumber.size(); i++) {
            ss2 >> colInfo;
            colName = categoryNumber.find(i)->second;
            ts.find(colName)->second.push_back(colInfo);
            if (ss2.peek() == ',') ss2.ignore();

        }
    }
    fin.close();
}

//copy constructor.
TimeSeries::TimeSeries(const TimeSeries &oldTs) {
    categoryNumber = oldTs.categoryNumber;
    ts = oldTs.ts;
}

// return the category name in colman i.
string TimeSeries::getCategoryName(int i) {
    if (i <= categoryNumber.size()) {
        return categoryNumber.find(i)->second;
    }
    return nullptr;
}


//return vector for category s.
vector<float> TimeSeries::getVector(string s) {
    map<string, vector<float>>::iterator itr;
    itr = ts.find(s);
    if (itr != ts.end()) {
        return itr->second;
    }
    return {};
}

//return vector for the i category.
vector<float> TimeSeries::getVector(int i) {
    if (i <= categoryNumber.size()) {
        return getVector(categoryNumber.find(i)->second);
    }
    return {};
}


//return size of map.
int TimeSeries::size() {
    return categoryNumber.size();
}


//print timeSeries map.
void TimeSeries::print() {

    map<string, vector<float>>::iterator itr;
    for (itr = ts.begin(); itr != ts.end(); ++itr) {
        cout << itr->first << " { ";
        for (int i = 0; i < itr->second.size(); i++) {
            cout << itr->second[i] << " ";
        }
        cout << "}" << endl;
    }

}

void TimeSeries::set(TimeSeries ts) {
    this->categoryNumber = ts.categoryNumber;
    this->ts = ts.ts;

}

int TimeSeries::tsLineSize() {
    return ts.begin()->second.size();
}

