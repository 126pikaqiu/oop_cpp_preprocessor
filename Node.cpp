//
// Created by asus on 2019/4/9.
//

#include "Node.h"

string Node::getArgu() {
    return argu;
}

string Node::getKey() {
    return key;
}

string Node::getvalue() {
    return value;
}
Node:: Node(string key,string value,string argu,bool isValid){
    this->key = key;
    this->value = value;
    this->argu = argu;
    this->isValid = isValid;
}
Node::Node(string key,string value,string argu){
    this->key = key;
    this->value = value;
    this->argu = argu;
    this->isValid = true;
}

bool Node::getIsValid() {
    return isValid;
}

void Node::setIsValid(bool isValid) {
    this->isValid = isValid;
}
