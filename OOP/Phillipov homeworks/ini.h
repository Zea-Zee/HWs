#include <string>
#include <unordered_map>
#include <vector>


namespace Ini {

    class Section {
    public:
        Section(const std::string name);
        void addVar(const std::string name, const std::string val);
        std::unordered_map<std::string, std::string> getVariablesCopy() const;
        std::string getName(){
            return name;
        }
    private:
        const std::string name;
        std::unordered_map<std::string, std::string> variables;
    };

    class Document {
    public:
        explicit Document();
        explicit Document(int);
        explicit Document(std::string);
        void readFile();
        void writeFile(const std::string);

        Section& AddSection(std::string);
        const Section& GetSection(const std::string&) const;
        size_t SectionCount() const;

    private:
        const std::string path;
        std::unordered_map<std::string, Section*> sections;
    };

} // namespace Ini


