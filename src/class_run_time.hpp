#pragma once

#include "types.hpp"
#include "utils.hpp"

#include <map>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

class ClassRuntime {

public:

    ClassRuntime(ClassFile*);

    ClassFile *class_file;

    map<string, Variable> static_fields;
};