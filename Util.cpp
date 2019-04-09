//
// Created by asus on 2019/4/9.
//

#include "Util.h"
#include <regex>
#include <iostream>

//原地修改字符串，传入的是左值引用
void trim(string &s) {
    if (s.empty()) {
        return;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
}

bool startWith(string target, string line){
    trim(line);
    string pattern("^\\b" + target + "\\b");
    regex regex1(pattern);
#ifdef DEBUG
    cout << regex_search(line, regex1);
#endif
    return regex_search(line, regex1);
}

