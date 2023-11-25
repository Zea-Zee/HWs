#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <utility>
#include <vector>

#include "ini.h"

using namespace std;
using namespace Ini;

int main(){
    Ini::Document a("./input.ini");
    a.AddSection("new section");
    cout << a.SectionCount() << "\n";
    Section ref = a.GetSection("new section");
    cout << ref.getName() << "\n";
    a.writeFile("./output.ini");
    return 0;
}