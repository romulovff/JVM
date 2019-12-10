#pragma once

#include <map>
#include <stack>

#include "types.hpp"
#include "class_run_time.hpp"
#include "utils.hpp"

class Frame {

public:

    Frame(ClassRuntime*, string, string, vector<Variable>);

    u4 pc;
    map<u4, Variable> local_variables;
    stack<Variable> operand_stack;
    ClassRuntime *class_run_time;
    MethodInfo method;
    CodeAttribute code_attribute;
    ExceptionsAttribute exceptions_attribute;

    u1 get_method_code(u4 index);

    void get_method(ClassFile, std::string, std::string);
    void get_attributes();

};