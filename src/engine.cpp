#include "engine.hpp"

void Engine::start(ClassRuntime* class_run_time) {
    stack<Frame> frame_stack;

    std::vector<Variable> arguments;

    frame_stack.push(Frame(class_run_time, "main", "([Ljava/lang/String;)V", arguments));

    while (frame_stack.size() > 0) {
        Frame curr_frame = frame_stack.top();
        // cout << get_constant_pool_element(curr_frame.class_run_time->class_file->constant_pool, curr_frame.class_run_time->class_file->this_class) << endl;
        u1 instruction_code = curr_frame.get_method_code(curr_frame.pc);

        // use printf instead of endl to not shuffle out put
        // printf("%x\n", int(instruction_code));

        // std::cout << "pc -> " << curr_frame.pc << " code -> " << std::hex << int(instruction_code) << "\n";
        // string current_method_name = get_constant_pool_element(curr_frame.class_run_time.class_file.constant_pool, curr_frame.method.name_index);
        // cout << "met -> " << current_method_name << endl;

        // std::cout << "tamanho -> " << std::dec << curr_frame.operand_stack.size() << std::endl;
        execute_instruction(&frame_stack, instruction_code);

        // {
        //     stack<Variable> copia = frame_stack.top().operand_stack;
        //     vector <string> todas;
        //     while(copia.size() > 0) {
        //         auto x = copia.top();
        //         copia.pop();
        //         if(x.type == 9 and x.ref_type == STRING) {
        //             todas.push_back(string(x.data.v_string));
        //         }
        //     }
        //     if(todas.size() > 0) {
        //         cout << "strings na pilha : " << endl;

        //         for(auto x: todas) {
        //             cout << x << endl;
        //         }

        //         cout << "fim das strings na pilha : " << endl;

        //     }

        // }
    }
}

void execute_instruction(stack<Frame>* frame_stack, u1 instruction_code) {
    switch (instruction_code) {
        case 0x00: 
            nop(frame_stack);
            break;
        case 0x01: 
            aconst_null(frame_stack);
            break;
        case 0x02: 
            iconst_m1(frame_stack);
            break;
        case 0x03: 
            iconst_0(frame_stack);
            break;
        case 0x04: 
            iconst_1(frame_stack);
            break;
        case 0x05: 
            iconst_2(frame_stack);
            break;
        case 0x06: 
            iconst_3(frame_stack);
            break;
        case 0x07: 
            iconst_4(frame_stack);
            break;
        case 0x08: 
            iconst_5(frame_stack);
            break;
        case 0x09: 
            lconst_0(frame_stack);
            break;
        case 0x0a: 
            lconst_1(frame_stack);
            break;
        case 0x0b: 
            fconst_0(frame_stack);
            break;
        case 0x0c: 
            fconst_1(frame_stack);
            break;
        case 0x0d: 
            fconst_2(frame_stack);
            break;
        case 0x0e: 
            dconst_0(frame_stack);
            break;
        case 0x0f: 
            dconst_1(frame_stack);
            break;
        case 0x10: 
            bipush(frame_stack);
            break;
        case 0x11: 
            sipush(frame_stack);
            break;
        case 0x12: 
            ldc(frame_stack);
            break;
        case 0x13: 
            ldc_w(frame_stack);
            break;
        case 0x14:
            ldc2_w(frame_stack);
            break;
        case 0x15: 
            iload(frame_stack);
            break;
        case 0x16: 
            lload(frame_stack);
            break;
        case 0x17: 
            fload(frame_stack);
            break;
        case 0x18:
            dload(frame_stack);
            break;
        case 0x19: 
            aload(frame_stack);
            break;
        case 0x1a: 
            iload_0(frame_stack);
            break;
        case 0x1b: 
            iload_1(frame_stack);
            break;
        case 0x1c: 
            iload_2(frame_stack);
            break;
        case 0x1d: 
            iload_3(frame_stack);
            break;
        case 0x1e: 
            lload_0(frame_stack);
            break;
        case 0x1f: 
            lload_1(frame_stack);
            break;
        case 0x20: 
            lload_2(frame_stack);
            break;
        case 0x21: 
            lload_3(frame_stack);
            break;
        case 0x22: 
            fload_0(frame_stack);
            break;
        case 0x23: 
            fload_1(frame_stack);
            break;
        case 0x24: 
            fload_2(frame_stack);
            break;
        case 0x25: 
            fload_3(frame_stack);
            break;
        case 0x26: 
            dload_0(frame_stack);
            break;
        case 0x27:
            dload_1(frame_stack);
            break;
        case 0x28: 
            dload_2(frame_stack);
            break;
        case 0x29:
            dload_3(frame_stack);
            break;
        case 0x2a:
            aload_0(frame_stack);
            break;
        case 0x2b:
            aload_1(frame_stack);
            break;
        case 0x2c:
            aload_2(frame_stack);
            break;
        case 0x2d:
            aload_3(frame_stack);
            break;
        case 0x2e:
            iaload(frame_stack);
            break;
        case 0x2f:
            laload(frame_stack);
            break;
        case 0x30:
            faload(frame_stack);
            break;
        case 0x31:
            daload(frame_stack);
            break;
        case 0x32:
            aaload(frame_stack);
            break;
        case 0x33:
            baload(frame_stack);
            break;
        case 0x34:
            caload(frame_stack);
            break;
        case 0x35:
            saload(frame_stack);
            break;
        case 0x36:
            istore(frame_stack);
            break;
        case 0x37:
            lstore(frame_stack);
            break;
        case 0x38:
            fstore(frame_stack);
            break;
        case 0x39:
            dstore(frame_stack);
            break;
        case 0x3a:
            astore(frame_stack);
            break;
        case 0x3b:
            istore_0(frame_stack);
            break;
        case 0x3c:
            istore_1(frame_stack);
            break;
        case 0x3d:
            istore_2(frame_stack);
            break;
        case 0x3e:
            istore_3(frame_stack);
            break;
        case 0x3f:
            lstore_0(frame_stack);
            break;
        case 0x40:
            lstore_1(frame_stack);
            break;
        case 0x41:
            lstore_2(frame_stack);
            break;
        case 0x42:
            lstore_3(frame_stack);
            break;
        case 0x43:
            fstore_0(frame_stack);
            break;
        case 0x44:
            fstore_1(frame_stack);
            break;
        case 0x45:
            fstore_2(frame_stack);
            break;
        case 0x46:
            fstore_3(frame_stack);
            break;
        case 0x47:
            dstore_0(frame_stack);
            break;
        case 0x48:
            dstore_1(frame_stack);
            break;
        case 0x49:
            dstore_2(frame_stack);
            break;
        case 0x4a:
            dstore_3(frame_stack);
            break;
        case 0x4b:
            astore_0(frame_stack);
            break;
        case 0x4c:
            astore_1(frame_stack);
            break;
        case 0x4d:
            astore_2(frame_stack);
            break;
        case 0x4e:
            astore_3(frame_stack);
            break;
        case 0x4f:
            iastore(frame_stack);
            break;
        case 0x50:
            lastore(frame_stack);
            break;
        case 0x51:
            fastore(frame_stack);
            break;
        case 0x52:
            dastore(frame_stack);
            break;
        case 0x53:
            aastore(frame_stack);
            break;
        case 0x54:
            bastore(frame_stack);
            break;
        case 0x55:
            castore(frame_stack);
            break;
        case 0x56:
            sastore(frame_stack);
            break;
        case 0x57:
            pop(frame_stack);
            break;
        case 0x58:
            pop2(frame_stack);
            break;
        case 0x59:
            dup(frame_stack);
            break;
        case 0x5a:
            dup_x1(frame_stack);
            break;
        case 0x5b:
            dup_x2(frame_stack);
            break;
        case 0x5c:
            dup2(frame_stack);
            break;
        case 0x5d:
            dup2_x1(frame_stack);
            break;
        case 0x5e:
            dup2_x2(frame_stack);
            break;
        case 0x5f:
            swap(frame_stack);
            break;
        case 0x60:
            iadd(frame_stack);
            break;
        case 0x61:
            ladd(frame_stack);
            break;
        case 0x62:
            fadd(frame_stack);
            break;
        case 0x63:
            dadd(frame_stack);
            break;
        case 0x64:
            isub(frame_stack);
            break;
        case 0x65:
            lsub(frame_stack);
            break;
        case 0x66:
            fsub(frame_stack);
            break;
        case 0x67:
            dsub(frame_stack);
            break;
        case 0x68:
            imul(frame_stack);
            break;
        case 0x69:
            lmul(frame_stack);
            break;
        case 0x6a:
            fmul(frame_stack);
            break;
        case 0x6b:
            dmul(frame_stack);
            break;
        case 0x6c:
            idiv(frame_stack);
            break;
        case 0x6d:
            ldiv_instruction(frame_stack);
            break;
        case 0x6e:
            fdiv(frame_stack);
            break;
        case 0x6f:
            ddiv(frame_stack);
            break;
        case 0x70:
            irem(frame_stack);
            break;
        case 0x71:
            lrem(frame_stack);
            break;
        case 0x72:
            frem(frame_stack);
            break;
        case 0x73:
            drem(frame_stack); 
            break;
        case 0x74:
            ineg(frame_stack);
            break;
        case 0x75:
            lneg(frame_stack);
            break;
        case 0x76:
            fneg(frame_stack);
            break;
        case 0x77:
            dneg(frame_stack);
            break;
        case 0x78:
            ishl(frame_stack);
            break;
        case 0x79:
            lshl(frame_stack);
            break;
        case 0x7a:
            ishr(frame_stack);
            break;
        case 0x7b:
            lshr(frame_stack);
            break;
        case 0x7c:
            iushr(frame_stack);
            break;
        case 0x7d:
            lushr(frame_stack);
            break;
        case 0x7e:
            iand(frame_stack);
            break;
        case 0x7f:
            land(frame_stack);
            break;
        case 0x80:
            ior(frame_stack);
            break;
        case 0x81:
            lor(frame_stack);
            break;
        case 0x82:
            ixor(frame_stack);
            break;
        case 0x83:
            lxor(frame_stack);
            break;
        case 0x84:
            iinc(frame_stack);
            break;
        case 0x85:
            i2l(frame_stack);
            break;
        case 0x86:
            i2f(frame_stack);
            break;
        case 0x87:
            i2d(frame_stack);
            break;
        case 0x88:
            l2i(frame_stack);
            break;
        case 0x89:
            l2f(frame_stack);
            break;
        case 0x8a:
            l2d(frame_stack);
            break;
        case 0x8b:
            f2i(frame_stack);
            break;
        case 0x8c:
            f2l(frame_stack);
            break;
        case 0x8d:
            f2d(frame_stack);
            break;
        case 0x8e:
            d2i(frame_stack);
            break;
        case 0x8f:
            d2l(frame_stack);
            break;
        case 0x90:
            d2f(frame_stack);
            break;
        case 0x91:
            i2b(frame_stack);
            break;
        case 0x92:
            i2c(frame_stack);
            break;
        case 0x93:
            i2s(frame_stack);
            break;
        case 0x94:
            lcmp(frame_stack);
            break;
        case 0x95:
            fcmpl(frame_stack);
            break;
        case 0x96:
            fcmpg(frame_stack);
            break;
        case 0x97:
            dcmpl(frame_stack);
            break;
        case 0x98:
            dcmpg(frame_stack);
            break;
        case 0x99:
            ifeq(frame_stack);
            break;
        case 0x9a:
            ifne(frame_stack);
            break;
        case 0x9b:
            iflt(frame_stack);
            break;
        case 0x9c:
            ifge(frame_stack);
            break;
        case 0x9d:
            ifgt(frame_stack);
            break;
        case 0x9e:
            ifle(frame_stack);
            break;
        case 0x9f:
            if_icmpeq(frame_stack);
            break;
        case 0xa0:
            if_icmpne(frame_stack);
            break;
        case 0xa1:
            if_icmplt(frame_stack);
            break;
        case 0xa2:
            if_icmpge(frame_stack);
            break;
        case 0xa3:
            if_icmpgt(frame_stack);
            break;
        case 0xa4:
            if_icmple(frame_stack);
            break;
        case 0xa5:
            if_acmpeq(frame_stack);
            break;
        case 0xa6:
            if_acmpne(frame_stack);
            break;
        case 0xa7:
            goto_instruction(frame_stack);
            break;
        case 0xa8:
            jsr(frame_stack);
            break;
        case 0xa9:
            ret(frame_stack);
            break;
        case 0xaa:
            tableswitch(frame_stack);
            break;
        case 0xab:
            lookupswitch(frame_stack);
            break;
        case 0xac:
            ireturn(frame_stack);
            break;
        case 0xad:
            lreturn(frame_stack);
            break;
        case 0xae:
            freturn(frame_stack);
            break;
        case 0xaf:
            dreturn(frame_stack);
            break;
        case 0xb0:
            areturn(frame_stack);
            break;
        case 0xb1:
            return_instruction(frame_stack);
            break;
        case 0xb2:
            getstatic(frame_stack);
            break;
        case 0xb3:
            putstatic(frame_stack);
            break;
        case 0xb4:
            getfield(frame_stack);
            break;
        case 0xb5:
            putfield(frame_stack);
            break;
        case 0xb6:
            invokevirtual(frame_stack);
            break;
        case 0xb7:
            invokespecial(frame_stack);
            break;
        case 0xb8:
            invokestatic(frame_stack);
            break;
        case 0xb9:
            invokeinterface(frame_stack);
            break;
        case 0xba:
            invokedynamic(frame_stack);
            break;
        case 0xbb:
            new_instruction(frame_stack);
            break;
        case 0xbc:
            newarray(frame_stack);
            break;
        case 0xbd:
            anewarray(frame_stack);
            break;
        case 0xbe:
            arraylength(frame_stack);
            break;
        case 0xbf:
            athrow(frame_stack);
            break;
        case 0xc0:
            checkcast(frame_stack);
            break;
        case 0xc1:
            instanceof(frame_stack);
            break;
        case 0xc2:
            monitorenter(frame_stack);
            break;
        case 0xc3:
            monitorexit(frame_stack);
            break;
        case 0xc4:
            wide(frame_stack);
            break;
        case 0xc5:
            multianewarray(frame_stack);
            break;
        case 0xc6:
            ifnull(frame_stack);
            break;
        case 0xc7:
            ifnonnull(frame_stack);
            break;
        case 0xc8:
            goto_w(frame_stack);
            break;
        case 0xc9:
            jsr_w(frame_stack);
            break;
    }
}