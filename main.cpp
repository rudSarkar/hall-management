#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <map>
#include <iomanip>

using namespace std;

// Student class
class Student {
    private: string name;
    string rollNumber;
    string contactDetails;
    string roomNumber;
    bool mealEnabled;
    double duePayment;

    friend class MealTracker;

    public: Student(const string& name, const string& rollNumber, const string& contactDetails, const string& roomNumber)
        : name(name), rollNumber(rollNumber), contactDetails(contactDetails), roomNumber(roomNumber), mealEnabled(true), duePayment(0.0) {}

    void enableMeal() {
        mealEnabled = true;
    }
    void disableMeal() {
        mealEnabled = false;
    }
    bool isMealEnabled() const {
        return mealEnabled;
    }

    void addPayment(double amount) {
        duePayment -= amount;
    }

    double getDuePayment(const MealTracker& mealTracker) const {
        double due = mealTracker.calculateMealDue(*this);
        return due + duePayment;
    }

    string getName() const {
        return name;
    }
    string getRollNumber() const {
        return rollNumber;
    }
    string getContactDetails() const {
        return contactDetails;
    }
    string getRoomNumber() const {
        return roomNumber;
    }
};


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


// MealTracker class
class MealTracker {
private:
    map<string, vector<time_t>> mealTimes;
    const double mealCost = 100.0; // Assuming a fixed cost per meal

public:
    void recordMeal(const string& rollNumber, time_t mealTime) {
        mealTimes[rollNumber].push_back(mealTime);
    }

    double calculateMealDue(const Student& student) const {
        double due = 0.0;
        const auto& rollNumber = student.getRollNumber();
        const auto& mealTimesForStudent = mealTimes.find(rollNumber);

        if (mealTimesForStudent != mealTimes.end()) {
            due = mealTimesForStudent->second.size() * mealCost;
        }

        return due;
    }

    friend class Student; // Friend declaration
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
        cout << "Entry and Exit Logs:" << endl;
        for (const auto & log: logs) {
            cout << "Roll Number: " << get < 0 > (log) << endl;
            cout << "Entry Time: " << put_time(localtime( & get < 1 > (log)), "%c") << endl;
            if (get < 2 > (log) != 0) {
                cout << "Exit Time: " << put_time(localtime( & get < 2 > (log)), "%c") << endl;
            }
            cout << endl;
        }
    }
};

// Payment class
class Payment {
    public: double calculateDueAmount(bool mealEnabled) {
        // Assume a fixed monthly fee for meal plans
        return mealEnabled ? 100.0 : 0.0;
    }

    void recordPayment(Student & student, double amount) {
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

    void registerStudent(const string & name,
        const string & rollNumber,
            const string & contactDetails,
                const string & roomNumber) {
        students.emplace_back(name, rollNumber, contactDetails, roomNumber);
        cout << "Student registered successfully: " << name << " (" << rollNumber << ")" << endl;
    }

    void enableMealForStudent(const string & rollNumber) {
        for (auto & student: students) {
            if (student.getRollNumber() == rollNumber) {
                student.enableMeal();
                cout << "Meal enabled for student: " << student.getName() << " (" << rollNumber << ")" << endl;
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

    double getDuePaymentForStudent(const string & rollNumber) {
        for (const auto & student: students) {
            if (student.getRollNumber() == rollNumber) {
                return payment.calculateDueAmount(student.isMealEnabled());
            }
        }
        return 0.0; // Student not found
    }

    void makePayment(const string & rollNumber, double amount) {
        for (auto & student: students) {
            if (student.getRollNumber() == rollNumber) {
                payment.recordPayment(student, amount);
                break;
            }
        }
    }

    void printStudentInformation(const MealTracker& mealTracker) const {
        std::cout << std::left << std::setw(20) << "Name" << std::setw(15) << "Roll Number" << std::setw(25) << "Contact Details" << std::setw(15) << "Room Number" << std::setw(12) << "Meal Status" << std::setw(12) << "Due Payment" << std::endl;
        std::cout << std::setfill('-') << std::setw(99) << "-" << std::setfill(' ') << std::endl;
        for (const auto& student : students) {
            std::cout << std::left << std::setw(20) << student.getName() << std::setw(15) << student.getRollNumber() << std::setw(25) << student.getContactDetails() << std::setw(15) << student.getRoomNumber() << std::setw(12) << (student.isMealEnabled() ? "Enabled" : "Disabled") << std::setw(12) << student.getDuePayment(mealTracker) << std::endl;
        }
        std::cout << std::endl;
    }

    void saveStudentDataToFile(const std::string& filename, const MealTracker& mealTracker) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& student : students) {
                file << student.getName() << "," << student.getRollNumber() << "," << student.getContactDetails() << "," << student.getRoomNumber() << "," << student.isMealEnabled() << "," << student.getDuePayment(mealTracker) << std::endl;
            }
            file.close();
            std::cout << "Student data saved to file: " << filename << std::endl;
        } else {
            std::cout << "Unable to open file: " << filename << std::endl;
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
        cout << "Hall Management System Menu:" << endl;
        cout << "1. Register Student" << endl;
        cout << "2. Enable/Disable Meal for Student" << endl;
        cout << "3. Record Student Entry" << endl;
        cout << "4. Record Student Exit" << endl;
        cout << "5. Get Due Payment for Student" << endl;
        cout << "6. Make Payment for Student" << endl;
        cout << "7. Print Student Information" << endl;
        cout << "8. Print Entry and Exit Logs" << endl;
        cout << "9. Print Late Entries" << endl;
        cout << "10. Save Student Data to File" << endl;
        cout << "11. Load Student Data from File" << endl;
        cout << "12. Record Meal for Student" << std::endl;
        cout << "0. Exit" << endl;
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
                break;
            }
            case 2: {
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                cout << "Enable (1) or Disable (0) Meal: ";
                int choice;
                cin >> choice;
                if (choice == 1) {
                    enableMealForStudent(rollNumber);
                } else {
                    disableMealForStudent(rollNumber);
                }
                break;
            }
            case 3: {
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                entryTime = time(nullptr);
                recordStudentEntry(rollNumber, entryTime);
                break;
            }
            case 4: {
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                exitTime = time(nullptr);
                recordStudentExit(rollNumber, exitTime);
                break;
            }
            case 5: {
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                double duePayment = getDuePaymentForStudent(rollNumber);
                cout << "Due Payment for Student " << rollNumber << ": BDT " << duePayment << endl;
                break;
            }
            case 6: {
                cout << "Enter Roll Number: ";
                cin >> rollNumber;
                cout << "Enter Amount: ";
                cin >> amount;
                makePayment(rollNumber, amount);
                break;
            }
            case 7: {
                printStudentInformation(mealTracker);
                break;
            }
            case 8: {
                entryLog.printLogs();
                break;
            }
            case 9: {
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
            case 12: {
                std::cout << "Enter Roll Number: ";
                std::cin >> rollNumber;
                mealTime = std::time(nullptr);
                mealTracker.recordMeal(rollNumber, mealTime);
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