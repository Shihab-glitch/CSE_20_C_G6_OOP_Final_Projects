// Flight Reservation System

#include <bits/stdc++.h>
using namespace std;
class FlightBase{
protected:
    string flightNo, departure, destination;


public:
    FlightBase(string fn, string dep, string des){
        flightNo = fn;
        departure = dep;
        destination = des;
    }

    void showBasicInfo() {
        cout << "Flight No: " << flightNo << endl;
        cout << "Route: " << departure << " -> " << destination << endl;
    }
};
class Flight : public FlightBase {
private:
    int rows;
    map<string, bool>seatBooked;
public:
    Flight(string fn, string dep, string des, int r): FlightBase(fn, dep, des){
        rows = r;

        for (int i = 1; i <= rows; i++) {
            seatBooked[to_string(i) + "A"] = false;
            seatBooked[to_string(i) + "B"] = false;
            seatBooked[to_string(i) + "C"] = false;
            seatBooked[to_string(i) + "D"] = false;
            seatBooked[to_string(i) + "E"] = false;
        }
    }

    bool reserveSeat(string seat){
        if (seatBooked.find(seat)==seatBooked.end()){
            return false;
        }
        if (seatBooked[seat]) {
            return false;
        }
        seatBooked[seat]=true;
        return true;
    }

    bool cancelSeat(string seat){
        if (seatBooked.find(seat)==seatBooked.end()){
            return false;
        }
        if (!seatBooked[seat]){
            return false;
        }
        seatBooked[seat] = false;
        return true;
    }
    void showSeatMap(){
        cout << "\nSeat Layout (X = Booked)\n";
        cout << " A     B  C     D  E\n";

        for (int i = 1; i <= rows; i++) {
            string a = to_string(i) + "A";
            string b = to_string(i) + "B";
            string c = to_string(i) + "C";
            string d = to_string(i) + "D";
            string e = to_string(i) + "E";

            cout<<setw(4)<< (seatBooked[a]? "X": a)<< " ";
            cout<<setw(4)<< (seatBooked[b]? "X": b)<< " ";
            cout<<setw(4)<< (seatBooked[c]? "X": c)<< " ";
            cout<<setw(4)<< (seatBooked[d]? "X": d)<< "  ";
            cout<<setw(4)<< (seatBooked[e]? "X": e)<< endl;
        }
    }

    friend class ReservationSystem;
};
class ReservationSystem {
private:
    vector<Flight> flights;

public:
    void addFlight(string fn, string dep, string des, int rows) {
        flights.push_back(Flight(fn, dep, des, rows));
    }
    void showFlights() {
        cout << "\n========= AVAILABLE FLIGHTS =====\n";
        for (auto &f : flights) {

            int booked = 0;
            for (auto &p : f.seatBooked) {
                if (p.second) booked++;
            }
            int totalSeats = f.rows * 5;
            int available = totalSeats - booked;

            cout << "Flight No: " << f.flightNo << endl;
            cout << "Route: " << f.departure << " -> " << f.destination << endl;
            cout << "Rows: " << f.rows << endl;
            cout << "Total Seats: " << totalSeats << endl;
            cout << "Booked Seats: " << booked << endl;
            cout << "Available Seats: " << available << endl;
            cout << "-----------------------------\n";
        }
    }

    void reserveSeat() {
        string flightNumber;
        cout << "\nEnter Flight Number:";
        cin >> flightNumber;

        for (auto &f : flights) {
            if (f.flightNo == flightNumber) {
                f.showSeatMap();

                string name, id, seat;
                cout << "\nEnter Passenger Name:";
                cin.ignore();
                getline(cin, name);

                cout << "Enter Passenger ID: ";
                cin >> id;

                cout << "Enter Seat Number (Example: 12B): ";
                cin >> seat;

                if (f.reserveSeat(seat)) {
                    char col = seat.back();
                    string seatType = (col == 'A' || col == 'E') ? "Window" : "Middle";

                    cout << "\n===== TICKET CONFIRMATION =====\n";
                    cout << "Airline: Fly BD Airlines\n";
                    cout << "----------------------------------\n";
                    cout << "Passenger: " << name << endl;
                    cout << "Passenger ID: " << id << endl;
                    cout << "Flight No: " << f.flightNo << endl;
                    cout << "Route: " << f.departure << " -> " << f.destination << endl;
                    cout << "Seat No: " << seat << endl;
                    cout << "Seat Type: " << seatType << endl;
                    cout << "----------------------------------\n";
                    cout << "Status: CONFIRMED\n";
                    cout << "Check-in Time: Please Check-in 2 hours before Departure\n";
                    cout << "================================================\n";
                    cout << " Thank You For Choosing Fly BD Airline\n";
                    cout << "Have a pleasant journey\n";
                    cout << "================================================\n";
                } else {
                    cout << "\nSorry! Seat not available or invalid.\n";
                }
                return;
            }
        }
        cout << "\nNo flight found.\n";
    }

    void cancelSeat() {
        string flightNumber, seat;
        cout << "\nEnter Flight Number: ";
        cin >> flightNumber;

        for (auto &f : flights) {
            if (f.flightNo == flightNumber) {
                f.showSeatMap();

                cout << "\nEnter Seat Number to Cancel: ";
                cin >> seat;

                if (f.cancelSeat(seat)) {
                    cout << "\nSeat " << seat << " has been successfully cancelled.\n";
                } else {
                    cout << "\nSeat cancellation failed! Seat invalid or not booked.\n";
                }
                return;
            }
        }
        cout << "\nNo flight found.\n";
    }
};

int main() {
    ReservationSystem system;

    system.addFlight("F101", "Dhaka", "Dubai", 10);
    system.addFlight("F201", "London", "Toronto", 20);
    system.addFlight("F301", "Dhaka", "Kuala Lumpur", 15);

    cout << "====================================\n";
    cout << "Welcome to Fly BD Airline System\n";
    cout << "====================================\n";
    while (true) {
        int choice;
        cout << "\n1. View Flights";
        cout << "\n2. Reserve Seat";
        cout << "\n3. Cancel Seat";
        cout << "\n4. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        if (choice == 1)
            system.showFlights();
        else if (choice == 2)
            system.reserveSeat();
        else if (choice == 3)
            system.cancelSeat();
        else if (choice == 4) {
            cout << "\nThank you for using Fly BD Airline System.\n";
            break;
        } else
            cout << "\nInvalid choice! Try again.\n";
    }
    return 0;
}
