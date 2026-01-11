// Notification System Implementation

#include <iostream>
using namespace std;
class Notification {
protected:
    string message;

public:
    Notification(string msg) {
        message = msg;
    }
    virtual void send() = 0;

    virtual ~Notification() {}
};
class EmailNotification : public Notification {
public:
    EmailNotification(string msg) : Notification(msg) {}

    void send() {
        cout << "Email sent: " << message << endl;
    }
};
class SMSNotification : public Notification {
public:
    SMSNotification(string msg) : Notification(msg) {}

    void send() {
        cout << "SMS sent: " << message << endl;
    }
};
class PushNotification : public Notification {
public:
    PushNotification(string msg) : Notification(msg) {}

    void send() {
        cout << "Push notification sent: " << message << endl;
    }
};
int main() {
    Notification* n;

    n = new EmailNotification("Welcome!");
    n->send();
    delete n;

    n = new SMSNotification("Your OTP is 1234");
    n->send();
    delete n;

    n = new PushNotification("You have a new message");
    n->send();
    delete n;

    return 0;
}
