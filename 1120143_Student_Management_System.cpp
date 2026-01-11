// Student Management System

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

//PASSWORD
string hashPassword(string pass) {
    int hash = 0;
    for(char c:pass) {
        hash=hash+(c*2);
    }
    return to_string(hash);
}

class User{
    public:
        virtual void menu() = 0;
        virtual ~User() {}
};

class Student: public User{
    private:
        int id;
        string name,dept;
        float ct[3];
        float assignment;
        float attendance;
        string teacherUsername;

    public:
        Student():id(0),assignment(0),attendance(0){ct[0]=ct[1]=ct[2]=0;} //Constructor

        Student(int i, string n, string d, float c1, float c2, float c3, float a, float att, string t)
            : id(i), name(n), dept(d), assignment(a), attendance(att), teacherUsername(t){
            ct[0]=c1; ct[1]=c2; ct[2]=c3;
    }

        void menu() override {}

        int getID() const { return id; }
        string getTeacherUsername() const { return teacherUsername; }

        void updateCT(int index, float value) { if(index>=1 && index<=3) ct[index-1]=value; }
        void updateAssignment(float value) { assignment=value; }
        void updateAttendance(float value) { attendance=value; }

        void display() const{
            cout << "\nID: " << id
                << "\nName: " << name
                 << "\nDepartment: " << dept
                 << "\nCT Marks: " << ct[0] << " " << ct[1] << " " << ct[2]
                 << "\nAssignment: " << assignment
                 << "\nAttendance: " << attendance << "\n";
    }

    string toFile() const{
        return to_string(id) + "|" + name + "|" + dept + "|" +
               to_string(ct[0]) + "|" + to_string(ct[1]) + "|" + to_string(ct[2]) + "|" +
               to_string(assignment) + "|" + to_string(attendance) + "|" + teacherUsername;
    }

    static bool fromFile(const string &line, Student &s){
        if(line.empty()) return false;
        stringstream ss(line);
        string i,n,d,c1,c2,c3,a,att,t;
        if(!getline(ss,i,'|')) return false;
        if(!getline(ss,n,'|')) return false;
        if(!getline(ss,d,'|')) return false;
        if(!getline(ss,c1,'|')) return false;
        if(!getline(ss,c2,'|')) return false;
        if(!getline(ss,c3,'|')) return false;
        if(!getline(ss,a,'|')) return false;
        if(!getline(ss,att,'|')) return false;
        if(!getline(ss,t,'|')) return false;
        try { s=Student(stoi(i),n,d,stof(c1),stof(c2),stof(c3),stof(a),stof(att),t); }
        catch(...) { return false; }
        return true;
    }
};

class Teacher : public User {
    private:
        string username, hashedPass;
    public:
        Teacher() {}
        Teacher(string u, string h) : username(u), hashedPass(h){}
        void menu() override{}
        string getUsername() const {return username;}
        bool login(string u, string p) const { return (username==u && hashedPass==hashPassword(p));}
        string toFile() const {return username+"|"+hashedPass;}
};


class StudentManagement{
    private:
        vector<Student> students;
        string levelTermFile;
    public:
        void setLevelTerm(int level, int term){
            levelTermFile="students_level"+to_string(level)+"_term"+to_string(term)+".txt";
            load();
        }

        void load(){
            students.clear();
            ifstream file(levelTermFile);
            string line;
            while(getline(file,line)){
                Student s;
                if(Student::fromFile(line,s)) students.push_back(s);
            }
            file.close();
        }

        void save(){
            ofstream file(levelTermFile);
            for(auto &s: students) file << s.toFile() << endl;
            file.close();
        }

        void addStudent(const string &teacherUsername){
            int id;
            string n,d;
            float c1,c2,c3,a,att;
            cout << "ID: "; cin >> id; cin.ignore();
            cout << "Name: "; getline(cin,n);
            cout << "Department: "; getline(cin,d);
            cout << "Enter 3 CT marks(1st-2nd-3rd): "; cin >> c1 >> c2 >> c3;
            cout << "Assignment Mark: "; cin >> a;
            cout << "Attendance Mark: "; cin >> att;
            students.push_back(Student(id,n,d,c1,c2,c3,a,att,teacherUsername));
            save();
            cout << "Student Added Successfully!\n";
        }

        void displayMyStudents(const string &teacherUsername){
            bool found=false;
            for(auto &s: students) if(s.getTeacherUsername()==teacherUsername){ s.display(); found=true;}
            if(!found) cout << "\nNo students assigned yet.\n";
        }

        void updateStudent(const string &teacherUsername){
            int id;
            cout << "Enter ID to Update: "; cin >> id;
            for(auto &s: students){
                if(s.getID()==id && s.getTeacherUsername()==teacherUsername){
                    int choice;
                    cout << "\nWhat to update?\n1. CT1\n2. CT2\n3. CT3\n4. Assignment\n5. Attendance\nChoice: ";
                    cin >> choice;
                    float val;
                    cout << "Enter new value: "; cin >> val;
                    switch(choice){
                        case 1: s.updateCT(1,val); break;
                        case 2: s.updateCT(2,val); break;
                        case 3: s.updateCT(3,val); break;
                        case 4: s.updateAssignment(val); break;
                        case 5: s.updateAttendance(val); break;
                        default: cout << "Invalid!\n"; return;
                    }
                    save();
                    cout << "Updated Successfully!\n";
                    return;
                }
            }
            cout << "Student not found.\n";
        }

        void deleteStudent(const string &teacherUsername){
            int id;
            cout << "Enter ID to Delete: "; cin >> id;
            for(size_t i=0;i<students.size();i++){
                if(students[i].getID()==id && students[i].getTeacherUsername()==teacherUsername){
                    students.erase(students.begin()+i);
                    save();
                    cout << "Deleted Successfully!\n";
                    return;
                }
            }
            cout << "Student not found.\n";
        }

        void searchStudent(){
            int id;
            cout << "Enter ID to search: "; cin >> id;
            for(auto &s: students){
                if(s.getID()==id){ s.display(); return;}
            }
            cout << "No student found with this ID\n";
        }
    };


    bool isTeacherUsernameExists(const string &username) {
        ifstream file("teachers.txt");
        string line;
        while(getline(file,line)){
            stringstream ss(line);
            string uname, thash;
            getline(ss, uname,'|');
            if(uname == username) return true;
        }
        return false;
    }

    void registerTeacher(){
        string username, pass;
        cout << "\n--- Teacher Registration ---\nUsername: "; cin >> username;

        if(isTeacherUsernameExists(username)){
            cout << "Username already exists! Registration Failed.\n";
            return;
        }

        cout << "Password: "; cin >> pass;
        ofstream file("teachers.txt", ios::app);
        file << username << "|" << hashPassword(pass) << endl;
        file.close();
        cout << "Registration Successful!\n";
    }

    bool loginTeacher(Teacher &t){
        ifstream file("teachers.txt");
        string line, username, pass;
        cout << "\n--- Teacher Login ---\nUsername: "; cin >> username;
        cout << "Password: "; cin >> pass;
        while(getline(file,line)){
            stringstream ss(line);
            string uname, thash;
            getline(ss,uname,'|'); getline(ss,thash,'|');
            Teacher temp(uname,thash);
            if(temp.login(username,pass)){ t=temp; return true; }
        }
        return false;
    }

    void forgotPassword(){
        string username;
        cout << "\n--- Forgot Password ---\nEnter Username: "; cin >> username;
        if(!isTeacherUsernameExists(username)){ cout << "Username not found!\n"; return; }

        string newPass;
        cout << "Enter New Password: "; cin >> newPass;

        // read all teachers
        vector<pair<string,string>> teachers;
        ifstream file("teachers.txt");
        string line;
        while(getline(file,line)){
            stringstream ss(line);
            string uname, thash;
            getline(ss,uname,'|'); getline(ss,thash,'|');
            if(uname==username) thash=hashPassword(newPass);
            teachers.push_back({uname, thash});
        }
        file.close();


        ofstream out("teachers.txt");
        for(auto &p: teachers) out << p.first << "|" << p.second << endl;
        out.close();
        cout << "Password Reset Successfully!\n";
    }


int main(){
    int mainChoice;
    do{
        cout << "\n===== MAIN MENU =====\n";
        cout << "1. Teacher Registration\n2. Teacher Login\n3. Forgot Password\n4. Exit\nChoice: ";
        cin >> mainChoice;

        if(mainChoice==1) registerTeacher();
        else if(mainChoice==2){
        Teacher teacher;
        if(!loginTeacher(teacher)){ cout << "Login Failed!\n"; continue; }

        cout << "\n===== Enter Level/Term =====\n";
        int level, term;
        cout << "\nEnter Level: "; cin >> level;
        cout << "Enter Term: "; cin >> term;

        StudentManagement sm;
        sm.setLevelTerm(level,term);

        int choice;
        do{
        cout << "\n===== Student Management Portal (Level " << level << " Term " << term << ") =====\n";
        cout << "1. Add Student\n2. View My Students\n3. Update Marks\n4. Delete Student\n5. Search Student\n6. Back to Level\n7. Logout\nChoice: ";
        cin >> choice;
        switch(choice){
            case 1: sm.addStudent(teacher.getUsername()); break;
            case 2: sm.displayMyStudents(teacher.getUsername()); break;
            case 3: sm.updateStudent(teacher.getUsername()); break;
            case 4: sm.deleteStudent(teacher.getUsername()); break;
            case 5: sm.searchStudent(); break;
            case 6:
                    cout << "\nEnter Level: "; cin >> level;
                    cout << "Enter Term: "; cin >> term;
                    sm.setLevelTerm(level,term);
                    break;
            case 7: goto LOGOUT;
                    default: cout << "Invalid choice!\n";
                    }
                } while(true);
    LOGOUT:
                cout << "Logged out successfully.\n";
            }
            else if(mainChoice==3) forgotPassword();
            else if(mainChoice==4) cout << "Program terminated.\n";
            else cout << "Invalid option!\n";
        }
        while(mainChoice!=4);
    return 0;
}

