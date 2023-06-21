#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using namespace std;

string exec(const char* cmd);
void toggle(const string & serviceName, const bool isRunning);
bool serviceIsRunning(const string & queryString);

int main(){
    // string containing the results of "sc query serviceName"
    string queryStatus;

    // name of service to be toggled on/off
    string service = "LITSSVC";

    // command to query service
    string query_cmd = "sc query " + service;

    // obtain status of service and save as string
    queryStatus = exec(query_cmd.c_str());

    // true if service is currently running
    bool serviceStatus = serviceIsRunning(queryStatus);

    // display status of service to user and prompt for action
    cout << "Service \"" << service << "\" is currently running: ";
    if(serviceStatus){
        cout << "true." << endl;
    }
    else{
        cout << "false." << endl;
    }
    cout << "Press ENTER to toggle the service on/off." << endl;
    cin.get();

    // turn service on/off
    toggle(service, serviceStatus);

    cout << "Press ENTER to quit." << endl;
    cin.get();
    return 0;
}

// returns string returned from command "cmd" from cmd.exe
string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// toggles "serviceName" service on/off
void toggle(const string & serviceName, const bool isRunning){
    // command to start/stop service
    string start_cmd = "net start " + serviceName;
    string stop_cmd = "net stop " + serviceName;

    if(isRunning){
        // if service is running, stop the service
        system(stop_cmd.c_str());
    }
    else{
        // if service is not running, start the service
        system(start_cmd.c_str());
    }
}

// returns true if "serviceName" service is currently running
bool serviceIsRunning(const string & queryString){
    if(queryString.find("RUNNING") != std::string::npos){
        return true;
    }
    return false;
}