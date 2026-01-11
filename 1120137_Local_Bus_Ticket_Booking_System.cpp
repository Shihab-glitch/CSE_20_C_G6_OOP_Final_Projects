// Local Bus Ticket Booking System

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

bool userExists(string username){
    ifstream in("users.txt");
    string u,p,r;
    while(in>>u>>p>>r)
        if(u==username) return true;
    return false;
}

void registerUser(){
    string u,p;
    cout<<"Register Username: ";
    cin>>u;

    if(userExists(u)){
        cout<<"User already exists!\n";
        return;
    }

    cout<<"Set Password: ";
    cin>>p;

    ofstream out("users.txt",ios::app);
    out<<u<<" "<<p<<" user\n";
    out.close();

    cout<<"Registration Successful!\n";
}

bool login(string &currentUser){
    string u,p,fu,fp,r;
    cout<<"Username: ";
    cin>>u;
    cout<<"Password: ";
    cin>>p;

    ifstream in("users.txt");
    while(in>>fu>>fp>>r){
        if(fu==u && fp==p){
            currentUser=u;
            return true;
        }
    }
    return false;
}

void forgotPassword(){
    string u,newp;
    cout<<"Enter Username: ";
    cin>>u;

    if(!userExists(u)){
        cout<<"User not found!\n";
        return;
    }

    cout<<"Enter New Password: ";
    cin>>newp;

    ifstream in("users.txt");
    ofstream out("temp.txt");

    string fu,fp,r;
    while(in>>fu>>fp>>r){
        if(fu==u) out<<fu<<" "<<newp<<" "<<r<<"\n";
        else out<<fu<<" "<<fp<<" "<<r<<"\n";
    }

    in.close(); out.close();
    remove("users.txt");
    rename("temp.txt","users.txt");

    cout<<"Password Reset Successful!\n";
}

class Passenger{
protected:
    int ticketID, seat;
    string name, busType;
    float fareAmount;

public:
    Passenger(int id,string n,int s,string type,float f){
        ticketID=id; name=n; seat=s; busType=type; fareAmount=f;
    }

    virtual float fare()=0;

    int getSeat(){ return seat; }
    int getID(){ return ticketID; }
    string getBusType(){ return busType; }

    virtual void display(){
        cout<<"\nTicket ID: "<<ticketID
            <<"\nName: "<<name
            <<"\nSeat: "<<seat
            <<"\nRoute: Noakhali -> Cumilla"
            <<"\nBus: "<<busType
            <<"\nFare: "<<fareAmount<<" Tk\n";
    }
};

class ACPassenger: public Passenger{
public:
    ACPassenger(int id,string n,int s):Passenger(id,n,s,"AC",500){}
    float fare(){ return fareAmount; }
};

class NonACPassenger: public Passenger{
public:
    NonACPassenger(int id,string n,int s):Passenger(id,n,s,"NonAC",180){}
    float fare(){ return fareAmount; }
};

class BusSystem{
    vector<Passenger*> passengers;

public:
    void loadTickets(){
        ifstream in("tickets.txt");
        int id,seat;
        string type,name;

        while(in>>id>>seat>>type>>name){
            if(type=="AC") passengers.push_back(new ACPassenger(id,name,seat));
            else passengers.push_back(new NonACPassenger(id,name,seat));
        }
    }

    void saveTickets(){
        ofstream out("tickets.txt");
        for(auto p:passengers)
            out<<p->getID()<<" "<<p->getSeat()<<" "<<p->getBusType()<<" "<<p->getID()<<"\n";
    }

    void seatMap(string busType){
        cout<<"\n===== "<<busType<<" Bus Seat Map =====\n";

        int seatNo=1;
        for(int row=0;row<5;row++){
            // A side
            for(int i=0;i<2;i++){
                bool booked=false;
                for(auto p:passengers)
                    if(p->getSeat()==seatNo && p->getBusType()==busType)
                        booked=true;

                if(booked) cout<<"[X] ";
                else cout<<"[A"<<seatNo<<"] ";
                seatNo++;
            }

            cout<<"   ";

            // B side
            for(int i=0;i<2;i++){
                bool booked=false;
                for(auto p:passengers)
                    if(p->getSeat()==seatNo && p->getBusType()==busType)
                        booked=true;

                if(booked) cout<<"[X] ";
                else cout<<"[B"<<seatNo-1<<"] ";
                seatNo++;
            }
            cout<<endl;
        }
    }

    void payment(float amount){
        int ch;
        cout<<"\nPayment Amount: "<<amount<<" Tk\n";
        cout<<"1. Bkash\n2. Nagad\n3. Rocket\nChoose: ";
        cin>>ch;
        cout<<"Payment Successful!\n";
    }

    void bookTicket(){
        string name;
        int seat,cls;

        cout<<"Enter Name: ";
        cin>>name;

        cout<<"1. AC Bus\n2. Non-AC Bus\nChoose Bus: ";
        cin>>cls;

        string busType = (cls==1) ? "AC" : "NonAC";

        seatMap(busType);

        cout<<"Choose Seat (1-10): ";
        cin>>seat;

        for(auto p:passengers){
            if(p->getSeat()==seat && p->getBusType()==busType){
                cout<<"Seat already booked in this bus!\n";
                return;
            }
        }

        int id = passengers.size()+1;
        Passenger *p;

        if(busType=="AC") p=new ACPassenger(id,name,seat);
        else p=new NonACPassenger(id,name,seat);

        payment(p->fare());
        passengers.push_back(p);
        saveTickets();

        cout<<"Ticket Booked Successfully!\n";
    }

    void cancelTicket(){
        int seat;
        string type;

        cout<<"1. AC Bus\n2. Non-AC Bus\nChoose Bus: ";
        int c; cin>>c;
        type = (c==1) ? "AC" : "NonAC";

        cout<<"Enter Seat Number to cancel: ";
        cin>>seat;

        for(int i=0;i<passengers.size();i++){
            if(passengers[i]->getSeat()==seat && passengers[i]->getBusType()==type){
                delete passengers[i];
                passengers.erase(passengers.begin()+i);
                saveTickets();
                cout<<"Ticket Cancelled Successfully!\n";
                return;
            }
        }
        cout<<"Seat not found!\n";
    }

    void showPassengers(){
        if(passengers.empty()){
            cout<<"No passengers booked yet!\n";
            return;
        }
        for(auto p:passengers) p->display();
    }

    ~BusSystem(){
        for(auto p:passengers) delete p;
    }
};

int main(){
    BusSystem bs;
    bs.loadTickets();

    int choice;
    string user;

    do{
        cout<<"\n===== Local Bus Ticket Booking System =====\n";
        cout<<"1. Register\n2. Login\n3. Forgot Password\n0. Exit\nChoose: ";
        cin>>choice;

        if(choice==1) registerUser();
        else if(choice==2){
            if(login(user)){
                cout<<"\nWelcome "<<user<<"!\n";
                int op;
                do{
                    cout<<"\n1. Book Ticket\n2. Cancel Ticket\n3. Seat Map\n4. Passenger Records\n0. Logout\nChoose: ";
                    cin>>op;

                    if(op==1) bs.bookTicket();
                    else if(op==2) bs.cancelTicket();
                    else if(op==3){
                        cout<<"1. AC Bus\n2. Non-AC Bus\nChoose Bus: ";
                        int b; cin>>b;
                        bs.seatMap(b==1?"AC":"NonAC");
                    }
                    else if(op==4) bs.showPassengers();

                }while(op!=0);
            }
            else cout<<"Invalid Login!\n";
        }
        else if(choice==3) forgotPassword();

    }while(choice!=0);

    return 0;
}

