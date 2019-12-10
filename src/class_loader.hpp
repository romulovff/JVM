#pragma once

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <math.h>
#include <map>

#include "types.hpp"
#include "reader.hpp"
#include "attribute.hpp"
#include "utils.hpp"
#include "class_run_time.hpp"

void set_path_files(string);

ClassRuntime* build_class(string);

void set_magic_number(FILE*, ClassFile*);
void set_version(FILE*, ClassFile*);
void set_constant_pool(FILE*, ClassFile*);
void set_access_flags(FILE*, ClassFile*);
void set_this_class(FILE*, ClassFile*);
void set_super_class(FILE*, ClassFile*);
void set_interfaces(FILE*, ClassFile*);
void set_fields(FILE*, ClassFile*);
void set_methods(FILE*, ClassFile*);
void set_attributes(FILE*, ClassFile*);

void check_class_name(ClassFile*, string);
