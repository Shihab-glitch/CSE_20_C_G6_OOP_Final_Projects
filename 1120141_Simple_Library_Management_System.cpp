// Simple Library Management System

#include <iostream>
#include <string>
using namespace std;

// Base Class
class User {
public:
    virtual void menu() = 0;   // Polymorphism (pure virtual function)
};

// Book Class
class Book {
private:
    int bookID;
    string title;
    string author;
    int quantity;

public:
    // Constructor
    Book() {
        bookID = 0;
        title = "";
        author = "";
        quantity = 0;
    }

    // Set book data
    void setBook() {
        cout << "Enter Book ID: ";
        cin >> bookID;
        cin.ignore();

        cout << "Enter Book Title: ";
        getline(cin, title);

        cout << "Enter Author Name: ";
        getline(cin, author);

        cout << "Enter Quantity: ";
        cin >> quantity;
    }

    // Display book data
    void displayBook() const {
        cout << "Book ID   : " << bookID << endl;
        cout << "Title     : " << title << endl;
        cout << "Author    : " << author << endl;
        cout << "Quantity  : " << quantity << endl;
    }

    int getID() const {
        return bookID;
    }
};

//  Library Class
class Library {
private:
    Book books[100];
    int count;

public:
    Library() {
        count = 0;
    }

    void addBook() {
        books[count].setBook();
        count++;
        cout << "Book Added Successfully!\n";
    }

    void displayBooks() const {
        if (count == 0) {
            cout << "No books available.\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            cout << "------------------------\n";
            books[i].displayBook();
        }
    }

    void searchBook() const {
        int id;
        cout << "Enter Book ID to search: ";
        cin >> id;

        for (int i = 0; i < count; i++) {
            if (books[i].getID() == id) {
                cout << "Book Found!\n";
                books[i].displayBook();
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void deleteBook() {
        int id;
        cout << "Enter Book ID to delete: ";
        cin >> id;

        for (int i = 0; i < count; i++) {
            if (books[i].getID() == id) {
                for (int j = i; j < count - 1; j++) {
                    books[j] = books[j + 1];
                }
                count--;
                cout << "Book Deleted Successfully!\n";
                return;
            }
        }
        cout << "Book not found.\n";
    }
};

//  Admin Class
class Admin : public User {
private:
    Library library;

public:
    void menu() override {   // Polymorphism
        int choice;
        do {
            cout << "\n Admin Me\n";
            cout << "1. Add Book\n";
            cout << "2. Display Books\n";
            cout << "3. Search Book\n";
            cout << "4. Delete Book\n";
            cout << "5. Exit\n";

            cout << "Enter Choice: ";
            cin >> choice;

            switch (choice) {
                case 1: library.addBook(); break;
                case 2: library.displayBooks(); break;
                case 3: library.searchBook(); break;
                case 4: library.deleteBook(); break;
                case 5: cout << "Exiting...\n"; break;
                default: cout << "Invalid choice!\n";
            }
        } while (choice != 5);
    }
};

//  Main Function
int main() {
    User* user;
    Admin admin;

    user = &admin;   // Polymorphism
    user->menu();

    return 0;
}
