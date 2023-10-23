#include <iostream>
#include <string>

using namespace std;

void sendSms(const string& number, const string& message){
    cout << "Message \"" << message << "\" has been sent to phone number \"" << number << endl;
}
void sendEmail(const string& email, const string& message){
    cout << "Message \"" << message << "\" has been sent to email \"" << email << endl;
}

class Notifier{
private:
    string message;
public:
    Notifier(std::string msg) : message{msg}
    {}
    string getMessage(){
        return this->message;
    }
    virtual void notify(){
        cout << "msg is \"" << this->message << endl;
    }
};

class SMSNotifier : public Notifier{
private:
    string number;
public:
    SMSNotifier(string num, string msg): number{num}, Notifier(msg)
    {}
    void notify() override{
        sendSms(this->number, getMessage());
    }
};

class EmailNotifier : public Notifier{
private:
    string email;
public:
    EmailNotifier(string mail, string msg): email{mail}, Notifier(msg)
    {}
    void notify() override{
        sendSms(this->email, getMessage());
    }
};

int main() {
    SMSNotifier sms("+7-923-228-38-72", "I know lessons in C++");
    EmailNotifier email("m.Filippov@g.nsu.ru", "And want the lesson one");
    sms.notify();
    email.notify();
    return 0;
}
