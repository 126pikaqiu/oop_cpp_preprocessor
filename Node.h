//
// Created by asus on 2019/4/9.
/*
 * 用来记住所有的宏的节点，包括宏的名字，参数，返回值，以及是否被取消定义
 */
//

#ifndef OOP_CPP_PREPROCESSOR_NODE_H
#define OOP_CPP_PREPROCESSOR_NODE_H

#include <string>
using namespace std;

class Node {
private:
    string value;
    string argu;
public:
    Node(string value,string argu);
    string get_value();
    string get_argu();
};


#endif //OOP_CPP_PREPROCESSOR_NODE_H
