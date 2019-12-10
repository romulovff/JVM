#include "utils.hpp"

string format_UTF8(u2 utf8_length, u1 *utf8_bytes) {
    string formated_string = "";

	for(int i = 0; i < utf8_length; i++){
        if (!(utf8_bytes[i] & 0x80)) { 
            // 1 byte
            // check if it is 1 byte encoded:
            // if the bit 7 is 0
            // is the same as if bit 7 is not 1
			formated_string += (char)utf8_bytes[i];
		} else {
			if (!(utf8_bytes[i+1] & 0x20)) { 
                // 2 bytes
                // check if it is 2 byte encoded:
                // if the bit 5 is 0
                // is the same as if the bit 5 is not 1
				formated_string += char(((utf8_bytes[i] & 0x1f) << 6) + (utf8_bytes[i+1] & 0x3f));
                i++;
			} else { 
                // 3 bytes
				formated_string += char(((utf8_bytes[i] & 0xf) << 12) + ((utf8_bytes[i+1] & 0x3f) << 6) + (utf8_bytes[i+2] & 0x3f));
				i += 2;
			}
		}
	}

    return formated_string;
}

void check_magic_number(u4 magic_number) {
    if(magic_number != 0xcafebabe) {
        cout << "missing cafe babe" << endl;
        exit(1);
    }
}

string get_constant_pool_element(vector<Constant_pool_variables> constant_pool, u2 index) {
    u2 tag  = constant_pool[index].tag;

    switch (tag) {
        case CONSTANT_CLASS:
        {
            u2 name_index = constant_pool[index].info.class_info.name_index;

            u2 utf8_length = constant_pool[name_index].info.utf8_info.length;
            u1 *utf8_bytes = constant_pool[name_index].info.utf8_info.bytes;
            return format_UTF8(utf8_length, utf8_bytes);
        }

        case CONSTANT_FIELD_REF:
        {
            FieldRefInfo field_ref_info = constant_pool[index].info.field_ref_info;
            NameAndTypeInfo name_and_type_info = constant_pool[field_ref_info.name_and_type_index].info.name_and_type_info;
            
            string class_name = get_constant_pool_element(constant_pool, field_ref_info.class_index);
            string name = get_constant_pool_element(constant_pool, name_and_type_info.name_index);
            
            return class_name + "." + name;
        }
        break;

        case CONSTANT_METHOD_REF:
        {
            MethodRefInfo method_ref_info = constant_pool[index].info.method_ref_info;
            NameAndTypeInfo name_and_type_info = constant_pool[method_ref_info.name_and_type_index].info.name_and_type_info;
            
            string class_name = get_constant_pool_element(constant_pool, method_ref_info.class_index);
            string name = get_constant_pool_element(constant_pool, name_and_type_info.name_index);
            
            return class_name + "." + name;
        }
        break;

        case CONSTANT_INTERFACE_METHOD_REF:
        {
            InterfaceMethodRefInfo interface_method_ref_info = constant_pool[index].info.interface_method_ref_info;
            NameAndTypeInfo name_and_type_info = constant_pool[interface_method_ref_info.name_and_type_index].info.name_and_type_info;
            
            string class_name = get_constant_pool_element(constant_pool, interface_method_ref_info.class_index);
            string name = get_constant_pool_element(constant_pool, name_and_type_info.name_index);
            
            return class_name + "." + name;
        }
        break;
        
        case CONSTANT_NAME_AND_TYPE:
        {
            NameAndTypeInfo name_and_type_info = constant_pool[index].info.name_and_type_info;
        
            string name = get_constant_pool_element(constant_pool, name_and_type_info.name_index);
            string descriptor = get_constant_pool_element(constant_pool, name_and_type_info.descriptor_index);
            
            return name + descriptor;
        }
        break;

        case CONSTANT_UTF8:
        {
            u2 utf8_length = constant_pool[index].info.utf8_info.length;
            u1 *utf8_bytes = constant_pool[index].info.utf8_info.bytes;
            return format_UTF8(utf8_length, utf8_bytes);
        }
        break;

        case CONSTANT_STRING:
        {
            StringInfo string_info = constant_pool[index].info.string_info;
            return get_constant_pool_element(constant_pool, string_info.string_index);
        }
        break;

        case CONSTANT_INTEGER:
        {
            IntegerInfo int_info = constant_pool[index].info.integer_info;
            int32_t number = int_info.bytes;
            return to_string(number);
        }
        break;

        case CONSTANT_FLOAT:
        {
            FloatInfo float_info = constant_pool[index].info.float_info;
            int32_t s = ((float_info.bytes >> 31) == 0) ? 1 : -1;
            int32_t e = ((float_info.bytes >> 23) & 0xff);
            int32_t m = (e == 0) ? (float_info.bytes & 0x7fffff) << 1 : (float_info.bytes & 0x7fffff) | 0x800000;
            float number = s * m * pow(2, e-150);
            
            return to_string(number);
        }
        break;
        
        case CONSTANT_LONG:
        {
            LongInfo long_info = constant_pool[index].info.long_info;
            int64_t number = ((int64_t) long_info.high_bytes << 32) + long_info.low_bytes;
            
            return to_string(number);
        }
        break;
        
        case CONSTANT_DOUBLE:
        {
            DoubleInfo double_info = constant_pool[index].info.double_info;
            int64_t bytes = ((int64_t) double_info.high_bytes << 32) + double_info.low_bytes;
            
            int32_t s = ((bytes >> 63) == 0) ? 1 : -1;
            int32_t e = (int32_t)((bytes >> 52) & 0x7ffL);
            int64_t m = (e == 0) ? (bytes & 0xfffffffffffffL) << 1 : (bytes & 0xfffffffffffffL) | 0x10000000000000L;
            double number = s * m * pow(2, e-1075);
            
            return to_string(number);
        }
        break;
    }
}

void format_path(string path, string* file_class_name, string* path_prefix) {

    const size_t last_slash_idx = path.find_last_of("\\/");

    if (string::npos != last_slash_idx) {
        *path_prefix = path.substr(0, last_slash_idx + 1);
        path.erase(0, last_slash_idx + 1);
    }

    // Remove extension if present.
    const size_t period_idx = path.rfind('.');
    if (string::npos != period_idx) {
        path.erase(period_idx);
    }

    *file_class_name = path;
}

void fill_multi_array(ArrayObject* array, VariableType var_type, stack<int> dim_length) {
    int curr_dim = dim_length.top();
    dim_length.pop();
    
    VariableType array_type = (dim_length.size() > 1) ? REFERENCE : var_type;
    
    if (dim_length.size() == 0) {
        for (int i = 0; i < curr_dim; i++) {
            Variable array_var;
            array_var.type = var_type;
            array_var.data.v_long = 0;
            array->elements.push_back(array_var);
        }
    } else {
        for (int i = 0; i < curr_dim; i++) {
            ArrayObject *sub_array = new ArrayObject(var_type);
            fill_multi_array(sub_array, var_type, dim_length);
            
            Variable array_var;
            array_var.type = REFERENCE;
            array_var.data.object = sub_array;
            array->elements.push_back(array_var);
        }
    }
}