#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <map>
#include <iomanip>

using namespace std;

class Student;

// MealTracker class
class MealTracker {
private:
    map<string, vector<time_t>> mealTimes;
    const double mealCost = 100.0; // A fixed cost per meal

public:
    bool recordMeal(const string& rollNumber, time_t mealTime) {
        mealTimes[rollNumber].push_back(mealTime);

        return !mealTimes[rollNumber].empty();
    }

    double calculateMealDue(const Student& student) const;

    friend class Student; // Friend declaration
};

// Student class
class Student {
private:
    string name;
    string rollNumber;
    string contactDetails;
    string roomNumber;
    bool mealEnabled;
    double duePayment;

public:
    Student(const string& name, const string& rollNumber, const string& contactDetails, const string& roomNumber)
        : name(name), rollNumber(rollNumber), contactDetails(contactDetails), roomNumber(roomNumber), mealEnabled(true), duePayment(0.0) {}

    void enableMeal() { mealEnabled = true; }
    void disableMeal() { mealEnabled = false; }
    bool isMealEnabled() const { return mealEnabled; }
    void addPayment(double amount) { duePayment -= amount; }
    double getDuePayment(const MealTracker& mealTracker) const {
        double due = mealTracker.calculateMealDue(*this);
        return due + duePayment;
    }

    string getName() const { return name; }
    string getRollNumber() const { return rollNumber; }
    string getContactDetails() const { return contactDetails; }
    string getRoomNumber() const { return roomNumber; }

    friend class MealTracker; // Friend declaration
};

// Definition of the calculateMealDue function
double MealTracker::calculateMealDue(const Student& student) const {
    double due = 0.0;
    const auto& rollNumber = student.getRollNumber();
    const auto& mealTimesForStudent = mealTimes.find(rollNumber);

    if (mealTimesForStudent != mealTimes.end()) {
        due = mealTimesForStudent->second.size() * mealCost;
    }

    return due;
}


// MealSchedule class
class MealSchedule {
    private: vector < pair < time_t,
    time_t >> mealTimings;

    public: void addMealTiming(time_t startTime, time_t endTime) {
        mealTimings.emplace_back(startTime, endTime);
    }

    bool isMealAvailable(time_t currentTime) {
        for (const auto & timing: mealTimings) {
            if (currentTime >= timing.first && currentTime <= timing.second) {
                return true;
            }
        }
        return false;
    }
};


// EntryLog class
class EntryLog {
    private: vector < tuple < string,
    time_t,
    time_t >> logs;

    public: void recordEntry(const string & studentRollNumber, time_t entryTime) {
        logs.emplace_back(studentRollNumber, entryTime, 0);
    }

    void recordExit(const string & studentRollNumber, time_t exitTime) {
        for (auto & log: logs) {
            if (get < 0 > (log) == studentRollNumber && get < 2 > (log) == 0) {
                get < 2 > (log) = exitTime;
                break;
            }
        }
    }

    void printLogs() const {
        // Define column widths
        const int rollNumberWidth = 15;
        const int entryTimeWidth = 25;
        const int exitTimeWidth = 25;

        // Display header
        cout << setw(rollNumberWidth) << left << "Roll Number";
        cout << setw(entryTimeWidth) << left << "Entry Time";
        cout << setw(exitTimeWidth) << left << "Exit Time" << endl;

        // Display logs
        for (const auto &log : logs) {
            // Roll Number
            cout << setw(rollNumberWidth) << left << get<0>(log);

            // Entry Time
            cout << setw(entryTimeWidth) << left << put_time(localtime(&get<1>(log)), " %c");

            // Exit Time if available
            if (get<2>(log) != 0) {
                cout << setw(exitTimeWidth) << left << put_time(localtime(&get<2>(log)), " %c");
            } else {
                cout << setw(exitTimeWidth) << left << " N/A";
            }

            cout << endl;
        }
    }
};

// Payment class
class Payment {
    public: double calculateDueAmount(bool mealEnabled) {
        // A fixed monthly fee for meal plans
        return mealEnabled ? 100.0 : 0.0;
    }

    void recordPayment(Student &student, double amount) {
        student.addPayment(amount);
    }
};

// LateEntry class
class LateEntry {
    private: time_t restrictedTime;
    vector < pair < string,
    time_t >> lateEntries;

    public: LateEntry(time_t restrictedTime): restrictedTime(restrictedTime) {}

    void recordLateEntry(const string & studentRollNumber, time_t entryTime) {
        if (entryTime > restrictedTime) {
            lateEntries.emplace_back(studentRollNumber, entryTime);
        }
    }

    void printLateEntries() const {
        cout << "Late Entries:" << endl;
        for (const auto & entry: lateEntries) {
            cout << "Roll Number: " << entry.first << endl;
            cout << "Entry Time: " << put_time(localtime( & entry.second), "%c") << endl;
            cout << endl;
        }
    }
};

// Hall Management System class
class HallManagementSystem {
    private: vector < Student > students;
    MealSchedule mealSchedule;
    EntryLog entryLog;
    Payment payment;
    LateEntry lateEntry;

    public: HallManagementSystem(time_t restrictedTime): lateEntry(restrictedTime) {
        // Initialize meal timings (example)
        time_t breakfastStart = time(nullptr);
        breakfastStart = breakfastStart + (8 * 60 * 60); // 8 AM
        time_t breakfastEnd = breakfastStart + (2 * 60 * 60); // 10 AM
        mealSchedule.addMealTiming(breakfastStart, breakfastEnd);

        time_t lunchStart = breakfastEnd + (2 * 60 * 60); // 12 PM
        time_t lunchEnd = lunchStart + (2 * 60 * 60); // 2 PM
        mealSchedule.addMealTiming(lunchStart, lunchEnd);

        time_t dinnerStart = lunchEnd + (4 * 60 * 60); // 6 PM
        time_t dinnerEnd = dinnerStart + (2 * 60 * 60); // 8 PM
        mealSchedule.addMealTiming(dinnerStart, dinnerEnd);
    }

    void registerStudent(const string& name, const string& rollNumber, const string& contactDetails, const string& roomNumber) {
        // Check if the roll number already exists
        for (const auto& student : students) {
            if (student.getRollNumber() == rollNumber) {
                cout << "Error: Roll number " << rollNumber << " already exists. Cannot register student." << endl;
                return;
            }
        }

        students.emplace_back(name, rollNumber, contactDetails, roomNumber);
    }

    void enableMealForStudent(const string & rollNumber) {
        for (auto & student: students) {
            if (student.getRollNumber() == rollNumber) {
                student.enableMeal();
                break;
            }
        }
    }

    void disableMealForStudent(const string & rollNumber) {
        for (auto & student: students) {
            if (student.getRollNumber() == rollNumber) {
                student.disableMeal();
                cout << "Meal disabled for student: " << student.getName() << " (" << rollNumber << ")" << endl;
                break;
            }
        }
    }

    void recordStudentEntry(const string & rollNumber, time_t entryTime) {
        entryLog.recordEntry(rollNumber, entryTime);
        lateEntry.recordLateEntry(rollNumber, entryTime);
    }

    void recordStudentExit(const string & rollNumber, time_t exitTime) {
        entryLog.recordExit(rollNumber, exitTime);
    }

    void makePayment(const string & rollNumber, double amount) {
        bool found = false;
        for (auto & student: students) {
            if (student.getRollNumber() == rollNumber) {
                payment.recordPayment(student, amount);
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "No student found with roll number: " << rollNumber << endl;
        }
    }

    void printStudentInformation(const MealTracker& mealTracker) const {
        cout << left << setw(20) << "Name" << setw(15) << "Roll Number" << setw(25) << "Contact Details" << setw(15) << "Room Number" << setw(12) << "Meal Status" << setw(12) << "Due Payment" << endl;
        cout << setfill('-') << setw(99) << "-" << setfill(' ') << endl;
        for (const auto& student : students) {
            cout << left << setw(20) << student.getName() << setw(15) << student.getRollNumber() << setw(25) << student.getContactDetails() << setw(15) << student.getRoomNumber() << setw(12) << (student.isMealEnabled() ? "Enabled" : "Disabled") << setw(12) << student.getDuePayment(mealTracker) << endl;
        }
        cout << endl;
    }

    void saveStudentDataToFile(const string& filename, const MealTracker& mealTracker) const {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& student : students) {
                file << student.getName() << "," << student.getRollNumber() << "," << student.getContactDetails() << "," << student.getRoomNumber() << "," << student.isMealEnabled() << "," << student.getDuePayment(mealTracker) << endl;
            }
            file.close();
            cout << "Student data saved to file: " << filename << endl;
        } else {
            cout << "Unable to open file: " << filename << endl;
        }
    }


    void loadStudentDataFromFile(const string & filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                string name, rollNumber, contactDetails, roomNumber;
                bool mealEnabled;
                double duePayment;

                size_t pos = 0;
                size_t delimiter = line.find(',');
                name = line.substr(pos, delimiter);
                pos = delimiter + 1;

                delimiter = line.find(',', pos);
                rollNumber = line.substr(pos, delimiter - pos);
                pos = delimiter + 1;

                delimiter = line.find(',', pos);
                contactDetails = line.substr(pos, delimiter - pos);
                pos = delimiter + 1;

                delimiter = line.find(',', pos);
                roomNumber = line.substr(pos, delimiter - pos);
                pos = delimiter + 1;

                delimiter = line.find(',', pos);
                mealEnabled = (line.substr(pos, delimiter - pos) == "1");
                pos = delimiter + 1;

                duePayment = stod(line.substr(pos));

                registerStudent(name, rollNumber, contactDetails, roomNumber);
                if (mealEnabled) {
                    enableMealForStudent(rollNumber);
                } else {
                    disableMealForStudent(rollNumber);
                }
                makePayment(rollNumber, -duePayment); // Negative payment to set the due amount
            }
            file.close();
            cout << "Student data loaded from file: " << filename << endl;
        } else {
            cout << "Unable to open file: " << filename << endl;
        }
    }

    void displayMenu() const {
        cout << "===================================" << endl;
        cout << "|   Hall Management System Menu   |" << endl;
        cout << "===================================" << endl;
        cout << "| 1. Register Student             |" << endl;
        cout << "| 2. Manage Meal                  |" << endl;
        cout << "| 3. Record Meal                  |" << endl;
        cout << "| 4. Record Entry                 |" << endl;
        cout << "| 5. Record Exit                  |" << endl;
        cout << "| 6. Make Payment                 |" << endl;
        cout << "| 7. Print Student Details        |" << endl;
        cout << "| 8. View Entry/Exit Logs         |" << endl;
        cout << "| 9. View Late Entries            |" << endl;
        cout << "| 10. Save Student Data           |" << endl;
        cout << "| 11. Load Student Data           |" << endl;
        cout << "| 0. Exit                         |" << endl;
        cout << "===================================" << endl;
        cout << "Enter your choice: ";
    }

    void run() {
        int choice;
        string name, rollNumber, contactDetails, roomNumber;
        time_t entryTime, exitTime, mealTime;
        double amount;
        MealTracker mealTracker;
        
        do {
            displayMenu();
            cin >> choice;

            switch (choice) {
            case 1: {
                cout << endl;
                cout << "=== Register Student ===" << endl;
                
                cout << "Enter Student Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Roll Number: ";
                getline(cin, rollNumber);
                cout << "Enter Contact Details: ";
                getline(cin, contactDetails);
                cout << "Enter Room Number: ";
                getline(cin, roomNumber);
                registerStudent(name, rollNumber, contactDetails, roomNumber);
                cout << "Student registered successfully: " << name << " (" << rollNumber << ")" << endl;
                break;
            }
            case 2: {
                cout << endl;
                cout << "=== Enable/Disable Meal ===" << endl;

                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                cout << "Enable (1) or Disable (0) Meal: ";
                int choice;
                cin >> choice;
                if (choice == 1) {
                    enableMealForStudent(rollNumber);
                    cout << "Meal enabled for student roll: " << " (" << rollNumber << ")" << endl;
                } else {
                    disableMealForStudent(rollNumber);
                    cout << "Meal disabled for student roll: " << " (" << rollNumber << ")" << endl;
                }
                break;
            }
            case 3: {
                cout << endl;
                cout << "=== Record Meal ===" << endl;

                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                mealTime = time(nullptr);
                
                if (mealTracker.recordMeal(rollNumber, mealTime)) {
                    cout << "Meal recorded successfully!" << endl;
                } else {
                    cout << "Failed to record the meal." << endl;
                }

                break;
            }
            case 4: {
                cout << endl;
                cout << "=== Record Entry ===" << endl;

                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                entryTime = time(nullptr);
                recordStudentEntry(rollNumber, entryTime);
                break;
            }
            case 5: {
                cout << endl;
                cout << "=== Record Exit ===" << endl;

                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                exitTime = time(nullptr);
                recordStudentExit(rollNumber, exitTime);
                break;
            }
            case 6: {
                cout << endl;
                cout << "=== Make Meal Payment ===" << endl;

                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                cout << "Enter Amount: ";
                cin >> amount;
                makePayment(rollNumber, amount);
                break;
            }
            case 7: {
                cout << endl;
                cout << "=== Student Information ===" << endl;
                cout << endl;

                printStudentInformation(mealTracker);
                break;
            }
            case 8: {
                cout << endl;
                cout << "=== Student Entry Logs ===" << endl;
                cout << endl;

                entryLog.printLogs();
                break;
            }
            case 9: {
                cout << endl;
                cout << "=== Student Late Entries ===" << endl;
                cout << endl;

                lateEntry.printLateEntries();
                break;
            }
            case 10: {
                cout << "Enter File Name: ";
                cin.ignore();
                getline(cin, name);
                saveStudentDataToFile(name, mealTracker);
                break;
            }
            case 11: {
                cout << "Enter File Name: ";
                cin.ignore();
                getline(cin, name);
                loadStudentDataFromFile(name);
                break;
            }
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
            }
            cout << endl;
        } while (choice != 0);
    }
};
int main() {
    // Set the restricted time for late entry (e.g., 11PM)
    time_t restrictedTime = time(nullptr);
    restrictedTime = restrictedTime + (23 * 60 * 60); // 11 PM
    HallManagementSystem hms(restrictedTime);
    hms.run();

    return 0;
}