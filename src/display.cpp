#include "display.hpp"

void display::class_file(ClassFile class_file) {
    printf("\n");

    printf("Informações gerais:\n");
    display::general_information(class_file);

    printf("\nConstant Pool:\n");
    display::constant_pool(class_file);

    printf("\nInterfaces:\n");
    display::interfaces(class_file);

    printf("\nFields:\n");
    display::fields(class_file, 1);

    printf("\nMethods:\n");
    display::methods(class_file, 1);

    printf("\nAttributes:\n");
    display::attributes(class_file, 1);
}

void display::general_information(ClassFile class_file) {
    display::indentation(1);
    printf("%-23s %d\n", "Minor Version:", class_file.min_version);

    display::indentation(1);
    printf("%-23s %d\n", "Major Version:", class_file.major_version);
    
    display::indentation(1);
    printf("%-23s %s\n", "Java Version:", display::java_versions[class_file.major_version].c_str());

    display::indentation(1);
    printf("%-23s %d\n", "Constant pool count:", class_file.constant_pool_length);

    display::indentation(1);
    printf("%-23s (0x%.4X)\n", "Access Flags:", class_file.access_flags);
    display::indentation(2);
    display::access_flags(class_file.access_flags);
    printf("\n");
    
    string class_name = get_constant_pool_element(class_file.constant_pool, class_file.this_class);
    display::indentation(1);
    printf("%-23s %-30s cp_index #%d\n", "This class:", class_name.c_str(), class_file.this_class);

    display::indentation(1);
    if(class_file.super_class == 0) {
        printf("%-23s %-30s\n", "Super class:", "none");
    } else {
        string super_class_name = get_constant_pool_element(class_file.constant_pool, class_file.super_class);
        printf("%-23s %-30s cp_index #%d\n", "Super class: ", super_class_name.c_str(), class_file.super_class);
    }

    display::indentation(1);
    printf("%-23s %d\n", "Interfaces count:", class_file.interfaces_count);

    display::indentation(1);
    printf("%-23s %d\n", "Fields count:", class_file.fields_count);

    display::indentation(1);
    printf("%-23s %d\n", "Methods pool count:", class_file.methods_count);

    display::indentation(1);
    printf("%-23s %d\n", "Attributes pool count:", class_file.attributes_count);
}

void display::access_flags(u2 accessFlags) {
    string flags[] = {"ACC_PUBLIC ", "ACC_FINAL ", "ACC_SUPER ", "ACC_INTERFACE ", "ACC_ABSTRACT ", "ACC_PRIVATE ", "ACC_PROTECTED ", "ACC_STATIC ", "ACC_VOLATILE ", "ACC_TRANSIENT "};
    u2 masks[] = {0x0001, 0x0010, 0x0020, 0x0200, 0x0400, 0x0002, 0x0004, 0x0008, 0x0040, 0x0080};

    for (uint8_t i = 0; i < 10; i++) {
        if ((accessFlags & masks[i]) != 0) {
            printf("%s ", flags[i].c_str());
        }
    }
    printf("\n");
}

void display::byte_code(CodeAttribute code_attribute, vector<Constant_pool_variables> constant_pool, int indentation) {
    u4 code_length = code_attribute.code_length;
    vector <u1> code = code_attribute.code;
    
    for(u4 i = 0; i < code_length;) {
        display::indentation(indentation);
        printf("\t%d\t%s ", i, display::instructions[code[i]].c_str());
        
        if (code[i] <= 0x0f || (code[i] >= 0x1a && code[i] <= 0x35) || (code[i] >= 0x3b && code[i] <= 0x83) || (code[i] >= 0x85 && code[i] <= 0x98) ||
            (code[i] >= 0xac && code[i] <= 0xb1) || code[i] == 0xbe || code[i] == 0xbf || code[i] == 0xc2 || code[i] == 0xc3) {
            printf("\n");
            i++;
        } else if (code[i] == 0x12) { 
            printf(" #%d %s\n",code[i+1],get_constant_pool_element(constant_pool, code[i+1]).c_str());
            i += 2;
        } else if (code[i] == 0xbc) { // newarray
            u1 atype = code[i+1];
            const string types[] = {"boolean", "char", "float", "double", "byte", "short", "int", "long"};
            printf(" #%d (%s)\n",atype, types[atype-4].c_str());
            i += 2;
        } else if (code[i] == 0x10 || (code[i] >= 0x15 && code[i] <= 0x19) || (code[i] >= 0x36 && code[i] <= 0x3a) || code[i] == 0xa9) {
            printf("%d\n",code[i+1]);
            i += 2;
        } else if (code[i] == 0x11) { // sipush
            int16_t number = (code[i+1] << 8) | code[i+2];
            printf(" %d\n", number);
            i += 3;
        } else if (code[i] == 0x84) { // iinc
            printf(" %d by %d\n", code[i+1], code[i+2]);
            i += 3;
        } else if (code[i] == 0x13 || code[i] == 0x14 || (code[i] >= 0xb2 && code[i] <= 0xb8) ||
                   code[i] == 0xbb || code[i] == 0xbd || code[i] == 0xc0 || code[i] == 0xc1) { // usa CP
            u2 number = (code[i+1] << 8) | code[i+2];
            printf(" #%d %s\n", number, get_constant_pool_element(constant_pool, number).c_str());
            i += 3;
        } else if ((code[i] >= 0x99 && code[i] <= 0xa8) || code[i] == 0xc6 || code[i] == 0xc7) {
            int16_t number = (code[i+1] << 8) | code[i+2];
            printf(" %d (%+d)\n", i+number, number);
            i += 3;
        } else if (code[i] == 0xc5) { // multianewarray
            u2 number = (code[i+1] << 8) | code[i+2];
            printf(" #%d %s dim %d\n", number, get_constant_pool_element(constant_pool, number).c_str(), code[i+3]);
            i += 4;
        } else if (code[i] == 0xb9) { // invokeinterface
            u2 number = (code[i+1] << 8) | code[i+2];
            printf(" #%d %s count %d\n", number, get_constant_pool_element(constant_pool, number).c_str(), code[i+3]);
            i += 5;
        } else if (code[i] == 0xc8 || code[i] == 0xc9) { // goto_w e jsr_w
            int32_t number = (code[i+1] << 24) | (code[i+2] << 16) | (code[i+3] << 8) | code[i+4];
            printf(" %d (%+d)\n", i+number, number);
            i += 5;
        } else if (code[i] == 0xc4) { // wide
            u2 indexbyte = (code[i+2] << 8) | code[i+3];
            
            printf("\n");
            display::indentation(indentation);
            printf("%d\t%s %d", i+1, instructions[code[i+1]].c_str(), indexbyte);
            
            if (code[i+1] == 0x84) { // format 2 (iinc)
                int16_t constbyte = (code[i+4] << 8) | code[i+5];
                printf(" by %d", constbyte);
                i += 6;
            } else {
                i += 4;
            }
            
            printf("\n");
        } else if (code[i] == 0xaa) { // tableswitch
            u1 padding = (i+1) % 4;
            int32_t defaultbytes = (code[padding+i+1] << 24) | (code[padding+i+2] << 16) | (code[padding+i+3] << 8) | code[padding+i+4];
            int32_t lowbytes = (code[padding+i+5] << 24) | (code[padding+i+6] << 16) | (code[padding+i+7] << 8) | code[padding+i+8];
            int32_t highbytes = (code[padding+i+9] << 24) | (code[padding+i+10] << 16) | (code[padding+i+11] << 8) | code[padding+i+12];
            
            printf(" %d to %d\n", lowbytes, highbytes);
            
            u4 howManyBytes = 1 + padding + 12; // 1 (instruction) + padding + 12 (the 12 bytes above)
            int32_t offsets = highbytes - lowbytes + 1;
            
            for (u4 n = 0; n < offsets; n++) {
                int32_t offset = (code[i+howManyBytes] << 24) | (code[i+howManyBytes+1] << 16) | (code[i+howManyBytes+2] << 8) | code[i+howManyBytes+3];
                display::indentation(indentation);
                printf("\t\t\t%d: %d (%+d)\n", lowbytes, i + offset, offset);
                
                lowbytes++;
                howManyBytes += 4;
            }

            display::indentation(indentation);
            printf("\t\t\tdefault: %d (%+d)\n", i + defaultbytes, defaultbytes);
            
            i += howManyBytes;
        } else if (code[i] == 0xab) { // lookupswitch
            u1 padding = (i+1) % 4;
            int32_t defaultbytes = (code[padding+i+1] << 24) | (code[padding+i+2] << 16) | (code[padding+i+3] << 8) | code[padding+i+4];
            int32_t npairs = (code[padding+i+5] << 24) | (code[padding+i+6] << 16) | (code[padding+i+7] << 8) | code[padding+i+8];
            
            u4 howManyBytes = 1 + padding + 8; // 1 (instruction) + padding + 8 (the 8 bytes above)
            
            printf(" %d\n", npairs);
            
            for (u4 n = 0; n < npairs; n++) {
                int32_t match = (code[i+howManyBytes] << 24) | (code[i+howManyBytes+1] << 16) | (code[i+howManyBytes+2] << 8) | code[i+howManyBytes+3];
                int32_t offset = (code[i+howManyBytes+4] << 24) | (code[i+howManyBytes+5] << 16) | (code[i+howManyBytes+6] << 8) | code[i+howManyBytes+7];
                display::indentation(indentation);
                printf("\t\t\t%d: %d (%+d)\n",match, i + offset, offset);
                howManyBytes += 8;
            }
            display::indentation(indentation);
            printf("\t\t\tdefault: %d (%+d)\n", i + defaultbytes, defaultbytes);
            
            i += howManyBytes;
        } else {
            printf("Invalid instruction opcode.\n");
            exit(7);
        }
    }
}

void display::code_attribute(CodeAttribute attribute_info, vector<Constant_pool_variables> constant_pool, int indentation) {
    CodeAttribute code = attribute_info;

    display::indentation(indentation);
    printf("%-30s %d\n", "Maximum stack:", code.max_stack);
    display::indentation(indentation);
    printf("%-30s %d\n", "Maximum local variables:", code.max_locals);
    display::indentation(indentation);
    printf("%-30s %d\n", "Code length:", code.code_length);
    display::indentation(indentation);
    printf("Exception table:\n");
    if (code.exception_table_length > 0) {
        // TODO
    } else {
        display::indentation(indentation+1);
        printf("Exception table is empty.\n");
    }

    display::indentation(indentation);
    printf("Bytecode:\n");
    display::byte_code(code, constant_pool, indentation);

    display::indentation(indentation);
    printf("Attributes:\n");
    for (u2 i = 0; i < code.attributes_count; i++) {
        display::attribute_info(code.attributes[i], constant_pool, indentation+1);
    }
}

void display::line_number_table_attribute(LineNumberTableAttribute attribute_info, vector<Constant_pool_variables> constant_pool, int indentation) {
    display::indentation(indentation+1);
    printf("%15s %s\n", "start_pc", "line_number");
    for (u2 i = 0; i < attribute_info.line_number_table_length; i++) {
        LineNumberTable table = attribute_info.line_number_table[i];
        display::indentation(indentation+1);
        printf("%9d %14d\n", table.start_pc, table.line_number);
    }
}

void display::source_file_attribute(SourceFileAttribute attribute_info, vector<Constant_pool_variables> constant_pool, int indentation) {
    display::indentation(indentation);
    printf("%-30s %15s cp_index #%d\n", "SourceFile:", get_constant_pool_element(constant_pool, attribute_info.source_file_index).c_str(), attribute_info.source_file_index);
}

void display::constant_value_attribute(ConstantValueAttribute attribute_info, vector<Constant_pool_variables> constant_pool, int indentation) {
    display::indentation(indentation);
    printf("%-30s %15s cp_index #%d\n", "Constant value:", get_constant_pool_element(constant_pool, attribute_info.constant_value_index).c_str(), attribute_info.constant_value_index);
}

void display::exceptions_attribute(ExceptionsAttribute attribute_info, vector<Constant_pool_variables> constant_pool, int indentation) {
    display::indentation(indentation);
    cout <<"\texception\tverbose" << endl;
    for (u2 i = 0; i < attribute_info.number_of_exceptions; i++) {
        display::indentation(indentation);
        cout << "\tcp_index #" << attribute_info.exception_index_table[i];
        cout << "\t\t" << get_constant_pool_element(constant_pool, attribute_info.exception_index_table[i]) << endl;
    }
}

void display::attribute_info(AttributeInfo attribute_info, vector<Constant_pool_variables> constant_pool, int indentation) {
    string attribute_name = get_constant_pool_element(constant_pool, attribute_info.attribute_name_index);
    display::indentation(indentation);
    printf("%s\n", attribute_name.c_str());
    display::indentation(indentation+1);
    printf("%-30s #%d\n", "Attribute name index:", attribute_info.attribute_name_index);
    display::indentation(indentation+1);
    printf("%-30s #%d\n", "Attribute length:", attribute_info.attribute_length);

    if(attribute_name == "ConstantValue") {
        display::constant_value_attribute(attribute_info.constant_value_attribute, constant_pool, indentation+1);
    } else if(attribute_name == "Code") {
        display::code_attribute(attribute_info.code_attribute, constant_pool, indentation+1);
    } else if(attribute_name == "LineNumberTable") {
        display::line_number_table_attribute(attribute_info.line_number_table_attribute, constant_pool, indentation+1);
    } else if(attribute_name == "SourceFile") {
        display::source_file_attribute(attribute_info.source_file_attribute, constant_pool, indentation+1);
    } else if(attribute_name == "Exceptions") {
        display::exceptions_attribute(attribute_info.exceptions_attribute, constant_pool, indentation+1);
    }

}

void display::constant_pool(ClassFile class_file) {
    vector<Constant_pool_variables> constant_pool = class_file.constant_pool;

    for (int i = 1; i < class_file.constant_pool_length; i++) {
        Constant_pool_variables element = constant_pool[i];

        display::indentation(1);
        printf("#%d ", i);

        switch (element.tag) {
            case CONSTANT_CLASS:
            {
                printf("Class\n");
                display::indentation(2);
                u2 name_index = element.info.class_info.name_index;
                u2 utf8_length = constant_pool[name_index].info.utf8_info.length;
                u1 *utf8_bytes = constant_pool[name_index].info.utf8_info.bytes;
                printf("%-15s %-30s cp_index #%d\n", "Class name: ", format_UTF8(utf8_length, utf8_bytes).c_str(), name_index);
            }
            break;

            case CONSTANT_FIELD_REF:
            {
                printf("Fieldref\n");

                // display class
                u2 class_index = element.info.field_ref_info.class_index;
                u2 name_index = constant_pool[class_index].info.class_info.name_index;
                u2 utf8_length = constant_pool[name_index].info.utf8_info.length;
                u1 *utf8_bytes = constant_pool[name_index].info.utf8_info.bytes;
                display::indentation(2);
                printf("%-15s %-30s cp_index #%d\n", "Class name: ", format_UTF8(utf8_length, utf8_bytes).c_str(), class_index);


                // get name and type information
                u2 name_and_type_index = element.info.field_ref_info.name_and_type_index;

                name_index = constant_pool[name_and_type_index].info.name_and_type_info.name_index;
                utf8_length = constant_pool[name_index].info.utf8_info.length;
                utf8_bytes = constant_pool[name_index].info.utf8_info.bytes;
                string name = format_UTF8(utf8_length, utf8_bytes);

                u2 descriptor_index = constant_pool[name_and_type_index].info.name_and_type_info.descriptor_index;
                utf8_length = constant_pool[descriptor_index].info.utf8_info.length;
                utf8_bytes = constant_pool[descriptor_index].info.utf8_info.bytes;
                name += format_UTF8(utf8_length, utf8_bytes); // descriptor

                display::indentation(2);
                printf("%-15s %-30s cp_index #%d\n", "Name and type: ", name.c_str(), name_and_type_index);
            }
            break;

            case CONSTANT_METHOD_REF:
            {
                printf("Methodref\n");

                // display class
                u2 class_index = element.info.method_ref_info.class_index;
                u2 name_index = constant_pool[class_index].info.class_info.name_index;
                u2 utf8_length = constant_pool[name_index].info.utf8_info.length;
                u1 *utf8_bytes = constant_pool[name_index].info.utf8_info.bytes;
                display::indentation(2);
                printf("%-15s %-30s cp_index #%d\n", "Class name: ", format_UTF8(utf8_length, utf8_bytes).c_str(), class_index);

                // get name and type information
                u2 name_and_type_index = element.info.method_ref_info.name_and_type_index;
                name_index = constant_pool[name_and_type_index].info.name_and_type_info.name_index;
                utf8_length = constant_pool[name_index].info.utf8_info.length;
                utf8_bytes = constant_pool[name_index].info.utf8_info.bytes;
                string name = format_UTF8(utf8_length, utf8_bytes);

                u2 descriptor_index = constant_pool[name_and_type_index].info.name_and_type_info.descriptor_index;
                utf8_length = constant_pool[descriptor_index].info.utf8_info.length;
                utf8_bytes = constant_pool[descriptor_index].info.utf8_info.bytes;
                name += format_UTF8(utf8_length, utf8_bytes); // descriptor

                display::indentation(2);
                printf("%-15s %-30s cp_index #%d\n", "Name and type: ", name.c_str(), name_and_type_index);
            }
            break;

            case CONSTANT_INTERFACE_METHOD_REF:
            {
                printf("Interface Method ref\n");
                u2 class_index = element.info.interface_method_ref_info.class_index;
                u2 name_index = constant_pool[class_index].info.class_info.name_index;
                u2 utf8_length = constant_pool[name_index].info.utf8_info.length;
                u1 *utf8_bytes = constant_pool[name_index].info.utf8_info.bytes;
                display::indentation(2);
                printf("%-15s %-30s cp_index #%d\n", "Class name: ", format_UTF8(utf8_length, utf8_bytes).c_str(), class_index);

                u2 name_and_type_index = element.info.interface_method_ref_info.name_and_type_index;
                name_index = constant_pool[name_and_type_index].info.name_and_type_info.name_index;
                utf8_length = constant_pool[name_index].info.utf8_info.length;
                utf8_bytes = constant_pool[name_index].info.utf8_info.bytes;
                string name = format_UTF8(utf8_length, utf8_bytes);

                u2 descriptor_index = constant_pool[name_and_type_index].info.name_and_type_info.descriptor_index;
                utf8_length = constant_pool[descriptor_index].info.utf8_info.length;
                utf8_bytes = constant_pool[descriptor_index].info.utf8_info.bytes;
                name += format_UTF8(utf8_length, utf8_bytes); // descriptor

                display::indentation(2);
                printf("%-15s %-30s cp_index #%d\n", "Name and type: ", name.c_str(), name_and_type_index);
            }
            break;

            case CONSTANT_NAME_AND_TYPE:
            {
                printf("NameAndType\n");
                u2 name_index = element.info.name_and_type_info.name_index;
                u2 descriptor_index = element.info.name_and_type_info.descriptor_index;

                u2 utf8_length = constant_pool[name_index].info.utf8_info.length;
                u1 *utf8_bytes = constant_pool[name_index].info.utf8_info.bytes;
                display::indentation(2);
                printf("%-15s %-30s cp_index #%d\n", "Name: ", format_UTF8(utf8_length, utf8_bytes).c_str(), name_index);

                utf8_length = constant_pool[descriptor_index].info.utf8_info.length;
                utf8_bytes = constant_pool[descriptor_index].info.utf8_info.bytes;
                display::indentation(2);
                printf("%-15s %-30s cp_index #%d\n", "Descriptor: ", format_UTF8(utf8_length, utf8_bytes).c_str(), name_index);
            }
            break;

            case CONSTANT_UTF8:
            {
                printf("Utf8\n");
                display::indentation(2);
                printf("%-15s %-30d\n", "Utf8 Length:", element.info.utf8_info.length);
                display::indentation(2);
                printf("%-15s %-30s\n", "Value:", format_UTF8(element.info.utf8_info.length, element.info.utf8_info.bytes).c_str());
            }
            break;

            case CONSTANT_STRING:
            {
                printf("String\n");
                u2 string_index = element.info.string_info.string_index;
                u2 utf8_length = constant_pool[string_index].info.utf8_info.length;
                u1 *utf8_bytes = constant_pool[string_index].info.utf8_info.bytes;
                display::indentation(2);
                printf("%-15s %-30s cp_index #%d\n", "String: ", format_UTF8(utf8_length, utf8_bytes).c_str(), string_index);
            }
            break;

            case CONSTANT_INTEGER:
            {
                printf("Integer\n");
                display::indentation(2);
                printf("%-15s 0x%-30.8X\n", "Bytes:", element.info.integer_info.bytes);
                display::indentation(2);
                printf("%-15s %-30d\n", "Integer:", int32_t(element.info.integer_info.bytes));
            }
            break;

            case CONSTANT_FLOAT:
            {
                u4 bytes = element.info.float_info.bytes;
                int32_t sig = ((bytes >> 31) == 0) ? 1 : -1;
                int32_t exponent = ((bytes >> 23) & 0xff);
                int32_t mantissa = (exponent == 0) ? (bytes & 0x7fffff) << 1 : (bytes & 0x7fffff) | 0x800000;
                float number = sig * mantissa * pow(2, exponent-150);

                printf("Float\n");
                display::indentation(2);
                printf("%-15s 0x%-30.8X\n", "Bytes:", bytes);
                display::indentation(2);
                printf("%-15s %-30f\n", "Float:", number);
            }
            break;

            case CONSTANT_LONG:
            {
                int64_t number = ((int64_t) element.info.long_info.high_bytes << 32) + element.info.long_info.low_bytes;
                printf("Long\n");
                display::indentation(2);
                printf("%-15s 0x%-30.8X\n", "High Bytes:", element.info.long_info.high_bytes);
                display::indentation(2);
                printf("%-15s 0x%-30.8X\n", "Low Bytes:", element.info.long_info.low_bytes);
                display::indentation(2);
                printf("%-15s %-30ld\n", "Long:", number);

                i++;
                display::indentation(1);
                printf("#%d Long continued\n", i);
            }
            break;

            case CONSTANT_DOUBLE:
            {
                int64_t bytes = ((int64_t) element.info.double_info.high_bytes << 32) + element.info.double_info.low_bytes;

                int32_t sig = ((bytes >> 63) == 0) ? 1 : -1;
                int32_t exponent = (int32_t)((bytes >> 52) & 0x7ffL);
                int64_t mantissa = (exponent == 0) ? (bytes & 0xfffffffffffffL) << 1 : (bytes & 0xfffffffffffffL) | 0x10000000000000L;
                double number = sig * mantissa * pow(2, exponent-1075);

                printf("Double\n");
                display::indentation(2);
                printf("%-15s 0x%-30.8X\n", "High Bytes:", element.info.double_info.high_bytes);
                display::indentation(2);
                printf("%-15s 0x%-30.8X\n", "Low Bytes:", element.info.double_info.low_bytes);
                display::indentation(2);
                printf("%-15s %-30f\n", "Double:", number);

                i++;
                printf("\n");
                display::indentation(1);
                printf("#%d Double continued\n", i);

            }
            break;
		}
    }

}

void display::interfaces(ClassFile class_file) {
	for (u2 i = 0; i < class_file.interfaces_count; i++) {
        display::indentation(1);
        printf("%s %d\n", "Interface", i);
        display::indentation(2);
        string class_name = get_constant_pool_element(class_file.constant_pool, class_file.interfaces[i]);
        printf("%-15s %-30s cp_index #%d\n", "Interface:", class_name.c_str(), class_file.interfaces[i]);
	}
}

void display::method(MethodInfo method, vector<Constant_pool_variables> constant_pool, int indentation) {
    display::indentation(indentation);
    printf("%-15s %-30s cp_index #%d\n", "Name:", get_constant_pool_element(constant_pool, method.name_index).c_str(), method.name_index);

    display::indentation(indentation);
    printf("%-15s %-30s cp_index #%d\n", "Descriptor:", get_constant_pool_element(constant_pool, method.descriptor_index).c_str(), method.descriptor_index);

    display::indentation(indentation);
    printf("%-15s (0x%.4X)\n", "Access Flags:", method.access_flags);
    display::indentation(indentation+1);
    display::access_flags(method.access_flags);

    display::indentation(indentation);
    printf("%s\n", "Attributes:");
    if(method.attributes_count == 0) {
        display::indentation(indentation+1);
        printf("%s\n", "Attributes is empty.");
    }

    for (u2 j = 0; j < method.attributes_count; j++) {
        display::attribute_info(method.attributes[j], constant_pool, indentation+1);
    }
}

void display::methods(ClassFile class_file, int indentation) {
    for (u2 i = 0; i < class_file.methods_count; i++) {
        MethodInfo method = class_file.methods[i];
        string method_name = get_constant_pool_element(class_file.constant_pool, method.name_index);
        display::indentation(indentation);
		printf("%s\n", method_name.c_str());
		display::method(method, class_file.constant_pool, indentation+1);
    }

}

void display::field(FieldInfo field, vector<Constant_pool_variables> constant_pool, int indentation) {
    display::indentation(indentation);
    printf("%-15s %-30s cp_index #%d\n", "Name:", get_constant_pool_element(constant_pool, field.name_index).c_str(), field.name_index);

    display::indentation(indentation);
    printf("%-15s %-30s cp_index #%d\n", "Descriptor:", get_constant_pool_element(constant_pool, field.descriptor_index).c_str(), field.descriptor_index);

    display::indentation(indentation);
    printf("%-15s (0x%.4X)\n", "Access Flags:", field.access_flags);
    display::indentation(indentation+1);
    display::access_flags(field.access_flags);

    display::indentation(indentation);
    printf("%s\n", "Attributes:");
    if(field.attributes_count == 0) {
        display::indentation(indentation+1);
        printf("%s\n", "Attributes is empty.");
    }

    for (u2 j = 0; j < field.attributes_count; j++) {
        display::attribute_info(field.attributes[j], constant_pool, indentation+1);
    }
}

void display::fields(ClassFile class_file, int indentation) {
	for (u2 i = 0; i < class_file.fields_count; i++) {
        FieldInfo field = class_file.fields[i];
        display::indentation(indentation);
        string field_name = get_constant_pool_element(class_file.constant_pool, field.name_index);
		printf("%s\n", field_name.c_str());
		display::field(field, class_file.constant_pool, indentation+1);
	}
}

void display::attributes(ClassFile class_file, int indentation) {
    for (u2 i = 0; i < class_file.attributes_count; i++) {
        display::attribute_info(class_file.attributes[i], class_file.constant_pool, indentation);
        printf("\n");
    }
}

void display::indentation(int indentation) {
    for(int i = 0; i < indentation; i++) {
        printf("\t");
    }
}
