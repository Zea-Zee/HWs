//#include "test_runner.h"
// #include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <assert.h>
// #include <unordered_map>
#include <map>
#include <iostream>

using namespace std;

template <typename String>
using Group = vector<String>;

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
    // unordered_map<set<typename String::value_type>, Group<String>> groupsMap;
    map<set<typename String::value_type>, Group<String>> groupsMap;
    for(auto &str : strings){
        set<char> chars(str.begin(), str.end());
        if(groupsMap.count(chars)){
            groupsMap.at(chars).push_back(str);
        } else{
            vector<string> val;
            val.push_back(str);
            groupsMap.insert(make_pair(chars, val));
        }
    }
   vector<Group<String>> res;
    // vector<vector<string>> res;
    // for(auto el = groupsMap.begin(); el != groupsMap.end(); el++){
    for(auto &group : groupsMap){
        // cout << group.second[0] << "\n";
        res.push_back(group.second);
    }
    return res;
}

//void TestGroupingABC() {
//    vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
//    auto groups = GroupHeavyStrings(strings);
//    ASSERT_EQUAL(groups.size(), 2);
//    sort(begin(groups), end(groups));
//    ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
//    ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
//}

//void TestGroupingReal() {
//    vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
//    auto groups = GroupHeavyStrings(strings);
//    ASSERT_EQUAL(groups.size(), 4);
//    sort(begin(groups), end(groups));
//    ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
//    ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
//    ASSERT_EQUAL(groups[2], vector<string>({"port"}));
//    ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
//}

int main(){
//    TestRunner tr;
//    RUN_TEST(tr, TestGroupingABC);
//    RUN_TEST(tr, TestGroupingReal);
    vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl", "caab", "abc", "cccc", "bacc", "c"};
    vector<Group<string>> groups = GroupHeavyStrings(strings);

    for (auto& group : groups) {
        for (const auto& charr: group.front()){
            cout << charr << " ";
        }
        cout << ":";
        for (const auto& str : group) {
            cout << str << " ";
        }
        cout << endl;
    }
    
    return 0;
}
