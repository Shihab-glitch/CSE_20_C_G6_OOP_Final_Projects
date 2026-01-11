// Airline Management System

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

/* ================= BASE CLASS ================= */
class Person {
protected:
    string name;
    string passport;

public:
    void inputPerson() {
        cout << "Name: ";
        cin >> name;
        cout << "Passport No: ";
        cin >> passport;
    }

    string getPassport() {
        return passport;
    }
};

/* ================= PASSENGER ================= */
class Passenger : public Person {
    string flightNo;
    int seats;

public:
    void inputPassenger() {
        inputPerson();
        cout << "Flight No: ";
        cin >> flightNo;
        cout << "Seats: ";
        cin >> seats;
    }

    string getFlight() { return flightNo; }
    int getSeats() { return seats; }

    void save() {
        ofstream out("Passenger.txt", ios::app);
        out << name << " " << passport << " "
            << flightNo << " " << seats << endl;
        out.close();
    }
};

/* ================= FLIGHT ================= */
class Flight {
    string flightNo, from, to;
    int seats;

public:
    Flight() {}

    Flight(string f, string fr, string t, int s) {
        flightNo = f;
        from = fr;
        to = t;
        seats = s;
    }

    string getFlightNo() { return flightNo; }
    int getSeats() { return seats; }

    void updateSeats(int s) {
        seats = s;
    }

    void save() {
        ofstream out("Flight.txt", ios::app);
        out << flightNo << " " << from
            << " " << to << " " << seats << endl;
        out.close();
    }

    static void display() {
        ifstream in("Flight.txt");
        string f, fr, t;
        int s;

        cout << "\nFlight  From  To  Seats\n";
        cout << "------------------------\n";
        while (in >> f >> fr >> t >> s) {
            cout << f << "  " << fr << "  "
                 << t << "  " << s << endl;
        }
        in.close();
    }
};

/* ================= AIRLINE SYSTEM ================= */
class AirlineSystem {
public:
    void addFlight() {
        string f, fr, t;
        int s;

        cout << "Flight No: ";
        cin >> f;
        cout << "From: ";
        cin >> fr;
        cout << "To: ";
        cin >> t;
        cout << "Seats: ";
        cin >> s;

        Flight fl(f, fr, t, s);
        fl.save();
        cout << "Flight Added Successfully!\n";
    }

    void bookSeat() {
        Passenger p;
        p.inputPassenger();

        ifstream in("Flight.txt");
        ofstream out("Temp.txt");

        string f, fr, t;
        int s;
        bool booked = false;

        while (in >> f >> fr >> t >> s) {
            if (f == p.getFlight() && s >= p.getSeats()) {
                s -= p.getSeats();
                booked = true;
                p.save();
            }
            out << f << " " << fr << " " << t << " " << s << endl;
        }

        in.close();
        out.close();
        remove("Flight.txt");
        rename("Temp.txt", "Flight.txt");

        if (booked)
            cout << "Seat Booked Successfully!\n";
        else
            cout << "Booking Failed!\n";
    }

    void cancelBooking() {
        string pass;
        cout << "Enter Passport No: ";
        cin >> pass;

        ifstream in("Passenger.txt");
        ofstream out("TempP.txt");

        string n, p, f;
        int s;
        bool found = false;

        while (in >> n >> p >> f >> s) {
            if (p == pass) {
                found = true;
                updateFlightSeat(f, s);
            } else {
                out << n << " " << p << " " << f << " " << s << endl;
            }
        }

        in.close();
        out.close();
        remove("Passenger.txt");
        rename("TempP.txt", "Passenger.txt");

        if (found)
            cout << "Booking Cancelled!\n";
        else
            cout << "No Record Found!\n";
    }

private:
    void updateFlightSeat(string flightNo, int seats) {
        ifstream in("Flight.txt");
        ofstream out("TempF.txt");

        string f, fr, t;
        int s;

        while (in >> f >> fr >> t >> s) {
            if (f == flightNo)
                s += seats;
            out << f << " " << fr << " " << t << " " << s << endl;
        }

        in.close();
        out.close();
        remove("Flight.txt");
        rename("TempF.txt", "Flight.txt");
    }
};

/* ================= MAIN ================= */
int main() {
    AirlineSystem airline;
    int ch;

    while (true) {
        system("cls");

        cout << "\nAIRLINE MANAGEMENT SYSTEM\n";
        cout << "1.Add Flight\n";
        cout << "2.View Flights\n";
        cout << "3.Book Seat\n";
        cout << "4.Cancel Booking\n";
        cout << "5.Exit\n";
        cout << "Choice: ";
        cin >> ch;

        switch (ch) {
        case 1: airline.addFlight(); break;
        case 2: Flight::display(); break;
        case 3: airline.bookSeat(); break;
        case 4: airline.cancelBooking(); break;
        case 5: exit(0);
        default: cout << "Invalid Choice!\n";
        }
        Sleep(4000);
    }
}
