// Hotel Booking System

#include <iostream>
#include <string>

using namespace std;

// ================= ABSTRACT CLASS =================
class Person {
protected:
    string username;
    string password;

public:
    virtual void dashboard() = 0; // abstraction
};

// ================= HOTEL CLASS ====================
class Hotel {
private:
    string name;
    string package;

public:
    void setHotel(string n, string p) {
        name = n;
        package = p;
    }

    string getName() {
        return name;
    }

    string getPackage() {
        return package;
    }
};

// ================= ADMIN CLASS ====================
class Admin : public Person {
private:
    Hotel hotels[10];
    int hotelCount = 0;

public:
    bool login() {
        cout << "\n--- ADMIN LOGIN ---\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (username == "admin" && password == "123") {
            cout << "Admin login successful!\n";
            return true;
        }
        cout << "Invalid admin credentials!\n";
        return false;
    }

    void addHotel() {
        if (hotelCount < 10) {
            string n, p;
            cout << "Hotel name: ";
            cin >> n;
            cout << "Package (low/medium/high): ";
            cin >> p;

            hotels[hotelCount].setHotel(n, p);
            hotelCount++;
            cout << "Hotel added successfully!\n";
        }
    }

    void viewHotels() {
        if (hotelCount == 0) {
            cout << "No hotels available.\n";
            return;
        }

        cout << "\n--- HOTEL LIST ---\n";
        for (int i = 0; i < hotelCount; i++) {
            cout << i + 1 << ". "
                 << hotels[i].getName()
                 << " (" << hotels[i].getPackage() << ")\n";
        }
    }

    void updateHotel() {
        int index;
        string newName, newPackage;

        viewHotels();
        cout << "Select hotel number to update: ";
        cin >> index;

        if (index > 0 && index <= hotelCount) {
            cout << "New hotel name: ";
            cin >> newName;
            cout << "New package: ";
            cin >> newPackage;

            hotels[index - 1].setHotel(newName, newPackage);
            cout << "Hotel updated successfully!\n";
        } else {
            cout << "Invalid selection!\n";
        }
    }

    void deleteHotel() {
        int index;
        viewHotels();
        cout << "Select hotel number to delete: ";
        cin >> index;

        if (index > 0 && index <= hotelCount) {
            for (int i = index - 1; i < hotelCount - 1; i++) {
                hotels[i] = hotels[i + 1];
            }
            hotelCount--;
            cout << "Hotel deleted successfully!\n";
        }
        else{
            cout<<"No Hotel available to delete"<<endl;
        }
    }

    void dashboard() {
        int choice;
        do {
            cout << "\n--- ADMIN DASHBOARD ---\n";
            cout << "1. Add Hotel\n";
            cout << "2. View Hotels\n";
            cout << "3. Update Hotel\n";
            cout << "4. Delete Hotel\n";
            cout << "5. Logout\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1) addHotel();
            else if (choice == 2) viewHotels();
            else if (choice == 3) updateHotel();
            else if (choice == 4) deleteHotel();

        } while (choice != 5); // Logout returns to home
    }

    Hotel* getHotels() {
        return hotels;
    }

    int getHotelCount() {
        return hotelCount;
    }
};

// ================= USER CLASS ====================
class User : public Person {
private:
    string bookedHotel;

public:
    void signup() {
        cout << "\n--- USER SIGNUP ---\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        cout << "Signup successful!\n";
    }

    bool login() {
        string u, p;
        cout << "\n--- USER LOGIN ---\n";
        cout << "Username: ";
        cin >> u;
        cout << "Password: ";
        cin >> p;

        if (u == username && p == password) {
            cout << "Login successful!\n";
            return true;
        }
        cout << "Invalid credentials!\n";
        return false;
    }

    void viewPackages(Hotel hotels[], int count) {
        cout << "\n--- AVAILABLE HOTELS ---\n";
        for (int i = 0; i < count; i++) {
            cout << i + 1 << ". "
                 << hotels[i].getName()
                 << " (" << hotels[i].getPackage() << ")\n";
        }
    }

    void bookHotel(Hotel hotels[], int count) {
        int choice;
        viewPackages(hotels, count);
        cout << "Select hotel number: ";
        cin >> choice;

        if (choice > 0 && choice <= count) {
            bookedHotel = hotels[choice - 1].getName();
            cout << "Hotel booked successfully!\n";
        }
    }

    void viewBooking() {
        if (bookedHotel != "")
            cout << "Booked Hotel: " << bookedHotel << endl;
        else
            cout << "No booking found.\n";
    }

    void dashboard() {} // required override
};

// ================= MAIN FUNCTION ====================
int main() {
    Admin admin;
    User user; 

    int choice;

    // 🔁 HOME PAGE LOOP (WEB-LIKE)
    while (true) {
        cout << "\n=== HOTEL BOOKING SYSTEM ===\n";
        cout << "1. User Signup\n";
        cout << "2. User Login\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n";
        cout << "Choose option: ";
        cin >> choice;

        if (choice == 1) {
            user.signup();
        }

        else if (choice == 2) {
            if (user.login()) {
                int c;
                do {
                    cout << "\n--- USER DASHBOARD ---\n";
                    cout << "1. View Packages\n";
                    cout << "2. Book Hotel\n";
                    cout << "3. View Booking\n";
                    cout << "4. Logout\n";
                    cin >> c;

                    if (c == 1)
                        user.viewPackages(admin.getHotels(), admin.getHotelCount());
                    else if (c == 2)
                        user.bookHotel(admin.getHotels(), admin.getHotelCount());
                    else if (c == 3)
                        user.viewBooking();

                } while (c != 4); // logout → back to home
            }
        }

        else if (choice == 3) {
            if (admin.login()) {
                admin.dashboard(); // logout → back to home
            }
        }

        else if (choice == 4) {
            cout << "Thank you for using the system!\n";
            break; // ONLY place program exits
        }
    }

    return 0;
}
