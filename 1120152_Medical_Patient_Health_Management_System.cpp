// Medical Patient Health Management System

#include<bits/stdc++.h>
using namespace std;
class person {
protected:
    string name;
    int age;
    string gender;
    string contact;

public:
    virtual void inputInfo() = 0;
    virtual void displayInfo() = 0;
};

class patient : public person {
private:
    string medicalHistory[100];
    int medHistcount = 0;

    string dietPlan[100];
    int dietcount = 0;

    string doctorHistory[100];
    int doctHistcount = 0;

    string medicines[100];
    int medcount = 0;

public:

    void setName(string newname){
        name = newname;
    }
    string getName(){
        return name;
    }

    void setAge(int newage){
        age = newage;
    }
    int getAge(){
        return age;
    }

    void setGender(string newgender){
        gender = newgender;
    }
    string getGender(){
        return gender;
    }

    void setContact(string newcontact){
        contact = newcontact;
    }
    string getContact(){
        return contact;
    }

    void inputInfo() {
        cout << "Enter name: ";
        getline(cin, name);

        cout << "Enter age: ";
        cin >> age;
        cin.ignore();

        cout << "Enter gender: ";
        getline(cin, gender);

        cout << "Enter contact: ";
        getline(cin, contact);
    }

    void displayInfo() {
        cout << "Patient Info:";
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Gender: " << gender << endl;
        cout << "Contact: " << contact << endl;
    }

    void addMedicalHistory(){
        string history;
        cout << "Enter medical history (type 'done' to stop):\n";
        while(true){
            getline(cin, history);
            if(history == "done") break;

            if(medHistcount < 100){
                medicalHistory[medHistcount] = history;
                medHistcount++;
            }
            else{
                cout << "Medical history full!"<<endl;
                break;
            }
        }
    }

    void showmedicalHistory() {
        cout << "Medical History:"<<endl;
        for(int i = 0; i < medHistcount; i++){
            cout << "- " << medicalHistory[i] << endl;
        }
    }

    void adddietPlan() {
        string diet;
        cout << "Enter diet plan (type 'done' to stop):\n";
        while(true){
            getline(cin, diet);
            if(diet == "done") break;

            if(dietcount < 100){
                dietPlan[dietcount] = diet;
                dietcount++;
            }
            else{
                cout << "Diet plan full!\n";
                break;
            }
        }
    }

    void showdietPlan() {
        cout << "Diet Plan:"<<endl;
        for(int i = 0; i < dietcount; i++){
            cout << "- " << dietPlan[i] << endl;
        }
    }


    void adddoctorHistory() {
        string doc;
        cout << "Enter doctor history (type 'done' to stop):\n";
        while(true){
            getline(cin, doc);
            if(doc == "done") break;

            if(doctHistcount < 100){
                doctorHistory[doctHistcount] = doc;
                doctHistcount++;
            }
            else{
                cout << "Doctor history full!"<<endl;
                break;
            }
        }
    }

    void showDoctorHistory() {
        cout << "\nDoctor History:"<<endl;
        for(int i = 0; i < doctHistcount; i++){
            cout << "- " << doctorHistory[i] << endl;
        }
    }

    // -------- Medicines --------
    void addmedicines() {
        string med;
        cout << "Enter medicines (type 'done' to stop):\n";
        while(true){
            getline(cin, med);
            if(med == "done") break;

            if(medcount < 100){
                medicines[medcount] = med;
                medcount++;
            }
            else{
                cout << "Medicine list full!"<<endl;
                break;
            }
        }
    }

    void showmedicines() {
        cout << "\nMedicines Reminder:"<<endl;
        for(int i = 0; i < medcount; i++){
            cout << "- " << medicines[i] << endl;
        }
    }


    void firstAid() {
        string symptom, severity;

        cout << "\nEnter symptom: ";
        getline(cin, symptom);

        cout << "Enter severity (mild/high/none): ";
        getline(cin, severity);

        cout << "First Aid Suggestion:\n";

        if(symptom == "fever"){
            if(severity == "high")
                cout << "Consult doctor immediately"<<endl;
            else
                cout << "Take paracetamol, rest, drink water"<<endl;
        }
        else if(symptom == "cut"){
            cout << "Clean wound, apply antiseptic, bandage"<<endl;
        }
        else if(symptom == "headache"){
            cout << "Rest and avoid screen"<<endl;
        }
        else if(symptom == "chest pain"){
            cout << "Lie down and check respiration"<<endl;
        }
        else if(symptom == "sudden chest pain"){
            cout << "Call emergency services immediately (999 or 112)\n";
        }
        else{
            cout << "Consult doctor if persists"<<endl;
        }
    }
};


class mobileuser {
public:
    virtual void sendmessage(string number) = 0;
};

class medicinereminder : public mobileuser {
public:
    void sendmessage(string number) {
        cout << "Sending message to number: " << number << endl;
        cout << "It's medicine time!\n";
    }
};


int main() {
    patient p;

    cout << "Patient Health Management System" << endl;

    p.inputInfo();

    p.addMedicalHistory();
    p.adddietPlan();
    p.adddoctorHistory();
    p.addmedicines();

    p.displayInfo();
    p.showmedicalHistory();
    p.showdietPlan();
    p.showDoctorHistory();
    p.showmedicines();

    p.firstAid();

    // Medicine Reminder
    string morningNumber, afternoonNumber;
    cout << "\nEnter mobile number for morning: ";
    cin >> morningNumber;
    cout << "Enter mobile number for afternoon: ";
    cin >> afternoonNumber;

    time_t now = time(0);
    tm *ltm = localtime(&now);
    int hour = ltm->tm_hour;

    mobileuser* user;
    medicinereminder reminder;
    user = &reminder;

    if (hour >= 6 && hour < 12) {
        cout << "\nMorning time detected!\n";
        user->sendmessage(morningNumber);
    }
    else if (hour >= 12 && hour < 18) {
        cout << "\nAfternoon time detected!\n";
        user->sendmessage(afternoonNumber);
    }
    else {
        cout << "\nIt's not morning or afternoon. No message sent.\n";
    }

    return 0;
}
