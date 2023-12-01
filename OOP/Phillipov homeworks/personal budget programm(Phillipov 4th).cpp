#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

class EarnHistory{
private:
    vector<double> dayByDay;
    tm zeroDate{};
public:
    EarnHistory(){
        istringstream inputDate("2000-01-01");
        inputDate >> get_time(&zeroDate, "%Y-%m-%d");
        time_t startDate = mktime(&zeroDate);
//        EarnHistory : dayByDay(37230, 0);
        vector<double> temp(50000,0);
        dayByDay = temp;
    }
    void Earn(const string &date1, const string &date2, int val){
        unsigned short fi = 0, li = 0;
        tm curDate{};
        istringstream inDate(date1);
        inDate >> get_time(&curDate, "%Y-%m-%d");
        time_t stDate = mktime(&zeroDate);
        time_t endDate = mktime(&curDate);
        long long timeDif = difftime(endDate, stDate);
        fi = (unsigned short) (timeDif / 3600 / 24);
        istringstream inDate2(date2);
        inDate2 >> get_time(&curDate, "%Y-%m-%d");
        endDate = mktime(&curDate);
        timeDif = difftime(endDate, stDate);
        li = (unsigned short) (timeDif / 3600 / 24);
        timeDif = li + 1 - fi;
        double avg = (double) val / timeDif;
        double sum = 0;
        for(int i = fi; i <= li; i++){
            dayByDay[i] += avg;
            sum += avg;
        }
        cout.precision(25);
        cout << "EARN::: Days from " << date1 << " to " << date2 << " was filled by number " <<fixed<<avg << endl;
    }
    void ComputeIncome(const string &date1, const string &date2){
        unsigned short fi = 0, li = 0;
        tm curDate{};
        istringstream inDate(date1);
        inDate >> get_time(&curDate, "%Y-%m-%d");
        time_t stDate = mktime(&zeroDate);
        time_t endDate = mktime(&curDate);
        long long timeDif = difftime(endDate, stDate);
        fi = (unsigned short) (timeDif / 3600 / 24);
        istringstream inDate2(date2);
        inDate2 >> get_time(&curDate, "%Y-%m-%d");
        endDate = mktime(&curDate);
        timeDif = difftime(endDate, stDate);
        li = (unsigned short) (timeDif / 3600 / 24);
//        cout << fi << ":" << li;
        timeDif = li + 1 - fi;
        double sum = 0;
        for(int i = fi; i <= li; i++){
            sum += dayByDay[i];
//            cout << "day number " << i << " was extracted " << dayByDay[i] << endl;
        }
        cout << "PROFT::: Total profit from " << date1 << " to " << date2 << " is: " <<fixed<< sum << endl;
    }
};

int main(){
//    tm zeroDate{};
//    istringstream inputDate("2100-01-01");
//    inputDate >> get_time(&zeroDate, "%Y-%m-%d");
//    time_t startDate = mktime(&zeroDate);
//    cout << startDate;
    int q;
    cin >> q;
    cin.ignore(1);
    EarnHistory Tom;
    for(int i = 0; i < q; i++){
        string str;
        string func;
        string date1, date2;
        cin >> func;
        cin >> date1;
        cin >> date2;
        if(func == "Earn"){
            int val;
            cin >> val;
            Tom.Earn(date1, date2, val);
        } else if(func == "ComputeIncome"){
            Tom.ComputeIncome(date1, date2);
        } else cout << "ERR" << endl;
        cin.ignore(1);
    }
    //!!!PROG TEST!
//    Tom.Earn("2047-01-01", "2049-01-01", 1000000);
//    Tom.ComputeIncome("2048-01-01", "2049-01-01");
//    string buff;
//    strftime(buff, 80, zeroDate);
//    cout << zeroDate.tm_year << "|" << zeroDate.tm_mon << "|" << zeroDate.tm_mday << "|" << std::put_time(&zeroDate) << endl;
//    cout << zeroDate.tm
    return 0;
}

//!!!CMD TEST!!!
//    5
//    Earn 2000-01-02 2000-01-06 20
//    ComputeIncome 2000-01-01 2001-01-01
//    ComputeIncome 2000-01-01 2000-01-03
//    Earn 2000-01-03 2000-01-03 10
//    ComputeIncome 2000-01-01 2001-01-01
//    Вывод
//    20
//    8
//    30