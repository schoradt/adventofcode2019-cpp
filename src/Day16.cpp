
#include "Day16.h"

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <unordered_map>

using namespace std;

Day16::Day16() {

}

long Day16::process1(vector<string> input) {
    map<string, vector<int>> rules;

    // parse rules
    for (string line: input) {
        if (line == "") {
            // rules section finished
            break;
        }

        int index1 = line.find(":");

        string name = line.substr(0, index1);

        int index2 = line.find_first_of('-', index1);

        vector<int> rule;

        rule.push_back(atoi(line.substr(index1 + 2, index2 - index1 + 2).c_str()));

        index1 = line.find_first_of(' ', index2);

        rule.push_back(atoi(line.substr(index2 + 1, index1 - index2).c_str()));

        index2 = line.find_first_of('-', index1);

        rule.push_back(atoi(line.substr(index1 + 4, index2 - index1 + 4).c_str()));
        rule.push_back(atoi(line.substr(index2 + 1).c_str()));

        rules[name] = rule;
    }

    // for (auto &e: rules) {
    //     cout << e.first << " -> ";

    //     for (auto i: e.second) {
    //         cout << i << " ";
    //     }

    //     cout << endl;
    // }

    // parse tickets
    bool start = false;

    long sum = 0;

    for (string line: input) {
        if (line == "nearby tickets:") {
            start = true;

            continue;
        } else if (!start) {
            continue;
        }

        vector<int> ticket = parseIntegerLines(splitLine(line));

        for (int field: ticket) {
            bool check = false;

            for (auto &r: rules) {
                for (size_t i = 0; i < r.second.size(); i += 2) {
                    if (field >= r.second[i] && field <= r.second[i+1]) {
                        check = true;
                        break;
                    }
                }

                if (check) break;
            }

            if (!check) {
                sum += field;
            }
        }
    }

    return sum;
}

long Day16::process2(vector<string> input) {
    map<string, vector<int>> rules;

    set<string> names;

    // parse rules
    for (string line: input) {
        if (line == "") {
            // rules section finished
            break;
        }

        int index1 = line.find(":");

        string name = line.substr(0, index1);

        names.insert(name);

        int index2 = line.find_first_of('-', index1);

        vector<int> rule;

        rule.push_back(atoi(line.substr(index1 + 2, index2 - index1 + 2).c_str()));

        index1 = line.find_first_of(' ', index2);

        rule.push_back(atoi(line.substr(index2 + 1, index1 - index2).c_str()));

        index2 = line.find_first_of('-', index1);

        rule.push_back(atoi(line.substr(index1 + 4, index2 - index1 + 4).c_str()));
        rule.push_back(atoi(line.substr(index2 + 1).c_str()));

        rules[name] = rule;
    }

    // for (auto &e: rules) {
    //     cout << e.first << " -> ";

    //     for (auto i: e.second) {
    //         cout << i << " ";
    //     }

    //     cout << endl;
    // }

    // parse tickets
    bool start = false;
    bool my = false;

    vector<set<string> > candidates;
    vector<int> myTicket;

    for (string line: input) {
        if (line == "your ticket:") {
            my = true;

            continue;
        } else if (my) {
            myTicket = parseIntegerLines(splitLine(line));

            for (int v: myTicket) {
                (void)(v);

                set<string> copy = names;

                candidates.push_back(copy);
            }

            my = false;

            continue;
        } else if (line == "nearby tickets:") {
            start = true;

            continue;
        } else if (!start) {
            continue;
        }

        vector<int> ticket = parseIntegerLines(splitLine(line));

        bool fail = false;

        for (int field: ticket) {
            bool check = false;

            for (auto &r: rules) {
                for (size_t i = 0; i < r.second.size(); i += 2) {
                    if (field >= r.second[i] && field <= r.second[i+1]) {
                        check = true;
                        break;
                    }
                }

                if (check) break;
            }

            if (!check) {
                fail = true;

                break;
            }
        }

        if (fail) continue;

        int index = 0;

        for (int field: ticket) {
            for (auto &r: rules) {
                bool ok = false;

                for (size_t i = 0; i < r.second.size(); i += 2) {
                    if (field >= r.second[i] && field <= r.second[i+1]) {
                        ok = true;
                    }
                }

                if (!ok) {
                    candidates[index].erase(r.first);
                }
            }

            index++;
        }
    }

    long prod = 1;

    for (size_t i = 0; i < myTicket.size(); i++) {
        for (size_t j = 0; j < candidates.size(); j++) {
            if (candidates[j].size() == 1) {
                string name = *(candidates[j].begin());

                //cout << name << ": " << myTicket[j] << endl;

                if (name.rfind("departure", 0) == 0) {
                    prod = prod * myTicket[j];
                }

                for (size_t k = 0; k < candidates.size(); k++) {
                    candidates[k].erase(name);
                }
            }
        }
    }

    return prod;
}
