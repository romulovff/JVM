#pragma once

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <math.h>
#include <stack>

#include "types.hpp"
#include "reader.hpp"
#include "attribute.hpp"
#include "array_obj.hpp"

using namespace std;

void check_magic_number(u4);

string format_UTF8(u2, u1*);

string get_constant_pool_element(vector<Constant_pool_variables>, u2);

void format_path(string, string*, string*);

void fill_multi_array(ArrayObject*, VariableType, stack<int>);
