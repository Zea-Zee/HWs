#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <exception>
#include <cerrno>
#include <array>

bool FIRST_TIME = true;

using namespace std;

class CSVParserException : public exception {
public:
    explicit CSVParserException(const string& message) : message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    string message_;
};

class FileOpenException : public CSVParserException {
public:
    explicit FileOpenException(const string& filename)
            : CSVParserException("Error: Unable to open file '" + filename + "'") {}
};

class ManyArgumentsException : public CSVParserException {
public:
    ManyArgumentsException(int row, int column)
            : CSVParserException("Error while reading: Too many arguments at (" +
                                 to_string(row) + ", " + to_string(column) + ')') {}
};

class FewArgumentsException : public CSVParserException {
public:
    FewArgumentsException(int row, int column)
            : CSVParserException("Error while reading: Too few arguments at (" +
                                 to_string(row) + ", " + to_string(column) + ')') {}
};

class DifferentTypesException : public CSVParserException {
public:
    DifferentTypesException(int row, int column)
            : CSVParserException("Error while reading: Different argument types at (" +
                                 to_string(row) + ", " + to_string(column) + ')') {}
};

template <size_t I = 0, typename... Args>
typename enable_if<I == sizeof...(Args), void>::type
print_tuple(ostream& /*os*/, const tuple<Args...>& /*t*/) {}

template <size_t I = 0, typename... Args>
typename enable_if<I < sizeof...(Args), void>::type
print_tuple(ostream& os, const tuple<Args...>& t) {
    if (I != 0)
        os << ", ";
    os << get<I>(t);
    print_tuple<I + 1>(os, t);
}

template <typename Ch, typename Tr, typename... Args>
basic_ostream<Ch, Tr>& operator<<(basic_ostream<Ch, Tr>& os, const tuple<Args...>& t) {
    if(FIRST_TIME){
        FIRST_TIME = false;
        return os;
    }
//    if (to_string(get<1>(t)) == "")
//        return os;
    os << "(";
    print_tuple(os, t);
    os << ")";
    return os;
}

template <typename... Args>
class CSVParser {
public:
    CSVParser(const char *path, int skipLines=0, char rowDelimiter='\n', char columnDelimiter=' ', char escapeChar='\"')
            : skipLines(skipLines), rowDelimiter(rowDelimiter), colDelimiter(columnDelimiter), escapeChar(escapeChar) {
        file.open(path);
        while(!file.is_open()){
            if(errno == EAGAIN or errno == EINTR){
                cerr << "Can't open the " << path << " due to core, trying again.\n";
                file.open(path);
            } else
                throw FileOpenException(path);
        }
        this->rowsNum = countRowNum();
    }
    /**
     * @brief It count number of rows in file.
     * @param resetFilePosition define will be fileposition reset or not.
     */
    unsigned long long countRowNum(bool resetFilePosition=true){
//        streampos oldPos = file.tellg();
//        unsigned long long count = 0;
//        file.seekg(0, ios::end);
//        streampos fileSize = file.tellg();
//        file.seekg(0, ios::beg);
//        string fileData;
//        file.read(fileData.data(), fileSize);
//        for(char ch : fileData) if(ch == '\n') count++;
//        if(resetFilePosition) file.seekg(0, ios::beg);
//        else file.seekg(oldPos, ios::beg);
//        cout << "file rows is " << count << "\n";
//        return count;
        unsigned long long count = 0;
        string line;

        while (getline(file, line)) {
            ++count;
        }

        if (resetFilePosition) {
            file.clear();  // Clear any error flags
            file.seekg(0, ios::beg);
        }

        cout << "file rows is " << count << "\n";
        return count;
    }

    class Iterator{
    public:
        Iterator(ifstream& file, unsigned long long len, int skipLines=0, char rowDelimeter='\n', char colDelimiter=' ', char escapeChar='"')
        : rows(len), file(file), skipLines(skipLines), rowDelimeter(rowDelimeter), colDelimiter(colDelimiter), escapeChar(escapeChar) {
            for(; curLine < skipLines; curLine++)
                file.ignore(ULLONG_MAX, '\n');
            cout << "constructor\n";
        }

        Iterator& operator++(){
//            cout << curLine << " will be incremented and rows is " << rows << "\n" ;
            trash:
            if(getline(file, line, rowDelimeter)){
                if(line.length() == 0)
                    goto trash;
                else{
                    parseLine();
                    curLine++;
                }
            }
            return *this;
        }

        unsigned long long getLen(){
            return rows;
        }

        tuple<Args...> operator*() const {
            return data;
        }

        bool isEnd(){
            return curLine == (rows + 1);
        }

        bool operator!=(const Iterator& other) const {
            return curLine != rows;
        }

    private:
        string parseString(const string& str) {
            int count_columns = 1;
            string new_line = " ";
            bool isEscaped = false;
            for(char c: str) {
                if(count_columns > sizeof ...(Args))
                    throw ManyArgumentsException(curLine, count_columns);
                if(isEscaped) {
                    new_line += c;
                    isEscaped = false;
                } else if (c == escapeChar) {
                    isEscaped = true;
                } else if (c == colDelimiter) {
                    count_columns++;
                    new_line += " ";
                }
                else new_line += c;
            }
            if (count_columns != sizeof ...(Args))
                throw FewArgumentsException(curLine, count_columns);
            return new_line;
        }

        void parseLine() {
            line = parseString(line);
            istringstream ss(line);
            parseArgs(ss, data);
        }

        template <size_t I = 0, typename... TArgs>
        typename enable_if<I == sizeof...(TArgs), void>::type
        parseArgs(istringstream&, tuple<TArgs...>&) {}

        template <size_t I = 0, typename... TArgs>
        typename enable_if<I < sizeof...(TArgs), void>::type
        parseArgs(istringstream& ss, tuple<TArgs...>& t) {
            if (!(ss >> get<I>(t))) throw DifferentTypesException(curLine, I);
            parseArgs<I + 1>(ss, t);
        }

        ifstream& file;
        unsigned long long rows;
        int skipLines;
        char rowDelimeter;
        char colDelimiter;
        char escapeChar;
        int curLine = 0;
        string line;
        tuple<Args...> data;
    };

    Iterator begin(){
        return Iterator(file, rowsNum, skipLines, rowDelimiter, colDelimiter, escapeChar);
    }

    Iterator end() {
        return Iterator(file, rowsNum, skipLines, rowDelimiter, colDelimiter, escapeChar);
    }

private:
    ifstream file;
    unsigned long long rowsNum;
    int skipLines;
    char rowDelimiter;
    char colDelimiter;
    char escapeChar;
};

int main() {
    const char *path = "../in.csv";
    CSVParser<string, int, double, string> parser(path);
    for (auto rs : parser) {
        cout << rs << endl;
    }
    return 0;
}
