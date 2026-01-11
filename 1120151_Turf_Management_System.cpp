// Turf Management System

#include <bits/stdc++.h>
using namespace std;

class Turf {
protected:
    string name;
    string status;
    string bookingBits;

public:
    Turf(string n, string s, string bits)
        : name(n), status(s), bookingBits(bits) {}

    virtual~Turf() {}

    string getName() const { return name; }
    string getStatus() const { return status; }
    string getBits() const { return bookingBits; }

    void setStatus(const string &s) { status = s; }

    bool isAvailable(int from, int to) const {
        if (status != "Open") return false;
        for (int i = from; i < to; i++)
            if (bookingBits[i] == '1') return false;
        return true;
    }

    void bookHours(int from, int to) {
        for (int i = from; i < to; i++)
            bookingBits[i] = '1';
    }

    virtual string getType() const = 0;

    virtual void showTurf() const {
        cout << "Type   : " << getType() << endl;
        cout << "Name   : " << name << endl;
        cout << "Status : " << status << endl;
        cout << "Booked : " << bookingBits << endl;
    }

    virtual void save(ofstream &fout, const string &division) const {
        fout << getType() << "|" << division << "|"
             << name << "|" << status << "|" << bookingBits << "\n";
    }
};

class FootballTurf : public Turf {
public:
    FootballTurf(string n, string s, string bits)
        : Turf(n, s, bits) {}

    string getType() const override {
        return "Football";
    }

    void showTurf() const override {
        cout << "[Football Turf]\n";
        Turf::showTurf();
    }
};

class CricketTurf : public Turf {
public:
    CricketTurf(string n, string s, string bits)
        : Turf(n, s, bits) {}

    string getType() const override {
        return "Cricket";
    }

    void showTurf() const override {
        cout << "[Cricket Turf]\n";
        Turf::showTurf();
    }
};

class Division {
private:
    string name;
    vector<Turf*> turfs;

public:
    Division(string n) : name(n) {}

    string getName() const { return name; }

    void addTurf(Turf* t) {
        turfs.push_back(t);
    }

    void showTurfs() const {
        if (turfs.empty()) {
            cout << "No turfs in " << name << endl;
            return;
        }
        for (size_t i = 0; i < turfs.size(); i++) {
            cout << "\nTurf " << i + 1 << ":\n";
            turfs[i]->showTurf();
        }
    }

    void bookTurf() {
        int from, to;
        cout << "From hour (0-23): ";
        cin >> from;
        cout << "To hour (1-24): ";
        cin >> to;

        bool found = false;
        for (size_t i = 0; i < turfs.size(); i++) {
            if (turfs[i]->isAvailable(from, to)) {
                cout << "\nAvailable Turf " << i + 1 << ":\n";
                turfs[i]->showTurf();
                found = true;
            }
        }

        if (!found) {
            cout << "No turf available!\n";
            return;
        }

        cout << "Choose turf number: ";
        int ch;
        cin >> ch;

        if (ch < 1 || ch > (int)turfs.size()) {
            cout << "Invalid choice!\n";
            return;
        }

        if (!turfs[ch - 1]->isAvailable(from, to)) {
            cout << "Turf not available!\n";
            return;
        }

        turfs[ch - 1]->bookHours(from, to);
        cout << "Booking successful!\n";
    }

    void updateStatus() {
        showTurfs();
        cout << "Choose turf number: ";
        int ch;
        cin >> ch;
        if (ch < 1 || ch > (int)turfs.size()) return;

        string s;
        cout << "Enter new status (Open/Closed): ";
        cin >> s;
        for (char &c : s) c = tolower(c);

        if (s == "open") s = "Open";
        else if (s == "closed" || s == "close") s = "Closed";
        else {
            cout << "Invalid status!\n";
            return;
        }

        turfs[ch - 1]->setStatus(s);
        cout << "Status updated!\n";
    }

    void saveDivision(ofstream &fout) const {
        for (auto t : turfs)
            t->save(fout, name);
    }
};

void saveAll(vector<Division> &divs) {
    ofstream fout("turfs.txt");
    for (auto &d : divs)
        d.saveDivision(fout);
    fout.close();
}

void loadAll(vector<Division> &divs) {
    ifstream fin("turfs.txt");
    if (!fin) return;

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string type, division, name, status, bits;

        getline(ss, type, '|');
        getline(ss, division, '|');
        getline(ss, name, '|');
        getline(ss, status, '|');
        getline(ss, bits, '|');

        Turf* t = nullptr;
        if (type == "Football")
            t = new FootballTurf(name, status, bits);
        else if (type == "Cricket")
            t = new CricketTurf(name, status, bits);

        for (auto &d : divs) {
            if (d.getName() == division) {
                d.addTurf(t);
                break;
            }
        }
    }
    fin.close();
}

bool hasAlphabet(const string &s) {
    for (char c : s)
        if (isalpha(c)) return true;
    return false;
}

int main() {
    vector<string> divNames = {
        "Dhaka", "Chattogram", "Rajshahi", "Khulna",
        "Barishal", "Sylhet", "Rangpur", "Mymensingh"
    };

    vector<Division> divisions;
    for (auto &n : divNames)
        divisions.emplace_back(n);

    loadAll(divisions);

    while (true) {
        cout << "\n--- Turf Management System ---\n";
        cout << "1. Add Turf\n";
        cout << "2. View Turfs\n";
        cout << "3. Book Turf\n";
        cout << "4. Update Turf Status\n";
        cout << "5. Save & Exit\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 5) {
            saveAll(divisions);
            cout << "Data saved. Exiting...\n";
            break;
        }

        cout << "Select Division:\n";
        for (int i = 0; i < 8; i++)
            cout << i + 1 << ". " << divNames[i] << endl;

        int d;
        cin >> d;
        d--;
        if (d < 0 || d >= 8) continue;

        if (choice == 1) {
            int type;
            cout << "1. Football Turf\n2. Cricket Turf\n";
            cin >> type;
            cin.ignore();

            string name;
            cout << "Enter Turf Name: ";
            getline(cin, name);

            if (!hasAlphabet(name)) {
                cout << "Invalid turf name!\n";
                continue;
            }

            string status;
            cout << "Enter Status (Open/Closed): ";
            cin >> status;
            for (char &c : status) c = tolower(c);

            if (status == "open") status = "Open";
            else if (status == "closed" || status == "close") status = "Closed";
            else {
                cout << "Invalid status!\n";
                continue;
            }

            string bits(24, '0');
            Turf* t = (type == 1)
                ? (Turf*)new FootballTurf(name, status, bits)
                : (Turf*)new CricketTurf(name, status, bits);

            divisions[d].addTurf(t);
            cout << "Turf added successfully!\n";
        }
        else if (choice == 2) divisions[d].showTurfs();
        else if (choice == 3) divisions[d].bookTurf();
        else if (choice == 4) divisions[d].updateStatus();
    }

    return 0;
}
