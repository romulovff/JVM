#pragma once

#include "types.hpp"
#include "object.hpp"

#include <vector>

using namespace std;

class ArrayObject : public Object {
    
public:

    ArrayObject(VariableType);
    
    ObjectType object_type();
    
    VariableType type;
    
    vector<Variable> elements;
};