#include <functional>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <tuple>
#include <array>
#include <vector>
using namespace std;

template <typename ... TList>
struct DefaultParsersTuple;

template<size_t I = 0, class Ch, class Tr, typename... Tp>
void print(basic_ostream<Ch, Tr>& os, tuple<Tp...> const& t) {
    os << get<I>(t) << " ";
    if constexpr (I + 1 != sizeof...(Tp))
        print<I + 1>(os, t);
}


template<class Ch, class Tr, typename... Args>
auto operator<<(basic_ostream<Ch, Tr>& os, tuple<Args...> const& t)
-> std::basic_ostream<Ch, Tr>&
{
    print(os, t);
    return os;
}

/**
 * (Simple) CSV reader.
 *
 * I wanted just simple class but it went out of my control;)
 */
template <typename... TList>
class CSVReader {
public:
    using row_type = tuple<TList ...>;
    CSVReader();
    explicit CSVReader(ifstream& st, int skip_first_line = 1, char separator = ',');
    row_type operator[] (std::size_t i);
    const row_type operator[] (std::size_t i) const;
    size_t row_size() const;
    size_t read_rows(size_t n = numeric_limits<size_t>::max());
    using iterator = typename vector<row_type>::iterator;
    using const_iterator = typename vector<row_type>::const_iterator;
    iterator begin() { return rows.begin(); }
    iterator end() { return rows.end(); }
    const_iterator begin() const { return rows.begin(); }
    const_iterator end() const { return rows.end(); }
private:
    vector<row_type> rows;
    ifstream* stream;

    tuple<function<TList(const string&)>...> parser_str =
            DefaultParsersTuple<TList ... >::get();

    char separator = ',';
    char qu = '"';
    char ends = '\n';
    bool inv_len = false;
    int skip_first_line = 1;
};

int r, c;

template <typename T>
struct DefaultParser {
    static T parse(const string& str) {
        if (str.empty())
            return T();

        istringstream stream(str);
        T t;
        stream >> t;
        if (!stream.eof() || stream.fail()) {
            cerr << "Incorrect format of data in row " << r << ", column " << c; exit(1);
        }

        return t;
    }
};

template <>
struct DefaultParser<string> {
    static string parse(const string& str) {
        return str;
    }
};

template <typename T>
struct DefaultParsersTuple<T> {
    using parsers_type = std::tuple<std::function<T(const std::string&)>>;

    static parsers_type get() {
        c++;
        return parsers_type(DefaultParser<T>::parse);
    }
};

template <typename T, typename ... TList>
struct DefaultParsersTuple<T, TList...> {
    using parsers_type = std::tuple<
            std::function<T(const std::string&)>,
            std::function<TList(const std::string&)>...>;

    static parsers_type get() {
        return tuple_cat(forward_as_tuple(function<T(const std::string&)>(DefaultParser<T>::parse)), DefaultParsersTuple<TList...>::get());
    }
};

template <typename Row, typename Parsers, int N>
struct RowParser {
    static void parse(Row& row, const Parsers& parsers, const array<string,tuple_size<Parsers>::value>& parts) {
        get<N - 1>(row) = get<N - 1>(parsers)(parts[N - 1]);
        RowParser<Row, Parsers, N - 1>::parse(row, parsers, parts);
    }
};

template <typename Row, typename Parsers>
struct RowParser<Row, Parsers, 1> {
    static void parse(Row& row,const Parsers& parsers,const array<string,tuple_size<Parsers>::value>& parts) {
        get<0>(row) = get<0>(parsers)(parts[0]);
    }
};

template <typename Row, typename Parsers>
void parse_row(Row& row, const Parsers& parsers, const array<std::string, tuple_size<Parsers>::value>& parts) {
    RowParser<Row,Parsers,std::tuple_size<Parsers>::value>::parse(row, parsers, parts);
}

template <typename... TList>
CSVReader<TList ...>::CSVReader() = default;

template <typename ... TList>
CSVReader<TList ...>::CSVReader(ifstream& st, int skip_first_line, char separator) : skip_first_line(skip_first_line), separator(separator) {
    stream = &st;
    read_rows();
}

template <typename ... TList>
typename CSVReader<TList...>::row_type CSVReader<TList ...>::operator[] (size_t i) {
    return rows[i];
}

template <typename ... TList>
const typename CSVReader<TList...>::row_type CSVReader<TList ...>::operator[] (
        size_t i) const {
    return rows[i];
}

template <typename ... TList>
size_t CSVReader<TList...>::row_size() const {
    return rows.size();
}

template <typename ... TList>
size_t CSVReader<TList ...>::read_rows(size_t n) {
    int rows_readed = 0;
    string line;
    bool quoted;

    /*if (!first_lines_skipped_ && skip_first_lines_ > 0) {
        for (std::size_t i = 0; i < skip_first_lines_; ++i)
            std::getline(stream, line);
        first_lines_skipped_ = true;
    }*/
    while (skip_first_line > 0) {
        getline(*stream, line);
        skip_first_line--;
    }

    while (rows_readed < n) {
        if (!(*stream))
            break;

        array<string, sizeof...(TList)> parts;

        getline(*stream, line);
        if (line.empty())
            continue;

        row_type row;

        bool is_escaped = false;
        int k = 0;
        int last_part = 0;
        quoted = false;

        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ends) {
                is_escaped = true;
                continue;
            }
            else
                is_escaped = false;

            if (!is_escaped && line[i] == qu) {
                quoted = !quoted;
                continue;
            }

            if (!is_escaped && !quoted &&
                (line[i] == separator || i+1 == line.length())) {
                if (k >= sizeof...(TList)) {
                    //throw runtime_error("Error in line\nInvalid size of row");
                    cerr << "Error in line " << (rows_readed + 1 + skip_first_line) << ", col " << (k) << "\nInvalid data";
                    exit(1);
                }
                parts[k] = line.substr(last_part, i - last_part + ((i == line.length() - 1) ? 1 : 0));
                last_part = i + 1;
                k++;
                continue;
            }
        }
        r = rows_readed+1;
        parse_row(row, parser_str, parts);
        c = 1;
        rows.push_back(move(row));
        rows_readed++;
    }
    return rows_readed;
}

int main() {
    ifstream st("../in.csv");
    CSVReader<int, int, string> reader(st, 0, ' ');
    for (tuple<int, int, string> i : reader) {
        cout << i << endl;
    }
}
