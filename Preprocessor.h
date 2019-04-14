//
// Created by asus on 2019/4/8.
//

#ifndef OOP_CPP_PREPROCESSOR_PREPROCESSOR_H
#define OOP_CPP_PREPROCESSOR_PREPROCESSOR_H

#include <string>
#include <vector>
#include <stack>
#include "Node.h"
#include <map>
using namespace std;

class Preprocessor {
private:
    typedef void (Preprocessor::* handle_method)(string line);
    struct handle_struct
    {
        int eType;
        handle_method pFun;
    };
    string pre_code;
    string new_code;
    string sign;
    int which_cmd(const string &line);
    void handle_include(string line);
    static string include_file(const string& path);
    string replace_define(string line);
    map<string,Node> defines;
    void handle_common(string line);
    void handle_define(string line);
    void handle_endif(string line);
    void handle_else(string line);
    void handle_if(string line);
    void handle_ifdef(string line);
    void handle_ifndef(string line);
    void handle_undef(string line);
    void handle(const string& line, const struct handle_struct g_handle_methods[]);
    stack<bool> status; //be used to cope with nesting if-else statements.
public:
    explicit Preprocessor(string code);//it means you can not use Preprocessor preprocessor = oldcodes,where oldcodes'type is string.

    ~Preprocessor();

    string preprocessor();
};


#endif //OOP_CPP_PREPROCESSOR_PREPROCESSOR_H
