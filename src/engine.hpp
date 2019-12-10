#pragma once

#include "types.hpp"
#include "class_loader.hpp"
#include "instructions.hpp"

class Engine {

public:
    void start(ClassRuntime*);
    
};

void execute_instruction(stack<Frame>*, u1);

