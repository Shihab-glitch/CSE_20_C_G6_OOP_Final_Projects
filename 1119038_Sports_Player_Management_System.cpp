// Sports Player Management System

#include <iostream>
#include <cstring>

using namespace std;

/* -------- Abstract Base Class -------- */
class User
{
protected:
    char username[20];
    char password[20];

public:
    virtual void showProfile() = 0; // Abstraction
};

/* -------- Player Class -------- */
class Player : public User
{
private:
    int id;
    int age;
    char sport[20];
    int isRegistered;

public:
    friend class Admin; // Friend class

    // Constructor (Fix garbage values)
    Player()
    {
        id = 0;
        age = 0;
        isRegistered = 0;
        strcpy(username, "");
        strcpy(password, "");
        strcpy(sport, "");
    }

    void registerPlayer()
    {
        cout << "Enter Player ID: ";
        cin >> id;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;
        cout << "Enter Sport: ";
        cin >> sport;
        cout << "Enter Age: ";
        cin >> age;

        isRegistered = 1;
        cout << "Player Registered Successfully\n";
    }

    int login(char u[], char p[])
    {
        if (isRegistered == 0)
        {
            cout << "No player registered yet.\n";
            return 0;
        }

        if (strcmp(username, u) == 0 && strcmp(password, p) == 0)
            return 1;

        return 0;
    }

    void updateProfile()
    {
        if (isRegistered == 0)
        {
            cout << "No profile to update.\n";
            return;
        }

        cout << "Enter New Sport: ";
        cin >> sport;
        cout << "Enter New Age: ";
        cin >> age;

        cout << "Profile Updated Successfully\n";
    }

    void showProfile() // Polymorphism
    {
        if (isRegistered == 0)
        {
            cout << "No player registered yet.\n";
            return;
        }

        cout << "\n----- Player Profile -----\n";
        cout << "ID: " << id << endl;
        cout << "Username: " << username << endl;
        cout << "Sport: " << sport << endl;
        cout << "Age: " << age << endl;
    }
};

/* -------- Admin Class -------- */
class Admin : public User
{
public:
    Admin()
    {
        strcpy(username, "admin");
        strcpy(password, "admin123");
    }

    int login(char u[], char p[])
    {
        if (strcmp(username, u) == 0 && strcmp(password, p) == 0)
            return 1;
        return 0;
    }

    void showProfile()
    {
        cout << "\nAdmin Logged In Successfully\n";
    }

    void viewPlayer(Player &p)
    {
        p.showProfile();
    }

    void deletePlayer(Player &p)
    {
        if (p.isRegistered == 0)
        {
            cout << "No player to delete.\n";
            return;
        }

        p.id = 0;
        p.age = 0;
        p.isRegistered = 0;
        strcpy(p.username, "");
        strcpy(p.password, "");
        strcpy(p.sport, "");

        cout << "Player Deleted Successfully\n";
    }
};

/* -------- Main Function -------- */
int main()
{
    Player player;
    Admin admin;

    int choice;
    char u[20], p[20];

    while (1)
    {
        cout << "\n=== Sports Player Management System ===\n";
        cout << "1. Player Registration\n";
        cout << "2. Player Login\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            player.registerPlayer();
        }
        else if (choice == 2)
        {
            cout << "Enter Username: ";
            cin >> u;
            cout << "Enter Password: ";
            cin >> p;

            if (player.login(u, p))
            {
                int pc;
                while (1)
                {
                    cout << "\n1. View Profile\n";
                    cout << "2. Update Profile\n";
                    cout << "3. Logout\n";
                    cout << "Enter Choice: ";
                    cin >> pc;

                    if (pc == 1)
                        player.showProfile();
                    else if (pc == 2)
                        player.updateProfile();
                    else
                        break;
                }
            }
            else
            {
                cout << "Invalid Login\n";
            }
        }
        else if (choice == 3)
        {
            cout << "Enter Admin Username: ";
            cin >> u;
            cout << "Enter Admin Password: ";
            cin >> p;

            if (admin.login(u, p))
            {
                int ac;
                while (1)
                {
                    cout << "\n1. View Player\n";
                    cout << "2. Delete Player\n";
                    cout << "3. Logout\n";
                    cout << "Enter Choice: ";
                    cin >> ac;

                    if (ac == 1)
                        admin.viewPlayer(player);
                    else if (ac == 2)
                        admin.deletePlayer(player);
                    else
                        break;
                }
            }
            else
            {
                cout << "Invalid Admin Login\n";
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}
