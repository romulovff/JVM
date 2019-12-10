#pragma once

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>

#include "types.hpp"
#include "reader.hpp"
#include "utils.hpp"

ExceptionTable get_exception_table(FILE);

LineNumberTable get_line_number_table(FILE);

CodeAttribute get_code_attribute(FILE*, std::vector<Constant_pool_variables>);

LineNumberTableAttribute get_line_number_table_attribute(FILE*, std::vector<Constant_pool_variables>);

AttributeInfo get_attribute_info(FILE*, std::vector<Constant_pool_variables>);