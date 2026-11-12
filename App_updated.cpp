
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <stdexcept>
#include <limits>

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
            cout << log <<endl;
        }
    }
};

// 3. WARNING LOG STRATEGY

class WarningLogStrategy : public LogStrategy {
public:
    void process(const string& log) override {
        if (log.find("WARNING") != string::npos) {
            cout << log <<endl;
        }
    }
};

// 4. INFO LOG STRATEGY

class InfoLogStrategy : public LogStrategy {
public:
    void process(const string& log) override {
        if (log.find("INFO") != string::npos) {
            cout << log <<endl;
        }
    }
};

// 5. ALL LOG STRATEGY

class AllLogStrategy : public LogStrategy {
public:
    void process(const string& log) override {
        cout << log <<endl;
    }
};

// 6. LOG STATISTICS

class LogStatistics {
private:
    long long totalLogs = 0;
    long long infoCount = 0;
    long long warningCount = 0;
    long long errorCount = 0;
    long long otherCount = 0;

public:
    void process(const string& log) {

        totalLogs++;

        if (log.find("ERROR") != string::npos) {
            errorCount++;
        }
        else if (log.find("WARNING") != string::npos) {
            warningCount++;
        }
        else if (log.find("INFO") != string::npos) {
            infoCount++;
        }
        else {
            otherCount++;
        }
    }

    void display() const {

        cout << "\n========== LOG STATISTICS ==========\n";

        cout << "Total Logs : " << totalLogs <<endl;
        cout << "INFO       : " << infoCount <<endl;
        cout << "WARNING    : " << warningCount <<endl;
        cout << "ERROR      : " << errorCount <<endl;
        cout << "OTHER      : " << otherCount <<endl;

        cout << "====================================\n";
    }
};

// 7. LOG PROCESSOR (CONTEXT)

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

// 8. LOG FILE READER

class LogFileReader {
public:

    void readFile(
        const string& filename,
        LogProcessor* processor,
        LogStatistics* statistics
    ) {

        ifstream file(filename);

        if (!file.is_open()) {
            throw runtime_error(
                "Unable to open log file: " + filename
            );
        }

        string line;

        long long lineNumber = 0;

        cout << "Processing log file...";

        while (getline(file, line)) {

            lineNumber++;

            if (line.empty()) {
                continue;
            }

            // Process using selected strategy
            if (processor != nullptr) {
                processor->processLog(line);
            }

            // Update statistics
            if (statistics != nullptr) {
                statistics->process(line);
            }
        }

        if (file.bad()) {
            throw runtime_error("Error occurred while reading file.");
        }

        cout << "File processing completed.";
        cout << "Lines read: " << lineNumber <<endl;

        file.close();
    }
};

// 9. MAIN METHOD

int main() {

    try {

        LogFileReader reader;

        LogProcessor processor;

        LogStatistics statistics;

        string filename = "application.log";

        int choice;

        while (true) {

            cout << "\n====================================\n";
            cout << "       C++ LOG PROCESSING TOOL      \n";
            cout << "====================================\n";

            cout << "1. Show All Logs\n";
            cout << "2. Show Error Logs\n";
            cout << "3. Show Warning Logs\n";
            cout << "4. Show Info Logs\n";
            cout << "5. Show Statistics\n";
            cout << "6. Exit\n";

            cout << "\nEnter your choice: ";

            if (!(cin >> choice)) {

                if (cin.eof()) {
                    cout << "\nInput closed. Exiting...\n";
                    break;
                }

                cin.clear();

                cin.ignore(
                    numeric_limits<streamsize>::max(),
                    '\n'
                );

                cout << "Invalid input. Enter a number.\n";

                continue;
            }

            // 1. ALL LOGS

            if (choice == 1) {

                processor.setStrategy(
                    make_unique<AllLogStrategy>()
                );

                reader.readFile(
                    filename,
                    &processor,
                    nullptr
                );
            }

            // 2. ERROR LOGS

            else if (choice == 2) {

                processor.setStrategy(
                    make_unique<ErrorLogStrategy>()
                );

                reader.readFile(
                    filename,
                    &processor,
                    nullptr
                );
            }

            // 3. WARNING LOGS

            else if (choice == 3) {

                processor.setStrategy(
                    make_unique<WarningLogStrategy>()
                );

                reader.readFile(
                    filename,
                    &processor,
                    nullptr
                );
            }

            // 4. INFO LOGS

            else if (choice == 4) {

                processor.setStrategy(
                    make_unique<InfoLogStrategy>()
                );

                reader.readFile(
                    filename,
                    &processor,
                    nullptr
                );
            }

            // 5. STATISTICS

            else if (choice == 5) {

                LogStatistics statistics;

                reader.readFile(
                    filename,
                    nullptr,
                    &statistics
                );

                statistics.display();
            }

            // 6. EXIT

            else if (choice == 6) {

                cout << "\nExiting application...\n";

                break;
            }

            // INVALID CHOICE

            else {

                cout << "\nInvalid choice. Please try again.\n";
            }
        }

    }

    catch (const exception& e) {

        cerr << "\nERROR: " << e.what() << '\n';

        return 1;
    }

    return 0;
}
