#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

using namespace std;



// 1. STRATEGY INTERFACE

class LogStrategy {
public:
    virtual void process(const string& log) = 0;

    virtual ~LogStrategy() = default;
};

// 2. ERROR LOG STRATEGY


class ErrorLogStrategy : public LogStrategy {
public:

    void process(const string& log) override {

        if (log.find("ERROR") != string::npos) {
            cout << log << endl;
        }
    }
};


// 3. WARNING LOG STRATEGY

class WarningLogStrategy : public LogStrategy {
public:

    void process(const string& log) override {

        if (log.find("WARNING") != string::npos) {
            cout << log << endl;
        }
    }
};



// 4. INFO LOG STRATEGY

class InfoLogStrategy : public LogStrategy {
public:

    void process(const string& log) override {

        if (log.find("INFO") != string::npos) {
            cout << log << endl;
        }
    }
};



// 5. ALL LOG STRATEGY

class AllLogStrategy : public LogStrategy {
public:

    void process(const string& log) override {

        cout << log << endl;
    }
};



// 6. LOG PROCESSOR - CONTEXT


class LogProcessor {

private:

    unique_ptr<LogStrategy> strategy;

public:

    void setStrategy(unique_ptr<LogStrategy> newStrategy) {

        strategy = move(newStrategy);
    }


    void processLog(const string& log) {

        if (strategy) {
            strategy->process(log);
        }
    }
};


// 7. LOG FILE READER

class LogFileReader {

public:

    vector<string> readFile(const string& filename) {

        ifstream file(filename);

        if (!file.is_open()) {

            throw runtime_error(
                "Unable to open log file: " + filename
            );
        }

        vector<string> logs;

        string line;

        while (getline(file, line)) {

            if (!line.empty()) {
                logs.push_back(line);
            }
        }

        file.close();

        return logs;
    }
};

// 8. LOG STATISTICS

class LogStatistics {

public:

    void generateStatistics(
        const vector<string>& logs) {

        int infoCount = 0;
        int warningCount = 0;
        int errorCount = 0;

        for (const auto& log : logs) {

            if (log.find("ERROR") != string::npos) {
                errorCount++;
            }
            else if (log.find("WARNING") != string::npos) {
                warningCount++;
            }
            else if (log.find("INFO") != string::npos) {
                infoCount++;
            }
        }

        cout << endl;
        cout << "========== LOG STATISTICS ==========" << endl;

        cout << "Total Logs : "
             << logs.size() << endl;

        cout << "INFO       : "
             << infoCount << endl;

        cout << "WARNING    : "
             << warningCount << endl;

        cout << "ERROR      : "
             << errorCount << endl;

        cout << "====================================" << endl;
    }
};

// 9. DISPLAY LOGS USING STRATEGY

void displayLogs(
    LogProcessor& processor,
    const vector<string>& logs) {

    for (const auto& log : logs) {

        processor.processLog(log);
    }
}

// 10. MAIN METHOD

int main() {

    try {

      
        // Read log file

        LogFileReader reader;

        vector<string> logs =
            reader.readFile("application.log");


        cout << "====================================" << endl;
        cout << "       C++ LOG PROCESSING TOOL      " << endl;
        cout << "====================================" << endl;

        cout << "Log file loaded successfully." << endl;
        cout << "Total logs: "
             << logs.size() << endl;

        // Create Log Processor

        LogProcessor processor;


        int choice;


        while (true) {

            cout << endl;

            cout << "========== MENU ==========" << endl;

            cout << "1. Show All Logs" << endl;
            cout << "2. Show Error Logs" << endl;
            cout << "3. Show Warning Logs" << endl;
            cout << "4. Show Info Logs" << endl;
            cout << "5. Show Statistics" << endl;
            cout << "6. Exit" << endl;

            cout << "Enter your choice: ";

            cin >> choice;


           
            // Show all logs

            if (choice == 1) {

                cout << endl;
                cout << "========== ALL LOGS =========="
                     << endl;

                processor.setStrategy(
                    make_unique<AllLogStrategy>()
                );

                displayLogs(processor, logs);
            }
            // Show ERROR logs

            else if (choice == 2) {

                cout << endl;
                cout << "========== ERROR LOGS =========="
                     << endl;

                processor.setStrategy(
                    make_unique<ErrorLogStrategy>()
                );

                displayLogs(processor, logs);
            }

            // Show WARNING logs

            else if (choice == 3) {

                cout << endl;
                cout << "========== WARNING LOGS =========="
                     << endl;

                processor.setStrategy(
                    make_unique<WarningLogStrategy>()
                );

                displayLogs(processor, logs);
            }
            // Show INFO logs

            else if (choice == 4) {

                cout << endl;
                cout << "========== INFO LOGS =========="
                     << endl;

                processor.setStrategy(
                    make_unique<InfoLogStrategy>()
                );

                displayLogs(processor, logs);
            }
            // Statistics

            else if (choice == 5) {

                LogStatistics statistics;

                statistics.generateStatistics(logs);
            }
            // Exit

            else if (choice == 6) {

                cout << endl;
                cout << "Exiting application..." << endl;

                break;
            }

            // Invalid choice
            else {

                cout << "Invalid choice. Please try again."
                     << endl;
            }
        }

    }
    catch (const exception& e) {

        cerr << "ERROR: "
             << e.what()
             << endl;

        return 1;
    }


    return 0;
}