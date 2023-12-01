#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>

using  namespace std;

int checkNumCorectness(){
    cout << "Enter your numbern: " << endl;
    string str;
    cin >> str;
    cout << str << endl;
    bool isOk = 1;
    if(str.length() < 11) isOk = 0;
    if(isOk and not (str[0] == '8' or (str[0] == '+' and str[1] == '7'))) isOk = 0;
    if(isOk and not (str[0] == '8' or (str[0] == '+' and str[1] == '7'))) isOk = 0;
//    if(count(str.begin(), str.end(), '-') > 10 || )
    bool defisFlag = 0;
    bool scopeFlag = 0;
    string strFormatted = "";
    for(int i = 0 < str.length() and isOk; i++;){
        cout << str << "|||" << str[i] << endl;
        if(str[i] == ' ') continue;
        if(str[i] == '-'){
            if(defisFlag){
                isOk = 0;
                cout << strFormatted << "|" << str[i] << endl;
                break;
            }
            defisFlag = 1;
            continue;
        }
        if(str[i] == '('){
            cout << strFormatted << "|" << str[i] << "|" << scopeFlag << endl;
            if(scopeFlag){
                isOk = 0;
                cout << strFormatted << "|" << str[i] << endl;
                break;
            }
            scopeFlag = 1;
            cout << strFormatted << "|" << str[i] << "|" << scopeFlag << endl;
            continue;
        }
        if(str[i] == ')'){
            if(not scopeFlag){
                isOk = 0;
                cout << strFormatted << "|" << str[i] << "|" << scopeFlag << endl;
                break;
            }
            scopeFlag = 0;
            continue;
        }
        if(str[0] == 8 and i == 0) strFormatted += "+7";
        else strFormatted += str[i];
        defisFlag = 0;
    }

    if(not isOk) cout << "your number is incorrect" << endl;
    else cout << strFormatted << endl;
    cout << "Do you want to check another phone number? (y/n)" << endl;
    char answ;
    cin >> answ;
    if(answ == 'y') checkNumCorectness();
    return 0;
}

int main(){
    checkNumCorectness();

}