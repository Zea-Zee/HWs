#include <iostream>
#include <memory>
#include <functional>
#include "test_runner.h"
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

struct Email {
    string from;
    string to;
    string body;
};

class Worker {
public:
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        throw logic_error("Unimplemented");
    }

protected:
    void PassOn(unique_ptr<Email> email) const {
        if (nextWorker) {
            nextWorker->Process(move(email));
        }
    }

public:
    unique_ptr<Worker>& GetNextWorker() {
        return nextWorker;
    }
    void SetNext(unique_ptr<Worker> worker) {
        nextWorker = move(worker);
    }

private:
    unique_ptr<Worker> nextWorker;
};

class Reader : public Worker {
public:
    explicit Reader(istream& in) : input(in) {}

    void Run() override {
        while (true) {
            auto email = make_unique<Email>();
            getline(input, email->from);
            getline(input, email->to);
            getline(input, email->body);

            if (input.eof()) {
                break;
            }

            PassOn(move(email));
        }
    }

    void Process(unique_ptr<Email> email) override {
        throw logic_error("Reader should not process emails");
    }

private:
    istream& input;
};

class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;

    explicit Filter(Function filterFunction) : filterFunction(move(filterFunction)) {}

    void Process(unique_ptr<Email> email) override {
        if (filterFunction(*email)) {
            PassOn(move(email));
        } else {
            auto& next = GetNextWorker();
            if (next) {
                auto nextWorker = move(next->GetNextWorker());
                next->SetNext(move(nextWorker));
            }
        }
    }

private:
    Function filterFunction;
};

class Copier : public Worker {
public:
    explicit Copier(string recipient) : recipient(move(recipient)) {}

    void Process(unique_ptr<Email> email) override {
<<<<<<< HEAD
// Pass the original email to the next worker
        auto copy = make_unique<Email>(*email);
        PassOn(move(email));
// If the recipients are different, create a copy and pass it to the next worker
        if (copy->to != recipient) {

            copy->to = recipient;
            PassOn(move(copy));
        }

=======
        auto copy = make_unique<Email>(*email);
        PassOn(move(email));
        if (copy->to != recipient) {
            copy->to = recipient;
            PassOn(move(copy));
        }
>>>>>>> 1e06dbb26eb373d18eaf1595de135176f6f0b386
    }

private:
    string recipient;
};

class Sender : public Worker {
public:
    explicit Sender(ostream& out) : output(out) {}
<<<<<<< HEAD

=======
>>>>>>> 1e06dbb26eb373d18eaf1595de135176f6f0b386
    void Process(unique_ptr<Email> email) override {
        output << email->from << "\n" << email->to << "\n" << email->body << "\n";
        PassOn(move(email));
    }

private:
    ostream& output;
};

class PipelineBuilder {
public:
    explicit PipelineBuilder(istream& in) {
        firstWorker = make_unique<Reader>(in);
        lastWorker = firstWorker.get();
    }

    PipelineBuilder& FilterBy(Filter::Function filter) {
        auto filterWorker = make_unique<Filter>(move(filter));
        lastWorker->SetNext(move(filterWorker));
        lastWorker = lastWorker->GetNextWorker().get();
        return *this;
    }

    PipelineBuilder& CopyTo(string recipient) {
        auto copierWorker = make_unique<Copier>(move(recipient));
        lastWorker->SetNext(move(copierWorker));
        lastWorker = lastWorker->GetNextWorker().get();
        return *this;
    }

    PipelineBuilder& Send(ostream& out) {
        auto senderWorker = make_unique<Sender>(out);
        lastWorker->SetNext(move(senderWorker));
        lastWorker = lastWorker->GetNextWorker().get();
        return *this;
    }

    unique_ptr<Worker> Build() {
        return move(firstWorker);
    }

private:
    unique_ptr<Worker> firstWorker;
    Worker* lastWorker;
};

void TestSanity() {
    string input = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "ralph@example.com\n"
            "erich@example.com\n"
            "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "erich@example.com\n"
            "richard@example.com\n"
            "Are you sure you pressed the right button?\n"
    );
    std::cout << outStream.str();
    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}