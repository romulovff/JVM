#pragma once

#include "types.hpp"
#include "object.hpp"

#include "class_run_time.hpp"

#include <map>
#include <string>

using namespace std;

class ClassObject : public Object {
    
public:

    ClassObject(ClassRuntime *class_run_time);
    
    ObjectType object_type();
    
    ClassRuntime *class_run_time;
    
    map<string, Variable> fields;
};