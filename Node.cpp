//
// Created by asus on 2019/4/9.
//

#include "Node.h"

string Node::get_argu() {
    return argu;
}

string Node::get_value() {
    return value;
}

Node::Node(string value,string argu){
    this->value = move(value);
    this->argu = move(argu);
}

