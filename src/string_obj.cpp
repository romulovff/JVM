#include "string_obj.hpp"

StringObject::StringObject(string s) : v_string(s) {}

ObjectType StringObject::object_type() {
    return STRING;
}