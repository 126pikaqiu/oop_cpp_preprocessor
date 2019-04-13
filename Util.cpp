//
// Created by asus on 2019/4/9.
//

#include "Util.h"
#include <regex>
#include <iostream>

//原地修改字符串，传入的是左值引用
string trim(string &s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

bool start_with(string target, string line){
    if (target == "#") {
        regex regex2("^" + target);
        return regex_search(line, regex2);
    }
    trim(line);
    string pattern("^\\b" + target + "\\b");
    regex regex1(pattern);
    return regex_search(line, regex1);
}

void split_string(string &s, vector<string> &v, string &c) {   //将字符串按指定字符集分割
    string::size_type pos1, pos2;
    pos2 = s.find_first_of(c);
    pos1 = 0;
    while (string::npos != pos2) {
        if ((pos2 - pos1) != 0u)
            v.push_back(s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = s.find_first_of(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

