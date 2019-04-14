//
// Created by asus on 2019/4/9.
//

#ifndef OOP_CPP_PREPROCESSOR_UTIL_H
#define OOP_CPP_PREPROCESSOR_UTIL_H

#include <string>
#include <vector>

using namespace std;

void split_string(string &string1, vector<string> &container, string &signs);
string trim(string &s);
bool start_with(const string& target, string line);



#endif //OOP_CPP_PREPROCESSOR_UTIL_H

