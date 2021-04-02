/*
 * CLI.cpp
 *
 * Author: Or Yaniv 205444805
 */


#include <sstream>
#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    commandsAssistance* assistance = new commandsAssistance();
    this->menuCommand.push_back(new uploadCommend(this->dio, assistance));
    this->menuCommand.push_back(new algorithmSettingsCommand(this->dio,assistance));
    this->menuCommand.push_back(new detectAnomaliesCommand(this->dio,assistance));
    this->menuCommand.push_back(new displayResultsCommand(this->dio,assistance));
    this->menuCommand.push_back(new uploadAndAnalyzeCommand(this->dio,assistance));

}

void CLI::start(){
    string choice;

    // print CLI and execute user choice,
    // until user input is 6.
    while(true){

        //print CLI menu
        print();

        //get user choice
        choice = this->dio->read();
        int i = stoi(choice);

        //stop condition
        if (i == 6) break;

        // use the number input to decide witch method to execute
        menuCommand[i-1]->execute();
    }

}


CLI::~CLI() {
}

