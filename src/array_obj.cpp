#include "array_obj.hpp"

ArrayObject::ArrayObject(VariableType type) : type(type) {}

ObjectType ArrayObject::object_type() {
    return ARRAY;
}