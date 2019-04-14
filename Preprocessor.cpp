//
// Created by asus on 2019/4/8.
//

#include "Preprocessor.h"
#include "Util.h"
#include <iostream>
#include <fstream>
#include <regex>

/**
 *
 * According to the document, there some 8
 * command lines to be handled. There chooses
 * an array to store them.
 */
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

/**
 * The method aims to decide the line contains which command.
 * And it's based the assumption that every line can only contain
 * at most one command.
 * Considering that both "# include<...>" and " # include<...>"
 * are OK. So I need to check whether it begins with "#" first,
 * then check whether the left begins with specific command word.
 * @param line
 * @return
 */
int Preprocessor::which_cmd(const string &line) {
    int i = 0;
    for(const string& cmd: cmds){
        i++;
        if(start_with("#",line) && start_with(cmd,line.substr(line.find_first_of('#') + 1))){
            return i;
        }
    }
    return 0;//0表示不需要处理，有特殊用处
}

/**
 * Constructor method.
 * Store the original codes.
 * Set the sign, which is used in spliting string.
 * Push a true value into the stack and now the process
 * can go on.
 * @param code
 */
Preprocessor::Preprocessor(string code) {
    this->pre_code = move(code);
    this->new_code = "";
    this->sign = " \t\n\r\f#";
    this->status.push(true);
}

void Preprocessor::handle_include(string line) {
    if(!status.top()){
        return;
    }
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

string Preprocessor::include_file(const string& filename) {
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
    for (auto & define : defines) {
        string key = define.first;
        Node node = define.second;
        if (!node.get_argu().empty()) {
            int pos1;
            if ((pos1 = tmp.find(key)) != string::npos) {
                int pos2 = tmp.find_first_of('(', pos1);
                int pos3 = tmp.find_first_of(')', pos2);
                string arg = tmp.substr(pos2 + 1, pos3 - pos2 - 1);
                string tmpArg = node.get_argu();
                string value = node.get_value();

                regex regex1("\"#" + tmpArg);
                value = regex_replace(value, regex1, arg + "\"");

                regex regex2(tmpArg);
                value = regex_replace(value, regex2, arg);
                if(value.find("##") != string::npos){
                    int pos = value.find("##");
                    string start = value.substr(0,pos);
                    string end = value.substr(pos + 2);
                    value = trim(start) + trim(end);
                }
                int index = tmp.find(key);
                int index2 = tmp.find(')', index);
                tmp.replace(index, index2 - index + 1, "");
                tmp.insert(index, value);
            }
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
        string sign1("()");
        vector<string> strings;
        split_string(line,strings,sign1);
        string value;
        value = strings[2];
        if(strings.size() >= 4){
            value = strings[3];
        }
        Node node(value,trim(strings[1]));
        vector<string> strings1;
        split_string(strings[0],strings1,sign);
        defines.insert(make_pair(strings1[1],node));
    }
}

/**
 * Need only pop the status and the status will go back to the high level.
 * @param line
 */
void Preprocessor::handle_endif(string line) {
    status.pop();
}

void Preprocessor::handle_if(string line) {
    vector<string> strings;
    split_string(line,strings,sign);
    status.push(replace_define(strings[1]) == "1" && status.top());
}

void Preprocessor::handle_else(string line) {
    bool latest_status = status.top();
    status.pop();
    status.push((!latest_status) && status.top());
}

void Preprocessor::handle_ifdef(string line) {
    vector<string> strings;
    split_string(line,strings,sign);
    status.push(defines.count(strings[1]) != 0  && status.top());
}

void Preprocessor::handle_ifndef(string line) {
    vector<string> strings;
    split_string(line,strings,sign);
    status.push(defines.count(strings[1]) == 0 && status.top());
}

/**
 * checking status is necessary.
 * O
 * @param line
 */
void Preprocessor::handle_undef(string line) {
    if(!status.top()){
        return;
    }
    vector<string> strings;
    split_string(line,strings,sign);
    defines.erase(strings[1]);
}

/**
 * check whether the line could be included.
 * If so, just replace some defined words.
 * Otherwise, just ignore it.
 * @param line raw text to be handled
 */
void Preprocessor::handle_common(string line) {
    if(!status.top()){
        return;
    }
    new_code.append(replace_define(std::move(line))).push_back('\n');
}

/**
 * According to the type of line, it will choose a proper handle method.
 * @param line raw text to be handled
 * @param g_handle_methods the structure containing the handle method
 */
void Preprocessor::handle(const string& line, const struct handle_struct g_handle_methods[]) {
    handle_method fun = g_handle_methods[which_cmd(line)].pFun;
    (this->*fun)(line);
}

/**
 * the main interface of the class.
 * It divides the text into raws to handle.
 * Using function pointer rather than switch-case
 * statements can improve the performance of program.
 *
 * @return the handled text.
 */
string Preprocessor::preprocessor() {
    const struct handle_struct g_handle_methods[9] =
            {{0,(handle_method)(&Preprocessor::handle_common)},{1,(handle_method)(&Preprocessor::handle_include)},{2,(handle_method)(&Preprocessor::handle_define)}
                    ,{3,(handle_method)(&Preprocessor::handle_if)},{4,(handle_method)(&Preprocessor::handle_endif)},{5,(handle_method)(&Preprocessor::handle_ifdef)}
                    ,{6,(handle_method)(&Preprocessor::handle_ifndef)},{7,(handle_method)(&Preprocessor::handle_undef)},{8,(handle_method)(&Preprocessor::handle_else)}};
    vector<string> lines;
    string basicString = "\n";
    split_string(pre_code, lines, basicString);
    for (const auto& line :lines) {
        handle(line,g_handle_methods);
    }
    return new_code;
}

/**
 * no malloc no free
 * no new no delete
 */
Preprocessor::~Preprocessor() = default;
