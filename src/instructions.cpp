#include "instructions.hpp"

void nop(stack<Frame>* frame_stack) {
    frame_stack->top().pc += 1;
}

void ldc2_w(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();
    
    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);
    u2 index = (byte1 << 8) | byte2;
    
    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;
    Constant_pool_variables cp_element = constant_pool[index];

    if (cp_element.tag == CONSTANT_LONG) {
        Variable long_variable;

        u4 high_bytes = cp_element.info.long_info.high_bytes;
        u4 low_bytes = cp_element.info.long_info.low_bytes;
        
        int64_t number = ((int64_t) high_bytes << 32) + low_bytes;
        long_variable.type = VariableType::LONG;
        long_variable.data.v_long = number;

        frame_stack->top().operand_stack.push(long_variable);
        
    } else if (cp_element.tag == CONSTANT_DOUBLE) {
        Variable double_variable;

        u4 highBytes = cp_element.info.double_info.high_bytes;
        u4 lowBytes = cp_element.info.double_info.low_bytes;
        
        int64_t bytes = ((int64_t) highBytes << 32) + lowBytes;
        
        int32_t sig = ((bytes >> 63) == 0) ? 1 : -1;
        int32_t exponent = (int32_t)((bytes >> 52) & 0x7ffL);
        int64_t mantissa = (exponent == 0) ? (bytes & 0xfffffffffffffL) << 1 : (bytes & 0xfffffffffffffL) | 0x10000000000000L;
        double number = sig * mantissa * pow(2, exponent-1075);
        
        double_variable.type = VariableType::DOUBLE;
        double_variable.data.v_double = number;
        
        frame_stack->top().operand_stack.push(double_variable);
    }
    
    frame_stack->top().pc += 3;
}

void dstore_1(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();
    
    Variable variable = curr_frame.operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[1] = variable;
    frame_stack->top().pc += 1;
}

void dstore_3(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();
    
    Variable variable = curr_frame.operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[3] = variable;
    frame_stack->top().pc += 1;
}

void dload(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
	u2 index = (int16_t) byte1;

    // TODO pensar como passar se o wide foi setado ou nao !!


	Variable variable = curr_frame.local_variables[index];

	frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 2;
}

void dload_1(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

	Variable variable = curr_frame.local_variables[1];
	frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void dsub(stack<Frame>* frame_stack) {
	Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = DOUBLE;
    result.data.v_double = (variable_1.data.v_double) - (variable_2.data.v_double);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void dadd(stack<Frame>* frame_stack) {
	Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = DOUBLE;
    result.data.v_double = (variable_1.data.v_double) + (variable_2.data.v_double);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void dmul(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = DOUBLE;
    result.data.v_double = (variable_1.data.v_double) * (variable_2.data.v_double);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void ddiv(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    if(variable_2.data.v_double == 0) {
        cerr << "divisao por 0" << endl;
        exit(1);
    }

    Variable result;
    result.type = DOUBLE;
    result.data.v_double = (variable_1.data.v_double) / (variable_2.data.v_double);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void dneg(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable result;
    result.type = DOUBLE;
    result.data.v_double = -(variable.data.v_double);
	
    frame_stack->top().operand_stack.push(result);
    frame_stack->top().pc += 1;
}

void drem(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    if(variable_2.data.v_double == 0) {
        cerr << "divisao por 0" << endl;
        exit(1);
    }

    Variable result;
    result.type = DOUBLE;
    result.data.v_double = variable_1.data.v_double - ((uint64_t)(variable_1.data.v_double / variable_2.data.v_double))*variable_2.data.v_double;
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void invokevirtual(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);
    u2 method_index = (byte1 << 8) | byte2;

    Constant_pool_variables cp_method = constant_pool[method_index];
    MethodRefInfo method_info = cp_method.info.method_ref_info;

    string class_name = get_constant_pool_element(constant_pool, method_info.class_index);

    Constant_pool_variables cp_name_and_type = constant_pool[method_info.name_and_type_index];
    NameAndTypeInfo method_name_and_type = cp_name_and_type.info.name_and_type_info;

    string method_name = get_constant_pool_element(constant_pool, method_name_and_type.name_index);
    string method_descriptor = get_constant_pool_element(constant_pool, method_name_and_type.descriptor_index);

    // cout << class_name << " " << method_name << " " << method_descriptor << endl;

    if (class_name.find("java/") != string::npos) {
        if (class_name == "java/io/PrintStream" && (method_name == "print" || method_name == "println")) {
            if (method_descriptor != "()V") {
                Variable variable = frame_stack->top().operand_stack.top();
                frame_stack->top().operand_stack.pop();

                switch (variable.type) {
                    case BOOLEAN:
                        printf("%s", variable.data.v_boolean == 0 ? "false" : "true");
                        break;
                    case BYTE:
                        printf("%d", variable.data.v_byte);
                        break;
                    case CHAR:
                        printf("%c", variable.data.v_char);
                        break;
                    case SHORT:
                        printf("%d", variable.data.v_short);
                        break;
                    case INT:
                        printf("%d", variable.data.v_int);
                        break;
                    case DOUBLE:
                        printf("%f", variable.data.v_double);
                        break;
                    case FLOAT:
                        printf("%f", variable.data.v_float);
                        break;
                    case LONG:
                        printf("%lld", (long long) variable.data.v_long);
                        break;
                    case REFERENCE:
                        StringObject* print_string = (StringObject *) variable.data.object;
                        printf("%s", print_string->v_string.c_str());
                        break;
                }
            }

            if (method_name == "println") printf("\n");
        } else if(class_name == "java/lang/StringBuffer") {
            if(method_name == "append") {
                Variable variable = frame_stack->top().operand_stack.top();
                // frame_stack->top().operand_stack.pop();

                StringObject* print_string = (StringObject *) variable.data.object;
                printf("%s", print_string->v_string.c_str());
            } else if(method_name == "toString") {

                // StringBuffer TA ERRADO MAS FUNCIONA !

                // Variable variable = frame_stack->top().operand_stack.top();
                // frame_stack->top().operand_stack.pop();

                // StringObject* print_string = (StringObject *) variable.data.object;
                // printf(" AAA%s", print_string->v_string.c_str());
            }
            
            
        } else {
            cout << "nao sei implementar isso ainda, tem haver com string object" << endl;
            exit(1);
        }
    } else {
        uint16_t args_count = 0; 
        uint16_t i = 1; // skip '('
        while (method_descriptor[i] != ')') {
            char base_type = method_descriptor[i];
            if (base_type == 'D' || base_type == 'J') {
                args_count += 2;
            } else if (base_type == 'L') {
                args_count++;
                while (method_descriptor[++i] != ';');
            } else if (base_type == '[') {
                args_count++;
                while (method_descriptor[++i] == '[');
                if (method_descriptor[i] == 'L') while (method_descriptor[++i] != ';');
            } else {
                args_count++;
            }
            i++;
        }

        vector<Variable> args;
        for (int i = 0; i < args_count; i++) {
            Variable variable = frame_stack->top().operand_stack.top();
            frame_stack->top().operand_stack.pop();
            args.push_back(variable);

            // simulate a padding
            if(variable.type == DOUBLE || variable.type == LONG) i++;
        }

        Variable object_variable = frame_stack->top().operand_stack.top();
        frame_stack->top().operand_stack.pop();

        args.push_back(object_variable);
        reverse(args.begin(), args.end());

        ClassRuntime *class_run_time = build_class(class_name);

        frame_stack->top().pc += 3;

        frame_stack->push(Frame(class_run_time, method_name, method_descriptor, args));

        return;
    }

    frame_stack->top().pc += 3;
}

void return_instruction(stack<Frame>* frame_stack) {
    frame_stack->pop();
}

void bipush(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

    u1 byte = curr_frame.get_method_code(curr_frame.pc + 1);

    Variable variable;
    variable.type = VariableType::INT;
    variable.data.v_int = (int32_t) (int8_t) byte;

    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 2;
}

void istore_1(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[1] = variable;
    frame_stack->top().pc += 1;
}

void aconst_null(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = REFERENCE;
    variable.data.object = NULL;

    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void iconst_m1(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::INT;
    variable.data.v_int = -1;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void iconst_0(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::INT;
    variable.data.v_int = 0;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void iconst_1(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::INT;
    variable.data.v_int = 1;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void iconst_2(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::INT;
    variable.data.v_int = 2;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void iconst_3(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::INT;
    variable.data.v_int = 3;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void iconst_4(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::INT;
    variable.data.v_int = 4;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void iconst_5(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::INT;
    variable.data.v_int = 5;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void lconst_0(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::LONG;
    variable.data.v_long = 0;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void lconst_1(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::LONG;
    variable.data.v_long = 1;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void fconst_0(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::FLOAT;
    variable.data.v_float = 0;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void fconst_1(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::FLOAT;
    variable.data.v_float = 1;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void fconst_2(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::FLOAT;
    variable.data.v_float = 2;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void dconst_0(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::DOUBLE;
    variable.data.v_double = 0;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void dconst_1(stack<Frame>* frame_stack) {
    Variable variable;
    variable.type = VariableType::DOUBLE;
    variable.data.v_double = 1;

    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 1;
}

void sipush(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);
    uint16_t short_value = (byte1 << 8) | byte2;

    Variable variable;
    variable.type = VariableType::INT;
    variable.data.v_int = (int32_t) (int16_t) short_value;

    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 3;
}

void ldc(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

    u1 index = curr_frame.get_method_code(curr_frame.pc + 1);
    
    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;
    Constant_pool_variables cp_element = constant_pool[index];

    Variable variable;
    
    if (cp_element.tag == CONSTANT_STRING) {
        Constant_pool_variables utf8_variable = constant_pool[cp_element.info.string_info.string_index];
        
        u1* bytes = utf8_variable.info.utf8_info.bytes;
        string utf8_string;

        for (int i = 0; i < utf8_variable.info.utf8_info.length; i++) {
            utf8_string += char(bytes[i]);
        }

        variable.type = REFERENCE;
        variable.data.object = new StringObject(utf8_string);

    } else if (cp_element.tag == CONSTANT_INTEGER) {
        variable.type = VariableType::INT;
        variable.data.v_int = cp_element.info.integer_info.bytes;

    } else if (cp_element.tag == CONSTANT_FLOAT) {
        u4 float_bytes = cp_element.info.float_info.bytes;
        int s = ((float_bytes >> 31) == 0) ? 1 : -1;
        int e = ((float_bytes >> 23) & 0xff);
        int m = (e == 0) ? (float_bytes & 0x7fffff) << 1 : (float_bytes & 0x7fffff) | 0x800000;
        
        float number = s*m*pow(2, e-150);
        variable.type = VariableType::FLOAT;
        variable.data.v_float = number;
    }
    
    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 2;
}

void ldc_w(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();
    
    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);
    u2 index = (byte1 << 8) | byte2;
    
    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;
    Constant_pool_variables cp_element = constant_pool[index];

    Variable variable;

    if (cp_element.tag == CONSTANT_STRING) {
        Constant_pool_variables utf8_variable = constant_pool[cp_element.info.string_info.string_index];
        
        u1* bytes = utf8_variable.info.utf8_info.bytes;
        string utf8_string;

        for (int i = 0; i < utf8_variable.info.utf8_info.length; i++) {
            utf8_string += char(bytes[i]);
        }

        variable.type = REFERENCE;
        variable.data.object = new StringObject(utf8_string);
        
    } else if (cp_element.tag == CONSTANT_INTEGER) {
        variable.type = VariableType::INT;
        variable.data.v_int = cp_element.info.integer_info.bytes;
    } else if (cp_element.tag == CONSTANT_FLOAT) {
        u4 float_bytes = cp_element.info.float_info.bytes;
        int s = ((float_bytes >> 31) == 0) ? 1 : -1;
        int e = ((float_bytes >> 23) & 0xff);
        int m = (e == 0) ? (float_bytes & 0x7fffff) << 1 : (float_bytes & 0x7fffff) | 0x800000;
        
        float number = s*m*pow(2, e-150);
        variable.type = VariableType::FLOAT;
        variable.data.v_float = number;
    }
    
    frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 3;
}

void iload(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
	u2 index = (int16_t) byte1;

    // TODO pensar como passar se o wide foi setado ou nao !!

	Variable variable = curr_frame.local_variables[index];
	
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 2;
}

void lload(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
	u2 index = (int16_t) byte1;

    // TODO pensar como passar se o wide foi setado ou nao !!

	Variable variable = curr_frame.local_variables[index];
	frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 2;
}

void fload(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
	u2 index = (int16_t) byte1;

    // TODO pensar como passar se o wide foi setado ou nao !!

	Variable variable = curr_frame.local_variables[index];
	frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 2;
}

void aload(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
	u2 index = (int16_t) byte1;

    // TODO pensar como passar se o wide foi setado ou nao !!

	Variable variable = curr_frame.local_variables[index];
	frame_stack->top().operand_stack.push(variable);
    frame_stack->top().pc += 2;
}

void iload_0(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[0];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void iload_1(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[1];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void iload_2(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[2];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void iload_3(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[3];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void lload_0(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[0];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void lload_1(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[1];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void lload_2(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[2];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void lload_3(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[3];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void fload_0(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[0];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void fload_1(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[1];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void fload_2(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[2];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void fload_3(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[3];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void dload_0(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[0];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void dload_2(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[2];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void dload_3(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[3];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void aload_0(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[0];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void aload_1(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[1];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void aload_2(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[2];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void aload_3(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().local_variables[3];
    frame_stack->top().operand_stack.push(variable);

    frame_stack->top().pc += 1;
}

void istore(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

	Variable variable = curr_frame.operand_stack.top(); 
    frame_stack->top().operand_stack.pop();

    u1 byte = curr_frame.get_method_code(curr_frame.pc + 1);
    u2 index = (int16_t) byte;

    frame_stack->top().local_variables[index] = variable;
    
    // TODO pensar como passar se o wide foi setado ou nao !!

    frame_stack->top().pc += 2;
}

void lstore(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

	Variable variable = curr_frame.operand_stack.top(); 
    frame_stack->top().operand_stack.pop();

    u1 byte = curr_frame.get_method_code(curr_frame.pc + 1);
    u2 index = (int16_t) byte;

    frame_stack->top().local_variables[index] = variable;
    
    // TODO pensar como passar se o wide foi setado ou nao !!

    frame_stack->top().pc += 2;
}

void fstore(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

	Variable variable = curr_frame.operand_stack.top(); 
    frame_stack->top().operand_stack.pop();

    u1 byte = curr_frame.get_method_code(curr_frame.pc + 1);
    u2 index = (int16_t) byte;

    frame_stack->top().local_variables[index] = variable;
    
    // TODO pensar como passar se o wide foi setado ou nao !!

    frame_stack->top().pc += 2;
}

void dstore(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

	Variable variable = curr_frame.operand_stack.top(); 
    frame_stack->top().operand_stack.pop();

    u1 byte = curr_frame.get_method_code(curr_frame.pc + 1);
    u2 index = (int16_t) byte;

    frame_stack->top().local_variables[index] = variable;
    
    // TODO pensar como passar se o wide foi setado ou nao !!

    frame_stack->top().pc += 2;
}

void astore(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

	Variable variable = curr_frame.operand_stack.top(); 
    frame_stack->top().operand_stack.pop();

    u1 byte = curr_frame.get_method_code(curr_frame.pc + 1);
    u2 index = (int16_t) byte;

    frame_stack->top().local_variables[index] = variable;
    
    // TODO pensar como passar se o wide foi setado ou nao !!

    frame_stack->top().pc += 2;
}

void istore_0(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[0] = variable;
    frame_stack->top().pc += 1;
}

void istore_2(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[2] = variable;
    frame_stack->top().pc += 1;
}

void istore_3(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[3] = variable;
    frame_stack->top().pc += 1;
}

void lstore_0(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[0] = variable;
    frame_stack->top().pc += 1;
}

void lstore_1(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[1] = variable;
    frame_stack->top().pc += 1;
}

void lstore_2(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[2] = variable;
    frame_stack->top().pc += 1;
}

void lstore_3(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[3] = variable;
    frame_stack->top().pc += 1;
}

void fstore_0(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[0] = variable;
    frame_stack->top().pc += 1;
}

void fstore_1(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[1] = variable;
    frame_stack->top().pc += 1;
}

void fstore_2(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[2] = variable;
    frame_stack->top().pc += 1;
}

void fstore_3(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[3] = variable;
    frame_stack->top().pc += 1;
}

void dstore_0(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[0] = variable;
    frame_stack->top().pc += 1;
}

void dstore_2(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[2] = variable;
    frame_stack->top().pc += 1;
}

void astore_0(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[0] = variable;
    frame_stack->top().pc += 1;
}

void astore_1(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[1] = variable;
    frame_stack->top().pc += 1;
}

void astore_2(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[2] = variable;
    frame_stack->top().pc += 1;
}

void astore_3(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    frame_stack->top().local_variables[3] = variable;
    frame_stack->top().pc += 1;
}

void pop(stack<Frame>* frame_stack) {
    frame_stack->top().operand_stack.pop();
    frame_stack->top().pc += 1;
}

void pop2(stack<Frame>* frame_stack) {
    frame_stack->top().operand_stack.pop();
    frame_stack->top().pc += 1;
}

void dup(stack<Frame>* frame_stack) {
    Variable top_variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.push(top_variable);
    
    frame_stack->top().pc += 1;
}

void dup_x1(stack<Frame>* frame_stack) {
    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    frame_stack->top().operand_stack.push(variable_1);
    frame_stack->top().operand_stack.push(variable_2);
    frame_stack->top().operand_stack.push(variable_1);
    
    frame_stack->top().pc += 1;
}

void dup_x2(stack<Frame>* frame_stack) {
    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    frame_stack->top().operand_stack.push(variable_1);
    frame_stack->top().operand_stack.push(variable_2);
    frame_stack->top().operand_stack.push(variable_1);
    
    frame_stack->top().pc += 1;
}

void swap(stack<Frame>* frame_stack) {
    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    frame_stack->top().operand_stack.push(variable_1);
    frame_stack->top().operand_stack.push(variable_2);
    
    frame_stack->top().pc += 1;
}

void iadd(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = INT;
    result.data.v_int = (variable_1.data.v_int) + (variable_2.data.v_int);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void ladd(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = LONG;
    result.data.v_long = (variable_1.data.v_long) + (variable_2.data.v_long);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void fadd(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = FLOAT;
    result.data.v_float = (variable_1.data.v_float) + (variable_2.data.v_float);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void isub(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = INT;
    result.data.v_int = (variable_1.data.v_int) - (variable_2.data.v_int);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void lsub(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = LONG;
    result.data.v_long = (variable_1.data.v_long) - (variable_2.data.v_long);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void fsub(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = FLOAT;
    result.data.v_float = (variable_1.data.v_float) - (variable_2.data.v_float);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void imul(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = INT;
    result.data.v_int = (variable_1.data.v_int) * (variable_2.data.v_int);

    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void lmul(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = LONG;
    result.data.v_long = (variable_1.data.v_long) * (variable_2.data.v_long);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void fmul(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = FLOAT;
    result.data.v_float = (variable_1.data.v_float) * (variable_2.data.v_float);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void idiv(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = INT;
    result.data.v_int = (variable_1.data.v_int) / (variable_2.data.v_int);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void ldiv_instruction(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = LONG;
    result.data.v_long = (variable_1.data.v_long) / (variable_2.data.v_long);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void fdiv(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = FLOAT;
    result.data.v_float = (variable_1.data.v_float) / (variable_2.data.v_float);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void irem(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = INT;
    result.data.v_int = (variable_1.data.v_int) % (variable_2.data.v_int);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void lrem(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = LONG;
    result.data.v_long = (variable_1.data.v_long) % (variable_2.data.v_long);
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void frem(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;
    result.type = FLOAT;
    result.data.v_float = variable_1.data.v_float - ((uint64_t)(variable_1.data.v_float / variable_2.data.v_float))*variable_2.data.v_float;
	
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void ineg(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable result;
    result.type = INT;
    result.data.v_int = -(variable.data.v_int);
	
    frame_stack->top().operand_stack.push(result);
    frame_stack->top().pc += 1;
}

void lneg(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable result;
    result.type = LONG;
    result.data.v_long = -(variable.data.v_long);
	
    frame_stack->top().operand_stack.push(result);
    frame_stack->top().pc += 1;
}

void fneg(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable result;
    result.type = FLOAT;
    result.data.v_float = -(variable.data.v_float);
	
    frame_stack->top().operand_stack.push(result);
    frame_stack->top().pc += 1;
}

void ishl(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;

    variable_2.data.v_int = 0x1f & variable_2.data.v_int;
	result.type = INT;
    result.data.v_int = variable_1.data.v_int << variable_2.data.v_int;
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void lshl(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;

    variable_2.data.v_long = 0x3f & variable_2.data.v_long;
	result.type = LONG;
    result.data.v_long = variable_1.data.v_long << variable_2.data.v_long;
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void ishr(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;

    variable_2.data.v_int = 0x1f & variable_2.data.v_int;
	result.type = INT;
    result.data.v_int = variable_1.data.v_int >> variable_2.data.v_int;
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void lshr(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;

    variable_2.data.v_int = 0x1f & variable_2.data.v_int;
	result.type = LONG;
    result.data.v_long = variable_1.data.v_long >> variable_2.data.v_long;
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void iushr(stack<Frame>* frame_stack) {
    cout << "falta implementar iushr" << endl;
    exit(1);
}

void lushr(stack<Frame>* frame_stack) {
    cout << "falta implementar lushr" << endl;
    exit(1);
}

void iand(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;

    variable_2.data.v_int = 0x1f & variable_2.data.v_int;
	result.type = INT;
    result.data.v_int = variable_1.data.v_int & variable_2.data.v_int;
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void land(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;

    variable_2.data.v_long = 0x1f & variable_2.data.v_long;
	result.type = LONG;
    result.data.v_long = variable_1.data.v_long & variable_2.data.v_long;
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;
}

void ior(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;

    variable_2.data.v_int = 0x1f & variable_2.data.v_int;
	result.type = INT;
    result.data.v_int = variable_1.data.v_int | variable_2.data.v_int;
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;   
}

void lor(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;

    variable_2.data.v_long = 0x1f & variable_2.data.v_long;
	result.type = LONG;
    result.data.v_long = variable_1.data.v_long | variable_2.data.v_long;
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;   
}

void ixor(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;

    variable_2.data.v_int = 0x1f & variable_2.data.v_int;
	result.type = INT;
    result.data.v_int = variable_1.data.v_int ^ variable_2.data.v_int;
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;   
}

void lxor(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable result;

    variable_2.data.v_int = 0x1f & variable_2.data.v_int;
	result.type = INT;
    result.data.v_int = variable_1.data.v_int ^ variable_2.data.v_int;
    frame_stack->top().operand_stack.push(result);

    frame_stack->top().pc += 1;   
}

void iinc(stack<Frame>* frame_stack) {
    // TODO pensar como passar se o wide foi setado ou nao !!
    
    u1 index = frame_stack->top().get_method_code(frame_stack->top().pc + 1);

    Variable local_variable = frame_stack->top().local_variables[index];
    
    u4 inc = (int32_t) (int8_t) frame_stack->top().get_method_code(frame_stack->top().pc + 2);

    local_variable.data.v_int += inc;
    frame_stack->top().local_variables[index] = local_variable;

    frame_stack->top().pc += 3;   
}

void i2l(stack<Frame>* frame_stack) {
	Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_long = (int64_t) variable.data.v_int;
	variable.type = LONG;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void i2f(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_float = (float) variable.data.v_int;
	variable.type = FLOAT;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void i2d(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_double = (double) variable.data.v_int;
	variable.type = DOUBLE;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void l2i(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_int = (int32_t) variable.data.v_long;
	variable.type = INT;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void l2f(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_float = (float) variable.data.v_long;
	variable.type = FLOAT;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void l2d(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_double = (double) variable.data.v_long;
	variable.type = DOUBLE;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void f2i(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_int = (uint32_t) variable.data.v_float;
	variable.type = INT;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void f2l(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_long = (uint64_t) variable.data.v_float;
	variable.type = LONG;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void f2d(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_double = (double) variable.data.v_float;
	variable.type = DOUBLE;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void d2i(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_int = (uint32_t) variable.data.v_double;
	variable.type = INT;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void d2l(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_long = (uint64_t) variable.data.v_double;
	variable.type = LONG;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void d2f(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_float = (float) variable.data.v_double;
	variable.type = FLOAT;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void i2b(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_byte = (int8_t) variable.data.v_int;
	variable.type = BYTE;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void i2c(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_char = (char) variable.data.v_int;
	variable.type = CHAR;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void i2s(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_short = (int16_t) variable.data.v_int;
	variable.type = SHORT;

    frame_stack->top().operand_stack.push(variable);

	frame_stack->top().pc += 1;
}

void lcmp(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable result;
	result.type = VariableType::INT;

	if (variable_1.data.v_long > variable_2.data.v_long) {
		result.data.v_int = 1;
	} else if (variable_1.data.v_long < variable_2.data.v_long) {
		result.data.v_int = -1;
	} else {
		result.data.v_int = 0;
	}

	frame_stack->top().operand_stack.push(result);

	frame_stack->top().pc += 1;
}

void fcmpl(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable result;
	result.type = VariableType::INT;

	if (isnan(variable_1.data.v_float) || isnan(variable_2.data.v_float)) {
		result.data.v_int = -1;
	} else if (variable_1.data.v_float > variable_2.data.v_float) {
		result.data.v_int = 1;
	} else if (variable_1.data.v_float == variable_2.data.v_float) {
		result.data.v_int = 0;
	} else {
		result.data.v_int = -1;
	}

	frame_stack->top().operand_stack.push(result);

	frame_stack->top().pc += 1;
}

void fcmpg(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable result;
	result.type = VariableType::INT;

	if (isnan(variable_1.data.v_float) || isnan(variable_2.data.v_float)) {
		result.data.v_int = 1;
	} else if (variable_1.data.v_float > variable_2.data.v_float) {
		result.data.v_int = 1;
	} else if (variable_1.data.v_float == variable_2.data.v_float) {
		result.data.v_int = 0;
	} else {
		result.data.v_int = -1;
	}

	frame_stack->top().operand_stack.push(result);

	frame_stack->top().pc += 1;
}

void ifeq(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	
	if (variable.data.v_int == 0) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
        uint16_t branch_off_set = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_off_set;
    } else {
		frame_stack->top().pc += 3;
    }
}

void ifne(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	
	if (variable.data.v_int != 0) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
        uint16_t branch_off_set = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_off_set;
    } else {
		frame_stack->top().pc += 3;
    }
}

void iflt(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	
	if (variable.data.v_int < 0) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
        uint16_t branch_off_set = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_off_set;
    } else {
		frame_stack->top().pc += 3;
    }
}

void ifge(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	
	if (variable.data.v_int >= 0) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
        uint16_t branch_off_set = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_off_set;
    } else {
		frame_stack->top().pc += 3;
    }
}

void ifgt(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	
	if (variable.data.v_int > 0) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
        uint16_t branch_off_set = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_off_set;
    } else {
		frame_stack->top().pc += 3;
    }
}

void ifle(stack<Frame>* frame_stack) {
	Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	if (variable.data.v_int <= 0) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
        uint16_t branch_off_set = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_off_set;
    } else {
		frame_stack->top().pc += 3;
    }
}

void if_icmpeq(stack<Frame>* frame_stack) {
	Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	
	if (variable_1.data.v_int == variable_2.data.v_int) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
		int16_t branch_offset = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_offset;
    } else {
		frame_stack->top().pc += 3;
    }
}

void if_icmpne(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	
	if (variable_1.data.v_int != variable_2.data.v_int) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
		int16_t branch_offset = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_offset;
    } else {
		frame_stack->top().pc += 3;
    }
}

void if_icmplt(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	
	if (variable_1.data.v_int < variable_2.data.v_int) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
		int16_t branch_offset = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_offset;
    } else {
		frame_stack->top().pc += 3;
    }
}

void if_icmpge(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	if (variable_1.data.v_int >= variable_2.data.v_int) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
		int16_t branch_offset = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_offset;
    } else {
		frame_stack->top().pc += 3;
    }
}

void if_icmpgt(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	
	if (variable_1.data.v_int > variable_2.data.v_int) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
		int16_t branch_offset = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_offset;
    } else {
		frame_stack->top().pc += 3;
    }
}

void if_icmple(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	
	if (variable_1.data.v_int <= variable_2.data.v_int) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
		int16_t branch_offset = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_offset;
    } else {
		frame_stack->top().pc += 3;
    }
}

void goto_instruction(stack<Frame>* frame_stack) {
	u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
    u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
    int16_t branch_offset = (byte1 << 8) | byte2;
    frame_stack->top().pc += branch_offset;
}

void jsr(stack<Frame>* frame_stack) {
    u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
    u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
    int16_t branch_offset = (byte1 << 8) | byte2;
    
    Variable return_variable;
	return_variable.type = RETURN_ADDR;
	return_variable.data.return_address = frame_stack->top().pc + 3;
    frame_stack->top().operand_stack.push(return_variable);
	
    frame_stack->top().pc += branch_offset;
}

void ret(stack<Frame>* frame_stack) {
    u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
	uint16_t index = (uint16_t) byte1;

    // TODO pensar como passar se o wide foi setado ou nao !!

	Variable variable = frame_stack->top().local_variables[index];
	frame_stack->top().pc = variable.data.return_address;
}

void ireturn(stack<Frame>* frame_stack) {
    Variable ret_variable = frame_stack->top().operand_stack.top();
    frame_stack->pop();

    frame_stack->top().operand_stack.push(ret_variable);
}

void lreturn(stack<Frame>* frame_stack) {
    Variable ret_variable = frame_stack->top().operand_stack.top();
    frame_stack->pop();

    frame_stack->top().operand_stack.push(ret_variable);
}

void freturn(stack<Frame>* frame_stack) {
    Variable ret_variable = frame_stack->top().operand_stack.top();
    frame_stack->pop();

    frame_stack->top().operand_stack.push(ret_variable);
}

void dreturn(stack<Frame>* frame_stack) {
    Variable ret_variable = frame_stack->top().operand_stack.top();
    frame_stack->pop();

    frame_stack->top().operand_stack.push(ret_variable);
}

void areturn(stack<Frame>* frame_stack) {
    Variable ret_variable = frame_stack->top().operand_stack.top();
    frame_stack->pop();

    frame_stack->top().operand_stack.push(ret_variable);
}

void ifnull(stack<Frame>* frame_stack) {

    Variable reference_variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    if (reference_variable.data.object == NULL) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
        int16_t branch_offset = (byte1 << 8) | byte2;
        frame_stack->top().pc += branch_offset;
    } else {
        frame_stack->top().pc += 3;
    }
}

void ifnonnull(stack<Frame>* frame_stack) {
    
    Variable reference_variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    if (reference_variable.data.object != NULL) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
        int16_t branch_offset = (byte1 << 8) | byte2;
        frame_stack->top().pc += branch_offset;
    } else {
        frame_stack->top().pc += 3;
    }
}

void goto_w(stack<Frame>* frame_stack) {
    u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
    u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
    u1 byte3 = frame_stack->top().get_method_code(frame_stack->top().pc + 3);
    u1 byte4 = frame_stack->top().get_method_code(frame_stack->top().pc + 4);

	int16_t branch_offset = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

    frame_stack->top().pc += branch_offset;
}

void jsr_w(stack<Frame>* frame_stack) {
    u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
    u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
    u1 byte3 = frame_stack->top().get_method_code(frame_stack->top().pc + 3);
    u1 byte4 = frame_stack->top().get_method_code(frame_stack->top().pc + 4);

	int16_t branch_offset = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

	Variable return_addr;
	return_addr.type = RETURN_ADDR;
	return_addr.data.return_address = frame_stack->top().pc + 5;
    frame_stack->top().operand_stack.push(return_addr);

    frame_stack->top().pc += branch_offset;
}

void monitorenter(stack<Frame>* frame_stack) {
    frame_stack->top().pc += 1;
}

void monitorexit(stack<Frame>* frame_stack) {
    frame_stack->top().pc += 1;
}

void tableswitch(stack<Frame>* frame_stack) {
    // TODO
    cout << "tableswitch nao implementado" << endl;
    exit(1);
}

void lookupswitch(stack<Frame>* frame_stack) {
    // TODO
    cout << "lookupswitch nao implementado" << endl;
    exit(1);
}

void if_acmpeq(stack<Frame>* frame_stack) {
	Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	
	if (variable_1.data.object == variable_2.data.object) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
        uint16_t branch_off_set = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_off_set;
    } else {
		frame_stack->top().pc += 3;
    }
}

void if_acmpne(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
	
	if (variable_1.data.object != variable_2.data.object) {
        u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
        u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
        uint16_t branch_off_set = (byte1 << 8) | byte2;
		frame_stack->top().pc += branch_off_set;
    } else {
		frame_stack->top().pc += 3;
    }
}

void getstatic(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);
    uint16_t field_index = (byte1 << 8) | byte2;

    Constant_pool_variables cp_field = constant_pool[field_index];

    FieldRefInfo field_ref = cp_field.info.field_ref_info;

    string class_name = get_constant_pool_element(constant_pool, field_ref.class_index);

    Constant_pool_variables cp_name_and_type = constant_pool[field_ref.name_and_type_index];

    NameAndTypeInfo field_name_and_type = cp_name_and_type.info.name_and_type_info;

    string field_name = get_constant_pool_element(constant_pool, field_name_and_type.name_index);
    string field_descriptor = get_constant_pool_element(constant_pool, field_name_and_type.descriptor_index);

    if (class_name == "java/lang/System" && field_descriptor == "Ljava/io/PrintStream;" ) {
        frame_stack->top().pc += 3;
        return;
    }
    
    ClassRuntime *class_run_time = build_class(class_name);

    Variable static_var = class_run_time->static_fields[field_name];

    frame_stack->top().operand_stack.push(static_var);

    frame_stack->top().pc += 3;
}

void putstatic(stack<Frame>* frame_stack) {

    Frame curr_frame = frame_stack->top();

    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);
    uint16_t field_index = (byte1 << 8) | byte2;

    Constant_pool_variables cp_field = constant_pool[field_index];

    FieldRefInfo field_ref = cp_field.info.field_ref_info;

    string class_name = get_constant_pool_element(constant_pool, field_ref.class_index);

    Constant_pool_variables cp_name_and_type = constant_pool[field_ref.name_and_type_index];

    NameAndTypeInfo field_name_and_type = cp_name_and_type.info.name_and_type_info;

    string field_name = get_constant_pool_element(constant_pool, field_name_and_type.name_index);
    string field_descriptor = get_constant_pool_element(constant_pool, field_name_and_type.descriptor_index);

    ClassRuntime *class_run_time = build_class(class_name);

    Variable top_var = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    switch (field_descriptor[0]) {
        case 'B':
            top_var.type = BYTE;
            break;
        case 'C':
            top_var.type = CHAR;
            break;
        case 'S':
            top_var.type = SHORT;
            break;
        case 'Z':
            top_var.type = BOOLEAN;
            break;
    }

    class_run_time->static_fields[field_name] = top_var;

    frame_stack->top().pc += 3;
}

void getfield(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);
    uint16_t field_index = (byte1 << 8) | byte2;

    Constant_pool_variables cp_field = constant_pool[field_index];

    FieldRefInfo field_ref = cp_field.info.field_ref_info;

    string class_name = get_constant_pool_element(constant_pool, field_ref.class_index);

    Constant_pool_variables cp_name_and_type = constant_pool[field_ref.name_and_type_index];

    NameAndTypeInfo field_name_and_type = cp_name_and_type.info.name_and_type_info;

    string field_name = get_constant_pool_element(constant_pool, field_name_and_type.name_index);
    string field_descriptor = get_constant_pool_element(constant_pool, field_name_and_type.descriptor_index);

    Variable object_variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Object *object = object_variable.data.object;
    ClassObject *class_object = (ClassObject*) object;

    // cout << "NOME DA CLASSE ";
    // cout <<  get_constant_pool_element(object->constant_pool, object->this_class) << endl;
    // cout << "NOME DO FIELD ";
    // cout << field_name << endl;

    Variable field_var = class_object->fields[field_name];

    // cout << "CONTEUDO ";
    // cout << field_var.data.v_string << endl;

    frame_stack->top().operand_stack.push(field_var);

    frame_stack->top().pc += 3;
}

void putfield(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();

    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);
    uint16_t field_index = (byte1 << 8) | byte2;

    Constant_pool_variables cp_field = constant_pool[field_index];

    FieldRefInfo field_ref = cp_field.info.field_ref_info;

    string class_name = get_constant_pool_element(constant_pool, field_ref.class_index);

    Constant_pool_variables cp_name_and_type = constant_pool[field_ref.name_and_type_index];

    NameAndTypeInfo field_name_and_type = cp_name_and_type.info.name_and_type_info;

    string field_name = get_constant_pool_element(constant_pool, field_name_and_type.name_index);
    string field_descriptor = get_constant_pool_element(constant_pool, field_name_and_type.descriptor_index);

    Variable insert_variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    switch (field_descriptor[0]) {
        case 'B':
            insert_variable.type = BYTE;
            break;
        case 'C':
            insert_variable.type = CHAR;
            break;
        case 'S':
            insert_variable.type = SHORT;
            break;
        case 'Z':
            insert_variable.type = BOOLEAN;
            break;
    }

    Variable object_variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Object *object = object_variable.data.object;
    ClassObject *class_object = (ClassObject*) object;

    class_object->fields[field_name] = insert_variable;

    // cout << "inserido na classe : ";
    // cout <<  get_constant_pool_element(object->constant_pool, object->this_class) << endl;
    // cout << "dentro do field : ";
    // cout << field_name << endl;
    // cout << "a variavel : ";
    // if(insert_variable.type == 9 and insert_variable.ref_type == STRING) {
    //     cout << insert_variable.data.v_string << endl;
    // }
    // cout << endl;

    frame_stack->top().pc += 3;
}

void invokespecial(stack<Frame>* frame_stack) {

    Frame curr_frame = frame_stack->top();
    
    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);
    uint16_t method_index = (byte1 << 8) | byte2;

    Constant_pool_variables cp_method = constant_pool[method_index];

    MethodRefInfo method_info = cp_method.info.method_ref_info;

    string class_name = get_constant_pool_element(constant_pool, method_info.class_index);

    Constant_pool_variables cp_name_and_type = constant_pool[method_info.name_and_type_index];

    NameAndTypeInfo method_name_and_type = cp_name_and_type.info.name_and_type_info;

    string method_name = get_constant_pool_element(constant_pool, method_name_and_type.name_index);
    string method_descriptor = get_constant_pool_element(constant_pool, method_name_and_type.descriptor_index);

    // special cases
    if ((class_name == "java/lang/Object" || class_name == "java/lang/String" || class_name == "java/lang/StringBuffer") && method_name == "<init>") {
        if (class_name == "java/lang/String" || class_name == "java/lang/StringBuffer") {
            frame_stack->top().operand_stack.pop();
        }

        frame_stack->top().pc += 3;        
        return;
    }
    
    uint16_t args_count = 0; 
    uint16_t i = 1; // skip '('
    while (method_descriptor[i] != ')') {
        char base_type = method_descriptor[i];
        if (base_type == 'D' || base_type == 'J') {
            args_count += 2;
        } else if (base_type == 'L') {
            args_count++;
            while (method_descriptor[++i] != ';');
        } else if (base_type == '[') {
            args_count++;
            while (method_descriptor[++i] == '[');
            if (method_descriptor[i] == 'L') while (method_descriptor[++i] != ';');
        } else {
            args_count++;
        }
        i++;
    }

    vector<Variable> args;
    for (int i = 0; i < args_count; i++) {
        Variable variable = frame_stack->top().operand_stack.top();
        frame_stack->top().operand_stack.pop();
        args.push_back(variable);

        // simulate a padding
        if(variable.type == DOUBLE || variable.type == LONG) i++;
    }

    Variable object_variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    args.push_back(object_variable);
    reverse(args.begin(), args.end());

    ClassRuntime *class_run_time = build_class(class_name);

    frame_stack->top().pc += 3;

    frame_stack->push(Frame(class_run_time, method_name, method_descriptor, args));
}

void invokestatic(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();
    
    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);
    uint16_t method_index = (byte1 << 8) | byte2;

    Constant_pool_variables cp_method = constant_pool[method_index];

    MethodRefInfo method_info = cp_method.info.method_ref_info;

    string class_name = get_constant_pool_element(constant_pool, method_info.class_index);

    Constant_pool_variables cp_name_and_type = constant_pool[method_info.name_and_type_index];

    NameAndTypeInfo method_name_and_type = cp_name_and_type.info.name_and_type_info;

    string method_name = get_constant_pool_element(constant_pool, method_name_and_type.name_index);
    string method_descriptor = get_constant_pool_element(constant_pool, method_name_and_type.descriptor_index);

    if (class_name == "java/lang/Object" && method_name == "registerNatives") {
        frame_stack->top().pc += 3;
        return;
    }
    
    uint16_t args_count = 0; 
    uint16_t i = 1; // skip '('
    while (method_descriptor[i] != ')') {
        char base_type = method_descriptor[i];
        if (base_type == 'D' || base_type == 'J') {
            args_count += 2;
        } else if (base_type == 'L') {
            args_count++;
            while (method_descriptor[++i] != ';');
        } else if (base_type == '[') {
            args_count++;
            while (method_descriptor[++i] == '[');
            if (method_descriptor[i] == 'L') while (method_descriptor[++i] != ';');
        } else {
            args_count++;
        }
        i++;
    }

    vector<Variable> args;
    for (int i = 0; i < args_count; i++) {
        Variable variable = frame_stack->top().operand_stack.top();
        frame_stack->top().operand_stack.pop();
        args.push_back(variable);

        // simulate a padding
        if(variable.type == DOUBLE || variable.type == LONG) i++;
    }

    reverse(args.begin(), args.end());

    // update before put the new Frame
    frame_stack->top().pc += 3;

    frame_stack->push(Frame(curr_frame.class_run_time, method_name, method_descriptor, args));
}

void invokeinterface(stack<Frame>* frame_stack) {
    
    Frame curr_frame = frame_stack->top();
    
    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);
    uint16_t method_index = (byte1 << 8) | byte2;

    Constant_pool_variables cp_method = constant_pool[method_index];

    MethodRefInfo method_info = cp_method.info.method_ref_info;

    string class_name = get_constant_pool_element(constant_pool, method_info.class_index);

    Constant_pool_variables cp_name_and_type = constant_pool[method_info.name_and_type_index];

    NameAndTypeInfo method_name_and_type = cp_name_and_type.info.name_and_type_info;

    string method_name = get_constant_pool_element(constant_pool, method_name_and_type.name_index);
    string method_descriptor = get_constant_pool_element(constant_pool, method_name_and_type.descriptor_index);
    
    uint16_t args_count = 0; 
    uint16_t i = 1; // skip '('
    while (method_descriptor[i] != ')') {
        char base_type = method_descriptor[i];
        if (base_type == 'D' || base_type == 'J') {
            args_count += 2;
        } else if (base_type == 'L') {
            args_count++;
            while (method_descriptor[++i] != ';');
        } else if (base_type == '[') {
            args_count++;
            while (method_descriptor[++i] == '[');
            if (method_descriptor[i] == 'L') while (method_descriptor[++i] != ';');
        } else {
            args_count++;
        }
        i++;
    }

    vector<Variable> args;
    for (int i = 0; i < args_count; i++) {
        Variable variable = frame_stack->top().operand_stack.top();
        frame_stack->top().operand_stack.pop();
        args.push_back(variable);

        // simulate a padding
        if(variable.type == DOUBLE || variable.type == LONG) i++;
    }

    Variable v_object = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    args.push_back(v_object);
    reverse(args.begin(), args.end());

    Object *object = v_object.data.object;
    ClassObject *class_obj = (ClassObject *) object;

    build_class(class_name);
    
    // update before put the new Frame
    frame_stack->top().pc += 5;

    frame_stack->push(Frame(class_obj->class_run_time, method_name, method_descriptor, args));
}

void invokedynamic(stack<Frame>* frame_stack) {
    // TODO acho que é igual ao static
    cout << "invokedynamic nao implementado" << endl;
    exit(1);
}

void new_instruction(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();
    
    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);

    uint16_t class_index = (byte1 << 8) | byte2;
    Constant_pool_variables cp_class = constant_pool[class_index];
    
    ClassInfo class_info = cp_class.info.class_info;
    string class_name = get_constant_pool_element(constant_pool, class_info.name_index);

    Object* object;
    if (class_name == "java/lang/String" || class_name == "java/lang/StringBuffer") {
        object = new StringObject();
    } else {
        ClassRuntime *class_run_time = build_class(class_name);
        object = new ClassObject(class_run_time);
    }

    Variable object_ref;
    object_ref.data.object = object;
    object_ref.type = REFERENCE;
    frame_stack->top().operand_stack.push(object_ref);

    frame_stack->top().pc += 3;    
}

void newarray(stack<Frame>* frame_stack) {
    
    Variable array_size = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    ArrayObject *array;
    Variable variable; 
    
    u1 array_type = frame_stack->top().get_method_code(frame_stack->top().pc + 1);

    switch (array_type) {
        case 4:
            array = new ArrayObject(BOOLEAN);
            variable.type = BOOLEAN;
            for (int i = 0; i < array_size.data.v_int; i++) {
                variable.data.v_boolean = 0;
                array->elements.push_back(variable);
            }
            break;
        case 5:
            array = new ArrayObject(CHAR);
            variable.type = CHAR;
            for (int i = 0; i < array_size.data.v_int; i++) {
                variable.data.v_char = 0;
                array->elements.push_back(variable);
            }
            break;
        case 6:
            array = new ArrayObject(FLOAT);
            variable.type = FLOAT;
            for (int i = 0; i < array_size.data.v_int; i++) {
                variable.data.v_float = 0;
                array->elements.push_back(variable);
            }
            break;
        case 7:
            array = new ArrayObject(DOUBLE);
            variable.type = DOUBLE;
            for (int i = 0; i < array_size.data.v_int; i++) {
                variable.data.v_double = 0;
                array->elements.push_back(variable);
            }
            break;
        case 8:
            array = new ArrayObject(BYTE);
            variable.type = BYTE;
            for (int i = 0; i < array_size.data.v_int; i++) {
                variable.data.v_byte = 0;
                array->elements.push_back(variable);
            }
            break;
        case 9:
            array = new ArrayObject(SHORT);
            variable.type = SHORT;
            for (int i = 0; i < array_size.data.v_int; i++) {
                variable.data.v_short = 0;
                array->elements.push_back(variable);
            }
            break;
        case 10:
            array = new ArrayObject(INT);
            variable.type = INT;
            for (int i = 0; i < array_size.data.v_int; i++) {
                variable.data.v_int = 0;
                array->elements.push_back(variable);
            }
            break;
        case 11:
            array = new ArrayObject(LONG);
            variable.type = LONG;
            for (int i = 0; i < array_size.data.v_int; i++) {
                variable.data.v_long = 0;
                array->elements.push_back(variable);
            }
            break;
    }
    
    Variable array_ref;
    array_ref.type = REFERENCE;
    array_ref.data.object = array;
    
    frame_stack->top().operand_stack.push(array_ref);

    frame_stack->top().pc += 2;
}

void anewarray(stack<Frame>* frame_stack) {
    Frame curr_frame = frame_stack->top();
    
    Variable count = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    vector<Constant_pool_variables> constant_pool = curr_frame.class_run_time->class_file->constant_pool;

    u1 byte1 = curr_frame.get_method_code(curr_frame.pc + 1);
    u1 byte2 = curr_frame.get_method_code(curr_frame.pc + 2);

    uint16_t class_index = (byte1 << 8) | byte2;

    Constant_pool_variables cp_class = constant_pool[class_index];
    
    ClassInfo class_info = cp_class.info.class_info;
    string class_name = get_constant_pool_element(constant_pool, class_info.name_index);

    Variable array_ref;
    array_ref.type = REFERENCE;
    array_ref.data.object = new ArrayObject(REFERENCE);
    
    Variable null_value;
    null_value.type = REFERENCE;
    null_value.data.object = NULL;
    for (int i = 0; i < count.data.v_int; i++) {
        ((ArrayObject *) array_ref.data.object)->elements.push_back(null_value);
    }

    frame_stack->top().operand_stack.push(array_ref);

    frame_stack->top().pc += 3;
}

void arraylength(stack<Frame>* frame_stack) {
    
    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    Variable length;
    length.type = INT;
    length.data.v_int = ((ArrayObject *) array_ref.data.object)->elements.size();
    
    frame_stack->top().operand_stack.push(length);
    frame_stack->top().pc += 1;
}

void athrow(stack<Frame>* frame_stack) {
    frame_stack->top().pc += 1;
}

void checkcast(stack<Frame>* frame_stack) {
    // TODO
    cout << "checkcast nao implementado" << endl;
    exit(1);
}

void instanceof(stack<Frame>* frame_stack) {
    // TODO
    cout << "instanceof nao implementado" << endl;
    exit(1);
}

void wide(stack<Frame>* frame_stack) {
    // TODO
    cout << "wide nao implementado" << endl;
    exit(1);
}

void multianewarray(stack<Frame>* frame_stack) {
    vector<Constant_pool_variables> constant_pool = frame_stack->top().class_run_time->class_file->constant_pool;

    u1 byte1 = frame_stack->top().get_method_code(frame_stack->top().pc + 1);
    u1 byte2 = frame_stack->top().get_method_code(frame_stack->top().pc + 2);
    u1 dimensions = frame_stack->top().get_method_code(frame_stack->top().pc + 3);

    uint16_t class_index = (byte1 << 8) | byte2;
    
    Constant_pool_variables cp_element = constant_pool[class_index];
    
    ClassInfo class_info = cp_element.info.class_info;
    string class_name = get_constant_pool_element(constant_pool, class_info.name_index);
    
    VariableType variable_type;
    int i = 0;
    while (class_name[i] == '[') i++;
    
    string multi_array_type = class_name.substr(i+1, class_name.size()-i-2); 
    
    switch (class_name[i]) {
        case 'L':
            if (multi_array_type != "java/lang/String") {
                cout << "falta implementar isso dentro do mult array" << endl;
                exit(1);
                // chamar o build class so para criar a referencia
            }
            variable_type = REFERENCE;
            break;
        case 'B':
            variable_type = BYTE;
            break;
        case 'C':
            variable_type = CHAR;
            break;
        case 'D':
            variable_type = DOUBLE;
            break;
        case 'F':
            variable_type = FLOAT;
            break;
        case 'I':
            variable_type = INT;
            break;
        case 'J':
            variable_type = LONG;
            break;
        case 'S':
            variable_type = SHORT;
            break;
        case 'Z':
            variable_type = BOOLEAN;
            break;
    }
    
    stack<int> count;
    for (int i = 0; i < dimensions; i++) {
        Variable dim_length = frame_stack->top().operand_stack.top();
        frame_stack->top().operand_stack.pop();
        count.push(dim_length.data.v_int);
    }
    
    ArrayObject *array = new ArrayObject((dimensions > 1) ? REFERENCE : variable_type);
    fill_multi_array(array, variable_type, count);
    
    Variable array_variable;
    array_variable.type = REFERENCE;
    array_variable.data.object = array;
    
    frame_stack->top().operand_stack.push(array_variable);

    frame_stack->top().pc += 4; 
}

void iaload(stack<Frame>* frame_stack) {
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    frame_stack->top().operand_stack.push(array->elements[index.data.v_int]);

    frame_stack->top().pc += 1;
}

void laload(stack<Frame>* frame_stack) {
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    frame_stack->top().operand_stack.push(array->elements[index.data.v_int]);

    frame_stack->top().pc += 1;
}

void faload(stack<Frame>* frame_stack) {
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    frame_stack->top().operand_stack.push(array->elements[index.data.v_int]);

    frame_stack->top().pc += 1;
}

void daload(stack<Frame>* frame_stack) {
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    frame_stack->top().operand_stack.push(array->elements[index.data.v_int]);

    frame_stack->top().pc += 1;
}

void aaload(stack<Frame>* frame_stack) {
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    frame_stack->top().operand_stack.push(array->elements[index.data.v_int]);

    frame_stack->top().pc += 1;
}

void baload(stack<Frame>* frame_stack) {
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    frame_stack->top().operand_stack.push(array->elements[index.data.v_int]);

    frame_stack->top().pc += 1;
}

void caload(stack<Frame>* frame_stack) {
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    frame_stack->top().operand_stack.push(array->elements[index.data.v_int]);

    frame_stack->top().pc += 1;
}

void saload(stack<Frame>* frame_stack) {
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    frame_stack->top().operand_stack.push(array->elements[index.data.v_int]);

    frame_stack->top().pc += 1;
}

void iastore(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    array->elements[index.data.v_int] = variable;

    frame_stack->top().pc += 1;   
}

void lastore(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    array->elements[index.data.v_int] = variable;

    frame_stack->top().pc += 1;
}

void fastore(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    array->elements[index.data.v_int] = variable;

    frame_stack->top().pc += 1; 
}

void dastore(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    array->elements[index.data.v_int] = variable;

    frame_stack->top().pc += 1; 
}

void aastore(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    array->elements[index.data.v_int] = variable;

    frame_stack->top().pc += 1;  
}

void bastore(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    if (array->type == BOOLEAN) {
        variable.data.v_boolean = (variable.data.v_int != 0) ? true : false;
        variable.type = BOOLEAN;
    } else {
        variable.data.v_byte = (uint8_t) variable.data.v_int;
        variable.type = BYTE;
    }

    array->elements[index.data.v_int] = variable;

    frame_stack->top().pc += 1;  
}

void castore(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_char = (uint8_t) variable.data.v_int;
    variable.type = CHAR;
    
    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    array->elements[index.data.v_int] = variable;

    frame_stack->top().pc += 1; 
}

void sastore(stack<Frame>* frame_stack) {
    Variable variable = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    
    Variable index = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable array_ref = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    variable.data.v_short = (int16_t) variable.data.v_int;
    variable.type = SHORT;
    
    ArrayObject* array = (ArrayObject *) array_ref.data.object;

    array->elements[index.data.v_int] = variable;

    frame_stack->top().pc += 1;   
}

void dup2(stack<Frame>* frame_stack) {
    // cout << "POSSIVELMENTE ERRADA !!!" << endl;
    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();
        
    frame_stack->top().operand_stack.push(variable_2);
    frame_stack->top().operand_stack.push(variable_1);
    frame_stack->top().operand_stack.push(variable_2);
    frame_stack->top().operand_stack.push(variable_1);
    
    frame_stack->top().pc += 1;   
}

void dup2_x1(stack<Frame>* frame_stack) {
    cout << "instrucao dup2_x1 nao implementada" << endl;
    exit(1);
}

void dup2_x2(stack<Frame>* frame_stack) {
    cout << "instrucao dup2_x2 nao implementada" << endl;
    exit(1);
}

void dcmpl(stack<Frame>* frame_stack) {

    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable result;
	result.type = VariableType::INT;

	if (isnan(variable_1.data.v_double) || isnan(variable_2.data.v_double)) {
		result.data.v_int = -1;
	} else if (variable_1.data.v_double > variable_2.data.v_double) {
		result.data.v_int = 1;
	} else if (variable_1.data.v_double == variable_2.data.v_double) {
		result.data.v_int = 0;
	} else {
		result.data.v_int = -1;
	}

	frame_stack->top().operand_stack.push(result);

	frame_stack->top().pc += 1;
}

void dcmpg(stack<Frame>* frame_stack) {
    Variable variable_2 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

    Variable variable_1 = frame_stack->top().operand_stack.top();
    frame_stack->top().operand_stack.pop();

	Variable result;
	result.type = VariableType::INT;

	if (isnan(variable_1.data.v_double) || isnan(variable_2.data.v_double)) {
		result.data.v_int = 1;
	} else if (variable_1.data.v_double > variable_2.data.v_double) {
		result.data.v_int = 1;
	} else if (variable_1.data.v_double == variable_2.data.v_double) {
		result.data.v_int = 0;
	} else {
		result.data.v_int = -1;
	}

	frame_stack->top().operand_stack.push(result);

	frame_stack->top().pc += 1;
}