#pragma once

#include "object.hpp"

#include <string>

using namespace std;

class StringObject : public Object {
    
public:
    StringObject(string s = "");
    
    string v_string;

    ObjectType object_type();
};