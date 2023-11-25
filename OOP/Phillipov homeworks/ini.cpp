#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <utility>
#include <vector>

using namespace std;

namespace Ini{

    class Section{
    public:
        Section(const string name) : name{name}{};
//        void addVar(const string name, const string type, const string val){
        void addVar(const string name, const string val){
//            variables.insert(make_pair(name, make_pair(type, val)));
            variables.insert(make_pair(name, val));
        }
        unordered_map<string, string> getVariablesCopy(){
            return variables;
        }
    private:
        const string name;
//        unordered_map<string, pair<string, string>> variables;      //pair: 1st string is type: int, bool, string; 2nd is its string mapping
        unordered_map<string, string> variables;
    };

    class Document{
    public:
        Document(){
            cout << "document\n";
        }
        explicit Document(int a){
            cout << a ;
        };
        explicit Document(string input) : path{std::move(input)}{
            readFile();
        }
        void readFile(){
            ifstream in(path);
            string line;
            Section *curSection = nullptr;
            while(getline(in, line)){
                int openScope, closeScope, hashPos, semicolonPos;
                openScope = line.find('[');
                hashPos = line.find('#');
                semicolonPos = line.find(';');
                if((hashPos != -1 and hashPos < openScope) or (semicolonPos != -1 and semicolonPos < openScope)){
                    continue;
                }
                closeScope = line.find(']');
                if(openScope == -1 or closeScope == -1 or closeScope <= openScope and curSection != nullptr){
                    istringstream ss(line);
                    string name, val;
                    ss >> name;
                    ss.ignore(1, '=');
                    ss >> val;
                    curSection->addVar(name, val);
                } else{
                    curSection = new Section(line.substr(openScope, closeScope - openScope));
                }
            }
        }
        void writeFile(const string path){
            ofstream out(path);
            for(auto &section : sections){
                out << '[' << section.first << "]\n";
                auto map = section.second->getVariablesCopy();
                for(auto &var : map){
                    out << var.first << " = " << var.second << "\n";
                }
            }
            out.close();
        }
    private:
        const string path;
        unordered_map<string, Section*> sections;
    };
}