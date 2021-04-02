/*
 * commands.cpp
 *
 * Author: Or Yaniv 205444805
 */


#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "timeseries.h"
#include<iostream>
#include <sstream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}

    // you may add additional methods here
};

// you may add here helper classes
class commandsAssistance {
public:
    HybridAnomalyDetector *hybDetector; // names of the correlated features
    TimeSeries *ts1;
    TimeSeries *ts2;

    commandsAssistance() {
        hybDetector = new HybridAnomalyDetector();
    };
};

// you may edit this class
class Command {

protected:
    DefaultIO *dio;
    commandsAssistance *assistance;
    string description;

public:
    Command(DefaultIO *dio, commandsAssistance *assistance) : dio(dio), assistance(assistance) {}

    virtual void execute() = 0;

    virtual ~Command() {}

    void print() {
        dio->write(description + "\n");
    }
};

//commends implementation

//1. upload commend
class uploadCommend : public Command {

    // create csv file from dio input
    // when input is 'done' stop.
    void createCsv(const char *name) {

        // open new  file
        ofstream outputFile;
        outputFile.open(name);

        //get file fields data
        string line = dio->read();
        while (line != "done") {

            //write line into  file
            outputFile << line << endl;
            line = dio->read();
        }
        outputFile.close();
    }

    void execute() override {

        // train file initial
        dio->write("Please upload your local train CSV file.\n");
        createCsv("anomalyTrain.csv");
        TimeSeries *train = new TimeSeries("anomalyTrain.csv");
        assistance->ts1 = train;
        dio->write("Upload complete.\n");

        // test file initial
        dio->write("Please upload your local test CSV file.\n");
        createCsv("anomalyTest.csv");
        TimeSeries *test = new TimeSeries("anomalyTest.csv");
        assistance->ts2 = test;
//        assistance->ts2->print();
        dio->write("Upload complete.\n");;
    }

    ~uploadCommend() {}

public:
    uploadCommend(DefaultIO *dio, commandsAssistance *assistance) : Command(dio, assistance) {
        this->description = "upload a time series csv file ";
    }
};

//2.algorithm Settings Command
class algorithmSettingsCommand : public Command {

    void execute() override {

        string treshold = to_string(assistance->hybDetector->getTreshold());
        treshold = treshold.substr(0,treshold.find(".")+2);
        dio->write("The current correlation threshold is " + treshold + '\n');
        dio->write("Type a new threshold\n");
        float newTreshold;
        while (true) {
            dio->read(&newTreshold);
            if (newTreshold >= 0 && newTreshold <= 1) {
                assistance->hybDetector->setCorrelationThreshold(newTreshold);
                break;

            }
            dio->write("please choose a value between 0 and 1.\n");
        }

    }

    ~algorithmSettingsCommand() {}

public:
    algorithmSettingsCommand(DefaultIO *dio, commandsAssistance *assistance) : Command(dio, assistance) {
        this->description = "algorithm settings";
    }
};

//3. detect anomalies
class detectAnomaliesCommand : public Command {

    void execute() override {
        assistance->hybDetector->learnNormal(*assistance->ts1);
        vector<AnomalyReport> ar = assistance->hybDetector->detect(*assistance->ts2);
        dio->write("anomaly detection complete.\n");
    }

    ~detectAnomaliesCommand() {}

public:
    detectAnomaliesCommand(DefaultIO *dio, commandsAssistance *assistance) : Command(dio, assistance) {
        this->description = "detect anomalies";
    }
};

//4. display results
class displayResultsCommand : public Command {

    void execute() override {
        vector<AnomalyReport> ar = assistance->hybDetector->detect(*assistance->ts2);
        for (auto element: ar) {
            string timeStep = to_string(element.timeStep);
            dio->write(timeStep);
            dio->write("         ");
            dio->write(element.description+'\n');
        }
        dio->write("Done.\n");

    }

    ~displayResultsCommand() {}

public:
    displayResultsCommand(DefaultIO *dio, commandsAssistance *assistance) : Command(dio, assistance) {
        this->description = "display results";
    }
};

//5. upload anomalies and analyze results
class uploadAndAnalyzeCommand : public Command {

    vector<pair<int, int>> getFileVector() {
        vector<pair<int, int>> fileVector;
        string time1, time2;
        int t1, t2;
        string line = dio->read();
        while (line != "done") {
            stringstream ss(line);
            getline(ss, time1, ',');
            getline(ss, time2, ',');
            t1 = stoi(time1);
            t2 = stoi(time2);
            fileVector.emplace_back(t1, t2);
            line = dio->read();
        }
        return fileVector;
    }

    int fileSize(vector<pair<int, int>> fileVector) {
        int size = 0;
        for (pair<int, int> element: fileVector) {
            size += element.second - element.first + 1;
        }
        return size;
    }

    vector<pair<int, int>> getDetectVector(vector<AnomalyReport> ar) {
        vector<pair<int, int>> detectVector;
        detectVector.emplace_back(ar[0].timeStep, ar[0].timeStep);
        int i=0;

        for (const AnomalyReport &element : ar) {
            if (detectVector[i].second == element.timeStep - 1||
                detectVector[i].second == element.timeStep) {
                detectVector[i].second = element.timeStep;
            } else {
                detectVector.push_back(pair<int,int>(element.timeStep, element.timeStep));
                i++;
            }
        }
        return detectVector;
    }

    bool rangeUnion(pair<int, int> p1, pair<int, int> p2) {
        if (p1.first <= p2.first && p1.second >= p2.first ||
            p1.first <= p2.second && p1.second >= p2.second||
            p1.first >= p2.first && p1.second <= p2.second) {
            return true;
        }
        return false;
    }

    int truePositive(vector<pair<int, int>> fileVector, vector<pair<int, int>> detectVector) {
        int tp=0;
        for (pair<int, int> element1 : fileVector) {
            for (pair<int, int> element2 : detectVector) {
                if (rangeUnion(element1,element2)){
                    tp++;
                    break;
                }
            }
        }
        return tp;
    }

    int falsePositive(vector<pair<int, int>> fileVector, vector<pair<int, int>> detectVector) {
        int fp = detectVector.size();
        for(pair<int, int> element1 : detectVector){
            for (pair<int, int> element2 : fileVector){
                if(rangeUnion(element1,element2)){
                    fp--;
                    break;
                }
            }
        }

        return fp;
    }

    void execute() override {
        int tp, fp, n, p;
        dio->write("Please upload your local anomalies file.\n");
        vector<AnomalyReport> ar = assistance->hybDetector->detect(*assistance->ts2);
        vector<pair<int, int>> fileVector = getFileVector();
        vector<pair<int, int>> detectVector = getDetectVector(ar);
        p = fileVector.size();
        n = assistance->ts2->tsLineSize() - fileSize(fileVector);
        tp = truePositive(fileVector, detectVector);
        fp = falsePositive(fileVector, detectVector);

//        cout<<fp<<endl;
        dio->write("Upload complete.\n");
        dio->write("True Positive Rate: ");
        dio->write((float) tp / (float) p);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        float far_f = (float) fp / (float) n;
        far_f = floorf(far_f*1000)/1000;
//        string far = to_string(far_f);
//        far = far.substr(0,far.find(".")+3);
        dio->write(far_f);
        dio->write("\n");
    }

    ~uploadAndAnalyzeCommand() {}

public:
    uploadAndAnalyzeCommand(DefaultIO *dio, commandsAssistance *assistance) : Command(dio, assistance) {
        this->description = "upload anomalies and analyze results";
    }
};


#endif /* COMMANDS_H_ */
