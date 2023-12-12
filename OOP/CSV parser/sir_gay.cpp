#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <tuple>
#include <iterator>
#include <stdexcept>
#include <exception>

using namespace std;

class CSVParserException : public std::exception {
public:
    explicit CSVParserException(std::string  message) : message_(std::move(message)) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class FileOpenException : public CSVParserException {
public:
    explicit FileOpenException(const std::string& filename)
            : CSVParserException("Ошибка: Невозможно открыть файл '" + filename + "'") {}
};

class ManyArgumentsException : public CSVParserException {
public:
    ManyArgumentsException(const int row, const int column)
            : CSVParserException("Ошибка при чтении: Слишком много аргументов в (" +
                                 std::to_string(row) +", " + std::to_string(column)+ ')') {}
};

class FewArgumentsException : public CSVParserException {
public:
    FewArgumentsException(const int row, const int column)
            : CSVParserException("Ошибка при чтении: Слишком мало аргументов в (" +
                                 std::to_string(row) +", " + std::to_string(column)+ ')') {}
};

class DifferentTypesException : public CSVParserException {
public:
    DifferentTypesException(const int row, const int column)
            : CSVParserException("Ошибка при чтении: Разные типы аргументов в (" +
                                 std::to_string(row) +", " + std::to_string(column)+ ')') {}
};



template <std::size_t I = 0, typename... Args>
typename std::enable_if<I == sizeof...(Args), void>::type
print_tuple(std::ostream& /*os*/, const std::tuple<Args...>& /*t*/) {}


template <std::size_t I = 0, typename... Args>
typename std::enable_if<I < sizeof...(Args), void>::type
print_tuple(std::ostream& os, const std::tuple<Args...>& t) {
    if (I != 0)
        os << ", ";
    os << std::get<I>(t);
    print_tuple<I + 1>(os, t);
}

template <typename Ch, typename Tr, typename... Args>
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
    if(std::get<1>(t)=="") return os;
    os << "(";
    print_tuple(os, t);
    os << ")";
    return os;
}


template <typename... Args>
class CSVParser {
public:
    CSVParser(std::ifstream& file, int skipLines=0, char rowDelimiter='\n', char columnDelimiter =' ', char escapeChar='\"')
            : file(file), skipLines(skipLines), rowDelimiter(rowDelimiter), columnDelimiter(columnDelimiter), escapeChar(escapeChar) {
        try {if (!file) {
                throw FileOpenException(".csv");
            }
        }
        catch (const CSVParserException &e) {
            std::cerr << e.what() << std::endl;
            exit(1);
        }
    }

    class iterator {
    public:
        iterator(std::ifstream& file, int skipLines, char columnDelimiter, char escapeChar, char rowDelimiter)
                : file(file), skipLines(skipLines), columnDelimiter(columnDelimiter), escapeChar(escapeChar), rowDelimiter(rowDelimiter) {
            for (int i = 0; i < skipLines; ++i) {
                ++currentLine;
                std::getline(file, line);
            }
        }

        iterator& operator++() {
            if (std::getline(file, line, rowDelimiter)) {
                parseLine();
                ++currentLine;
            }
            else
                isEnd = true;
            return *this;
        }

        bool operator!=(const iterator& other) const {
            return !isEnd;
        }

        std::tuple<Args...> operator*() const {
            return data;
        }

    private:
        std::string parseString(const std::string& string) {
            try {
                int count_columns=1;
                std::string new_line="";
                bool isEscaped = false;
                for (char c: string) {
                    if (count_columns>sizeof ...(Args))
                        throw ManyArgumentsException( currentLine, count_columns);
                    if (isEscaped) {
                        new_line += c;
                        isEscaped = false;
                    } else if (c==escapeChar) {
                        isEscaped = true;
                    }
                    else if (c==columnDelimiter) {
                        count_columns++;
                        new_line+=" ";
                    }
                    else new_line+=c;
                }
                if (count_columns!=sizeof ...(Args))
                    throw FewArgumentsException(currentLine, count_columns);
                return new_line;
            }
            catch (const CSVParserException &e) {
                std::cerr << e.what() << std::endl;
                exit(1);
            }
        }
        void parseLine() {
            line = parseString(line);
            std::istringstream ss(line);
            parseArgs(ss, data);
        }

        template <std::size_t I = 0, typename... TArgs>
        typename std::enable_if<I == sizeof...(TArgs), void>::type
        parseArgs(std::istringstream&, std::tuple<TArgs...>&) {}

        template <std::size_t I = 0, typename... TArgs>
        typename std::enable_if<I < sizeof...(TArgs), void>::type
        parseArgs(std::istringstream& ss, std::tuple<TArgs...>& t) {
            try {
                if (!(ss >> std::get<I>(t))) {
                    throw DifferentTypesException(currentLine, I);
                }
//            ss >> std::get<I>(t);
                parseArgs<I + 1>(ss, t);
            }
            catch (const CSVParserException &e) {
                std::cerr << e.what() << std::endl;
                exit(1);
            }
        }

        std::ifstream& file;
        int skipLines;
        bool isEnd = false;
        char columnDelimiter;
        char escapeChar;
        char rowDelimiter;
        int currentLine=0;
        std::string line;
        std::tuple<Args...> data;
    };

    iterator begin() {
        return iterator(file, skipLines, columnDelimiter, escapeChar, rowDelimiter);
    }

    iterator end() {
        return iterator(file, 0, columnDelimiter, escapeChar, rowDelimiter);
    }

private:
    std::ifstream& file;
    int skipLines;
    char rowDelimiter;
    char columnDelimiter;
    char escapeChar;
};


int main() {
    std::ifstream file("../in.csv");
//    CSVParser<int, double, string, char> parser(file, 0, '\n', ',');
    CSVParser<string, int, double, string> parser(file, 0, '\n', ' ');
    for (auto rs : parser) {
        std::cout << rs << std::endl;
    }
    return 0;
}
