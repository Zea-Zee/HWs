#include <iostream>
#include <sstream>
using namespace std;

class Rational {
public:
    Rational(){
        cout << "Rational object has created without any data" << endl;
    }
    Rational(long long numerator, long long denominator) {
        if (denominator == 0) {
//            std::stringstream ss;
            cout << numerator << "Argument must not be zero" << denominator << endl;
            throw "invalid arg";
        }
        num = numerator;
        denom = denominator;
        cout << "Rational object has created with " << this->num << "/" << this->denom << endl;
    }
    long long Numerator() const{
        return this->num;
    }
    long long Denominator() const{
        return this->denom;
    }
    friend Rational operator+(const Rational& a, const Rational& b);
    friend Rational operator*(const Rational& a, const Rational& b);
    friend Rational operator/(const Rational& a, const Rational& b);
    friend Rational operator-(const Rational& a, const Rational& b);
    friend bool operator==(const Rational &a, const Rational &b);
//    friend ostream &operator<<(ostream &out,const Rational &a);
    friend ostream &operator<<(ostream &out,const Rational &a){
        cout << a.num << "/" << a.denom << endl;
        return out << a.num << "/" << a.denom;
    }
    friend istream &operator>>(istream &in,  Rational &a){
        in >> a.num;
        in.ignore(1);
        in >> a.denom;
        return in;
    }
private:
    long long num, denom;
};

long long gcd(long long a, long long b){
    while(b){
//        cout << a << "|" << b << endl;
        a %= b;
//        a, b = b, a;
        long long tmp = a;
        a = b;
        b = tmp;
    }
    return a;
}


Rational operator+(const Rational& a, const Rational& b){
//    long long sameBase = gcd(a.denom, b.denom);
//    a.denom > b.denom ? (b.denom = a.denom)
    long long num, denom;
    long long an, bn, ad, bd;
    if(a.num == 0){
        num = b.num;
        denom = b.denom;
    } else if(b.num == 0){
        num = a.num;
        denom = a.denom;
    } else if(a.denom == b.denom){
        num = a.num + b.num;
    } else if(a.denom % b.denom == 0){
//        cout << a.num << a.denom << b.num << b.denom << endl;
        bd = a.denom;
        bn = (a.denom / b.denom) * b.num;
//        cout << bd << bn << endl;
        num = a.num + bn;
        denom = bd;
    } else if(b.denom % a.denom == 0){
        ad *= (b.denom / a.denom);
        an *= (b.denom / a.denom);
        num = b.num + an;
        denom = ad;
    } else{
        denom = a.denom * b.denom;
        num = a.num * b.denom + b.num * a.denom;
    }
//    cout << num << "/" << denom << endl;
    long long sameBase = gcd(num, denom);
    if(sameBase > 1){
        num /= sameBase;
        denom /= sameBase;
    }
    Rational res{num, denom};
    return res;
}

Rational operator-(const Rational& a, const Rational& b){
    long long num, denom;
    long long an, bn, ad, bd;
    if(a.num == 0){
        num = b.num;
        denom = b.denom;
    } else if(b.num == 0){
        num = a.num;
        denom = a.denom;
    } else if(a.denom == b.denom){
        num = a.num - b.num;
    } else if(a.denom % b.denom == 0){
        denom = a.denom;
        bn = (a.denom / b.denom) * b.num;
        num = a.num - bn;
//        denom = bd;
    } else if(b.denom % a.denom == 0){

        ad *= (b.denom / a.denom);
        an *= (b.denom / a.denom);
        num = b.num - an;
        denom = ad;
    } else{
        denom = a.denom * b.denom;
        num = a.num * b.denom - b.num * a.denom;
    }
    cout << num << "/" << denom << endl;
    long long sameBase = gcd(num, denom);
    if(sameBase > 1){
        num /= sameBase;
        denom /= sameBase;
    }
    Rational res{num, denom};
    return res;
}

Rational operator*(const Rational &a, const Rational &b){
    long long num, denom;
    long long an = a.num, bn = b.num, ad = a.denom, bd = b.denom;
    long long sameBaseA = gcd(an, ad);
    if(sameBaseA > 1){
        an = a.num / sameBaseA;
        ad = a.denom / sameBaseA;
    }
    long long sameBaseB = gcd(bn, bd);
    if(sameBaseB > 1){
        bn = b.num / sameBaseB;
        bd = b.denom / sameBaseB;
    }
    long long sameBaseAB = gcd(an, bd);
    if(sameBaseAB > 1){
        an /= sameBaseAB;
        bd /= sameBaseAB;
    }
    long long sameBaseBA = gcd(bn, ad);
    if(sameBaseAB > 1){
        ad /= sameBaseBA;
        bn /= sameBaseBA;
    }
    num = an * bn;
    denom = ad * bd;
//    long long sameBase = gcd(num, denom);
//    if(sameBase > 1){
//        num /= sameBase;
//        denom /= sa
//    }
    Rational res{num, denom};
    return res;
}

Rational operator/(const Rational &a, const Rational &b){
    Rational newb{b.denom, b.num};
    Rational res = a * newb;
    return res;
}

bool operator==(const Rational &a, const Rational &b){
    if(a.num == b.num && a.denom == b.denom) return true;
//    if(a.denom % b.denom == 0 && a.num)
    long long an, ad, bn, bd;
    long long sameBaseA = gcd(an, ad);
    if(sameBaseA > 1){
        an = a.num / sameBaseA;
        ad = a.denom / sameBaseA;
    }
    long long sameBaseB = gcd(bn, bd);
    if(sameBaseB > 1){
        bn = b.num / sameBaseB;
        bd = b.denom / sameBaseB;
    }
    long long sameBaseAB = gcd(an, bd);
    if(sameBaseAB > 1){
        an /= sameBaseAB;
        bd /= sameBaseAB;
    }
    long long sameBaseBA = gcd(bn, ad);
    if(sameBaseAB > 1){
        ad /= sameBaseBA;
        bn /= sameBaseBA;
    }
    if(an == bn && ad == bd) return true;
    return false;
}

//int main() {
//    Rational r1{6, 4};
//    Rational r2{5, 2};
//    Rational res = r1 + r2;
//    cout << res.Numerator() << "/" << res.Denominator();
//    Rational res2 = r1 - r2;
//    cout << res2.Numerator() << "/" << res2.Denominator();
//    Rational res3 = r1 * r2;
//    cout << res3.Numerator() << "/" << res3.Denominator();
//    Rational res4 = r1 / r2;
//    cout << res4.Numerator() << "/" << res4.Denominator();
//    bool isEq = r1 == r2;
//    cout << isEq;
//    return 0;
//}

int main() {
    string input;
    getline(std::cin, input);
    stringstream ss(input);
    Rational r1;
    Rational r2;
    char operation;
    try {
        ss >> r1;
        ss.ignore(1);
        ss >> operation;
        ss.ignore(1);
        ss >> r2;
    } catch (exception& ex) {
        std::cout << "Invalid argument" << std::endl;
        return 1;
    }
    Rational res;
    try {
        switch (operation) {
            case '+': {
                res = r1 + r2;
                string output;
                stringstream os(output);
                ss << res;
                cout << output;
                break;
            }
            case '-': {
                res = r1 - r2;
                break;
            }
            case '*': {
                res = r1 * r2;
                break;
            }
            case '/': {
                res = r1 / r2;
                break;
            }
            default: {
                std::cout << "Invalid operation" << std::endl;
            }
        }
    }
    catch (exception& ex) {
        std::cout << "Division by zero" << std::endl;
        return 2;
    }

    return 0;
}
