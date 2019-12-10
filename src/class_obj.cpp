#include "class_obj.hpp"

ClassObject::ClassObject(ClassRuntime *class_run_time) : class_run_time(class_run_time) {
    
    ClassFile *class_file = class_run_time->class_file;

    vector<FieldInfo> all_fields = class_file->fields;
    
    for (int i = 0; i < class_file->fields_count; i++) {
        
        FieldInfo field = all_fields[i];
        
        u2 static_and_final_flag = 0x0008 | 0x0010;
        
        if ((field.access_flags & static_and_final_flag) == 0) { 
            string field_name = get_constant_pool_element(class_file->constant_pool, field.name_index);
            string field_descriptor = get_constant_pool_element(class_file->constant_pool, field.descriptor_index);
            
            char field_type = field_descriptor[0];
            Variable variable;
            
            switch (field_type) {
                case 'B':
                    variable.type = BYTE;
                    variable.data.v_byte = 0;
                    break;
                case 'C':
                    variable.type = CHAR;
                    variable.data.v_char = 0;
                    break;
                case 'D':
                    variable.type = DOUBLE;
                    variable.data.v_double = 0;
                    break;
                case 'F':
                    variable.type = FLOAT;
                    variable.data.v_float = 0;
                    break;
                case 'I':
                    variable.type = INT;
                    variable.data.v_int = 0;
                    break;
                case 'J':
                    variable.type = LONG;
                    variable.data.v_long = 0;
                    break;
                case 'S':
                    variable.type = SHORT;
                    variable.data.v_short = 0;
                    break;
                case 'Z':
                    variable.type = BOOLEAN;
                    variable.data.v_boolean = false;
                    break;
                default:
                    variable.type = REFERENCE;
                    variable.data.object = NULL;
            }
            
            fields[field_name] = variable;
        }
    }
}


ObjectType ClassObject::object_type() {
    return CLASS;
}