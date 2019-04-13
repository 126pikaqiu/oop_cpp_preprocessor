//
// Created by asus on 2019/4/8.
//

#include "Preprocessor.h"
#include "Util.h"
#include <iostream>
#include <fstream>
#include <regex>

string cmds[8] = {
        "include",
        "define",
        "if",
        "endif",
        "ifdef",
        "ifndef",
        "undef",
        "else"
};


//之所以这么写是因为 //# include<>  // #include<>都是正确的
int Preprocessor::which_cmd(const string &line) {
    int i = 0;
    for(const string& cmd: cmds){
        i++;
        if(start_with("#",line) && start_with(cmd,line.substr(line.find_first_of('#') + 1))){
            return i;
        }
    }
    return 0;//0表示不需要处理
}

Preprocessor::Preprocessor(string code) {
    this->pre_code = move(code);
    this->new_code = "";
    this->sign = " \t\n\r\f#";
    this->status.push(true);
}

void Preprocessor::handle_include(string line) {
    if(line.find('<') != string::npos || line.find(".h") == string::npos){
        new_code.append(line).push_back('\n');
        return;
    }
    string path("test/");
    int start_index = line.find_first_of('"');
    int end_index = line.find_first_of('"',start_index + 1);
    path.append(line.substr(start_index + 1,end_index - start_index - 1));
    new_code.append(include_file(path));
}

string Preprocessor::include_file(string filename) {
    string file;
    ifstream is(filename);
    string line;
    while (getline(is, line)) {
        file.append(line).push_back('\n');
    }
    is.close();
    return file;
}

//There maybe two types of define
string Preprocessor::replace_define(string line) {
    string tmp = line;
    for (auto it = defines.begin(); it != defines.end();it++) {
        string key = it->first;
        Node node = it->second;
        if (!node.get_argu().empty()) {
//            unsigned long pos1;
//            if ((pos1 = tmp.find(key)) != string::npos) {
//                unsigned long pos2 = tmp.find_first_of("(", pos1);
//                unsigned long pos3 = tmp.find_first_of(")", pos2);
//                string t = tmp.substr(pos2, pos3);
//                string arg = tmp.substr(pos2 + 1, pos3 - pos2 - 1);
//                string tmpArg = node.get_argu();
//                string value = node.get_value();
//                regex regex1("\"#" + tmpArg);
//                value = regex_replace(value, regex1, arg + "\"");
//                regex regex2(tmpArg);
//                value = regex_replace(value, regex2, arg);
//                string pattern(key + "(" + arg + ")");
//                unsigned long index = tmp.find(key);
//                unsigned long index2 = tmp.find(")", index);
//                tmp.replace(index, index2 - index + 1, "");
//                tmp.insert(index, value);
//            }
        } else if(!node.get_value().empty()){
            string pattern{"\\b" + key + "\\b"};
            regex re(pattern);
            tmp = regex_replace(tmp, re, node.get_value());
        }

    }
    return tmp;

}

void Preprocessor::handle_define(string line) {
    if(!status.top()){
        return;
    }
    trim(line);

    if(line.find('(') == string::npos || line.find_first_of(')') == line.size() - 1){
        vector<string> strings;
        split_string(line,strings,sign);
        string value = replace_define(line.substr(line.find_first_of(strings[1]) + strings[1].length()));
        Node node(trim(value),"");//no arguments
        defines.insert(make_pair(strings[1],node));

    }else{
//        vector<string> strings;
//        split_string(line,strings,sign);
//        //todo
    }
}

void Preprocessor::handle_endif(string line) {
    status.pop();
}

void Preprocessor::handle_if(string line) {
    if(!status.top()){
        return;
    }
    vector<string> strings;
    split_string(line,strings,sign);
    status.push(replace_define(strings[1]) == "1");
}

void Preprocessor::handle_else(string line) {
    bool latest_status = status.top();
    status.pop();
    status.push(!latest_status);
}

void Preprocessor::handle_ifdef(string line) {
    if(!status.top()){
        return;
    }
    vector<string> strings;
    split_string(line,strings,sign);
    status.push(defines.count(strings[1]) != 0);
}

void Preprocessor::handle_ifndef(string line) {
    if(!status.top()){
        return;
    }
    vector<string> strings;
    split_string(line,strings,sign);
    status.push(defines.count(strings[1]) == 0);
}

void Preprocessor::handle_undef(string line) {
    if(!status.top()){
        return;
    }
    vector<string> strings;
    split_string(line,strings,sign);
    defines.erase(strings[1]);
}

void Preprocessor::handle_common(string line) {
    if(!status.top()){
        return;
    }
    new_code.append(replace_define(line)).push_back('\n');
}

void Preprocessor::handle(string line, const struct handle_struct g_handle_methods[]) {
    handle_method fun = g_handle_methods[which_cmd(line)].pFun;
    (this->*fun)(line);
}

string Preprocessor::preprocessor() {
    const struct handle_struct g_handle_methods[9] =
            {{0,(handle_method)(&Preprocessor::handle_common)},{1,(handle_method)(&Preprocessor::handle_include)},{2,(handle_method)(&Preprocessor::handle_define)}
                    ,{3,(handle_method)(&Preprocessor::handle_if)},{4,(handle_method)(&Preprocessor::handle_endif)},{5,(handle_method)(&Preprocessor::handle_ifdef)}
                    ,{6,(handle_method)(&Preprocessor::handle_ifndef)},{7,(handle_method)(&Preprocessor::handle_undef)},{8,(handle_method)(&Preprocessor::handle_else)}};
    vector<string> lines;
    string sign = "\n";
    split_string(pre_code, lines, sign); //将源文件切割成行
    for (auto line :lines) {
        handle(line,g_handle_methods);
    }
    return new_code;
}

Preprocessor::~Preprocessor() = default;
