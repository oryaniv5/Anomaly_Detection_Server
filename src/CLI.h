/*
 * CLI.h
 *
 * Author: Or Yaniv 205444805
 */


#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"
#include "timeseries.h"

using namespace std;

class CLI {
    DefaultIO *dio;
    vector<Command *> menuCommand;

public:
    CLI(DefaultIO *dio);

    void start();

    virtual ~CLI();

    void print() {
        dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
        int n = menuCommand.size();
        for (int i = 0; i < n; i++) {

            string b = to_string(i+1)+".";
            dio->write(b);
            menuCommand[i]->print();
        }
        dio->write("6.exit\n");
    }
};

#endif /* CLI_H_ */
