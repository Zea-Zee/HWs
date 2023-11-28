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

Section::Section(const string name) : name{name}{
//    variables = *new unordered_map<std::string, std::string>;
};
//        void addVar(const string name, const string type, const string val){
 void Section::addVar(const string name, const string val){
//            variables.insert(make_pair(name, make_pair(type, val)));
//    cout << " now varibles len is " << variables.size();
    variables.insert(make_pair(name, val));
}
unordered_map<string, string> Section::getVariablesCopy() const{
    return variables;
}

//Document::Document(){
//    cout << "document\n";
//}
//Document::Document(int a){
//    cout << a ;
//};
Document::Document(string input) : path{std::move(input)}{
    readFile();
}
void Document::readFile() {
    ifstream in(path);
    string line;
    Section *curSection = nullptr;
    while (getline(in, line)) {
        int openScope, closeScope, hashPos, semicolonPos;
        openScope = line.find('[');
        hashPos = line.find('#');
        semicolonPos = line.find(';');
        if(hashPos == 0 or semicolonPos == 0){
            continue;
        }
        closeScope = line.find(']');
        int a = line.find('=');
        if ((openScope != 0 or closeScope == -1)){
            if (curSection != nullptr and line.find('=') != -1) {
                int end = -1;
                if(semicolonPos != -1){
                    if(hashPos != -1) end = min(hashPos, semicolonPos);
                    else end = semicolonPos;
                } else if(hashPos != -1) end = hashPos;
                if(end != -1) line = line.substr(0, end);
//                istringstream ss(line);
                int eqPos = line.find('=');
//                int endPos = line.find(' ');
//                if(endPos == -1) endPos = line.find('\n');
                end = min(end, (int) line.length());
                string name{line.substr(0, eqPos)}, val{line.substr(eqPos + 1, end - eqPos - 1)};
//                ss >> name;
//                ss.ignore(1, '=');
//                ss >> val;
//                curSection->addVar(name, val);
                curSection->addVar(name, val);
            } else {
                continue;
            }
        } else{
            if(curSection != nullptr){
                sections.insert(make_pair(curSection->getName(), curSection));
//                writeFile("./output.ini");
//                exit(2);
            }
            curSection = new Section(line.substr(openScope + 1, closeScope - openScope - 1));
        }
    }
    if(curSection != nullptr) sections.insert(make_pair(curSection->getName(), curSection));
}
void Document::writeFile(const string path){
    ofstream out(path);
    for(auto &section : sections){
        out << '[' << section.first << "]\n";
//        out << section.first << "\n";
        auto map = section.second->getVariablesCopy();
        for(auto &var : map){
            out << var.first << " = " << var.second << "\n";
        }
        out << "\n";
    }
    out.close();
}

Section& Document::AddSection(string name){
    Section* a = new Section(name);
    sections.insert(make_pair(a->getName(), a));
    return *a;
 }
const Section& Document::GetSection(const string& name) const{
     const Section& res = *sections.at(name);
     return res;
 };
size_t Document::SectionCount() const{
    return sections.size();
};