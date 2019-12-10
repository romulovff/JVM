#pragma once

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <map>

using namespace std;

#define CONSTANT_CLASS 7
#define CONSTANT_FIELD_REF 9
#define CONSTANT_METHOD_REF 10
#define CONSTANT_INTERFACE_METHOD_REF 11
#define CONSTANT_STRING 8
#define CONSTANT_INTEGER 3
#define CONSTANT_FLOAT 4
#define CONSTANT_LONG 5
#define CONSTANT_DOUBLE 6
#define CONSTANT_NAME_AND_TYPE 12
#define CONSTANT_UTF8 1

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;

typedef struct ClassFile ClassFile;

typedef struct Variable Variable;

typedef struct AttributeInfo AttributeInfo;

enum VariableType {
    BOOLEAN,
    BYTE,
    CHAR,
    SHORT,
    INT,
    FLOAT,
    LONG,
    DOUBLE,
    RETURN_ADDR,
    REFERENCE
};

enum ObjectType {
    CLASS,
    STRING,
    ARRAY,
    UNDEFINED
};

class Object;

struct Variable {
    VariableType type;
    union {
        bool v_boolean;
        int8_t v_byte;
        uint8_t v_char;
        int16_t v_short;
        int32_t v_int;
        float v_float;
        int64_t v_long;
        double v_double;
        u4 return_address;
        Object *object;
    } data;
};

struct ClassInfo {
	u2 name_index;
};

struct FieldRefInfo {
    u2 class_index;
    u2 name_and_type_index;
};

struct MethodRefInfo {
    u2 class_index;
    u2 name_and_type_index;
};

struct InterfaceMethodRefInfo {
    u2 class_index;
    u2 name_and_type_index;
};

struct StringInfo {
    u2 string_index;
};

struct IntegerInfo {
    u4 bytes;
};

struct FloatInfo {
    u4 bytes;
};

struct LongInfo {
    u4 high_bytes;
    u4 low_bytes;
};

struct DoubleInfo {
    u4 high_bytes;
    u4 low_bytes;
};

struct NameAndTypeInfo {
	u2 name_index;
	u2 descriptor_index;
};

struct Utf8Info {
    u2 length;
    u1 *bytes;
};

struct Constant_pool_variables {
    u1 tag;
    union {
        ClassInfo class_info;
        FieldRefInfo field_ref_info;
        MethodRefInfo method_ref_info;
        InterfaceMethodRefInfo interface_method_ref_info;
        StringInfo string_info;
        IntegerInfo integer_info;
        FloatInfo float_info;
        LongInfo long_info;
        DoubleInfo double_info;
        NameAndTypeInfo name_and_type_info;
        Utf8Info utf8_info;
    } info;
};

struct ExceptionTable {
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
};

struct LineNumberTable {
    u2 start_pc;
    u2 line_number;
};

struct ExceptionsAttribute {
    u2 number_of_exceptions;
    vector<u2> exception_index_table;
};

struct LineNumberTableAttribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 line_number_table_length;
    vector<LineNumberTable> line_number_table;
};

struct CodeAttribute {
	u2 max_stack;
	u2 max_locals;
	u4 code_length;
	vector<u1> code;
	u2 exception_table_length;
    vector<ExceptionTable> exception_table;
    u2 attributes_count;
    vector<AttributeInfo> attributes;
};

struct SourceFileAttribute {
  	u2 source_file_index;
};

struct ConstantValueAttribute {
    u2 constant_value_index;
};

struct AttributeInfo {
    u2 attribute_name_index;
    u4 attribute_length;

    CodeAttribute code_attribute;
    LineNumberTableAttribute line_number_table_attribute;
    SourceFileAttribute source_file_attribute;
    ConstantValueAttribute constant_value_attribute;
    ExceptionsAttribute exceptions_attribute;
};

struct FieldInfo {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    vector<AttributeInfo> attributes;
};

struct MethodInfo {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    vector<AttributeInfo> attributes;
};

struct ClassFile {
    u4 magic_number;

    u2 min_version;
    u2 major_version;

    u2 constant_pool_length;
    vector<Constant_pool_variables> constant_pool;
    
    u2 access_flags;
    
    u2 this_class;
    
    u2 super_class;
    
    u2 interfaces_count;
    vector<u2> interfaces;
    
    u2 fields_count;
    vector<FieldInfo> fields;
    
    u2 methods_count;
    vector<MethodInfo> methods;
    
    u2 attributes_count;
    vector<AttributeInfo> attributes;
};