// Student Management System

#include <bits/stdc++.h>
using namespace std;

class Person {
protected:
    string name;
    string contact;

public:
    void inputBasicInfo() {
        cout << "Enter Name: ";
        cin >> name;
        cout << "Enter Contact: ";
        cin >> contact;
    }

    void showBasicInfo() const {
        cout << "Name: " << name << endl;
        cout << "Contact: " << contact << endl;
    }
};

class Student : public Person {
private:
    string roll;
    string course;
    string clas;

public:
    void inputData(int index) {
        cout << "\nEnter Data of student " << index + 1 << endl;

        inputBasicInfo();

        cout << "Enter Roll No: ";
        cin >> roll;
        cout << "Enter Course: ";
        cin >> course;
        cout << "Enter Class: ";
        cin >> clas;
    }

    void showData(int index) const {
        cout << "\nData of Student " << index + 1 << endl;

        showBasicInfo();

        cout << "Roll No: " << roll << endl;
        cout << "Course: " << course << endl;
        cout << "Class: " << clas << endl;
    }

    string getRoll() const {
        return roll;
    }

    void updateData() {
        cout << "\nEnter new data" << endl;

        inputBasicInfo();

        cout << "Enter Roll No: ";
        cin >> roll;
        cout << "Enter Course: ";
        cin >> course;
        cout << "Enter Class: ";
        cin >> clas;
    }
};

class ManagementSystem {
private:
    Student students[20];
    int total;

public:
    ManagementSystem() {
        total = 0;
    }

    void enterData() {
        int ch;
        cout << "How many students do you want to enter? " << endl;
        cin >> ch;

        if (total + ch > 20) {
            cout << "You can only store up to 20 students in total." << endl;
            cout << "Currently stored: " << total << endl;
            cout << "You can add at most: " << (20 - total) << " more student(s)." << endl;
            return;
        }

        for (int i = total; i < total + ch; i++) {
            students[i].inputData(i);
        }
        total += ch;
    }

    void showData() const {
        if (total == 0) {
            cout << "No data is entered." << endl;
        } else {
            for (int i = 0; i < total; i++) {
                students[i].showData(i);
            }
        }
    }

    void searchData() const {
        if (total == 0) {
            cout << "No data is entered." << endl;
        } else {
            string roll;
            cout << "Enter Roll No to search: ";
            cin >> roll;
            bool found = false;

            for (int i = 0; i < total; i++) {
                if (students[i].getRoll() == roll) {
                    students[i].showData(i);
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Student not found." << endl;
            }
        }
    }

    void updateData() {
        if (total == 0) {
            cout << "No data is entered." << endl;
        } else {
            string roll;
            cout << "Enter Roll No to update: ";
            cin >> roll;
            bool found = false;

            for (int i = 0; i < total; i++) {
                if (students[i].getRoll() == roll) {
                    cout << "\nPrevious data:";
                    students[i].showData(i);
                    students[i].updateData();
                    cout << "Record Updated!" << endl;
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Student not found." << endl;
            }
        }
    }

    void deleteData() {
        if (total == 0) {
            cout << "No data is entered." << endl;
        } else {
            cout << "Press 1 to delete all records" << endl;
            cout << "Press 2 to delete a specific record" << endl;
            int ch;
            cin >> ch;

            if (ch == 1) {
                total = 0;
                cout << "All records deleted!" << endl;
            } else if (ch == 2) {
                string roll;
                cout << "Enter Roll No to delete: ";
                cin >> roll;
                bool found = false;

                for (int i = 0; i < total; i++) {
                    if (students[i].getRoll() == roll) {
                        for (int j = i; j < total - 1; j++) {
                            students[j] = students[j + 1];
                        }
                        total--;
                        cout << "Record deleted!" << endl;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    cout << "Student not found." << endl;
                }
            } else {
                cout << "Invalid input!" << endl;
            }
        }
    }
};

int main() {
    ManagementSystem obj;
    int choice;
    bool running = true;

    while (running) {
        cout << "\nPress 1 to Enter Data" << endl;
        cout << "Press 2 to Show Data" << endl;
        cout << "Press 3 to Search Data" << endl;
        cout << "Press 4 to Update Data" << endl;
        cout << "Press 5 to Delete Data" << endl;
        cout << "Press 6 to Exit" << endl;

        cin >> choice;

        switch (choice) {
        case 1: obj.enterData(); break;
        case 2: obj.showData(); break;
        case 3: obj.searchData(); break;
        case 4: obj.updateData(); break;
        case 5: obj.deleteData(); break;
        case 6: running = false; break;
        default: cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}
