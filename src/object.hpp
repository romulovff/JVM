#pragma once

#include "types.hpp"

class Object {
    virtual ObjectType object_type() = 0;
};