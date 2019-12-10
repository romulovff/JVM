#include "class_loader.hpp"

// global variable to save already loaded classes
map<string, ClassRuntime*> loaded_classes;

string files_path = "";

void set_path_files(string path) {
    files_path = path;
}

ClassRuntime* build_class(string file_name) {

    string class_path = files_path + file_name + ".class";
    
    // cout << "carregou a classe " << class_path << endl;
    
    if(loaded_classes.count(class_path) != 0) {
        // cout << file_name << " sendo recarregado" << endl;
        return loaded_classes[class_path];
    }
    
    FILE* file_pointer = fopen(class_path.c_str(), "rb");
    if(file_pointer == NULL) {
        cerr << "A classe " << class_path << " nao foi encontrada!" << endl;
        exit(1);
    }

    // this variable holds all the information about .class file
    ClassFile *class_file = new ClassFile();

    // set magic number and check if it is 0xcafebabe
    set_magic_number(file_pointer, class_file);

    // set versions
    set_version(file_pointer, class_file);

    // set constant pool length and create it
    set_constant_pool(file_pointer, class_file);

    // set access flags
    set_access_flags(file_pointer, class_file);

    // set this class
    set_this_class(file_pointer, class_file);

    // set super class
    set_super_class(file_pointer, class_file);

    // set interface size and create it
    set_interfaces(file_pointer, class_file);

    // set fields size and create it
    set_fields(file_pointer, class_file);

    // set methods size and create it
    set_methods(file_pointer, class_file);

    // set attributes size and create it
    set_attributes(file_pointer, class_file);

    fclose(file_pointer);

    check_class_name(class_file, file_name);

    return loaded_classes[class_path] = new ClassRuntime(class_file);
}

void check_class_name(ClassFile* class_file, string file_name) {
    string class_file_name = get_constant_pool_element(class_file->constant_pool, class_file->this_class);
    if (file_name != class_file_name) {
        std::cerr << std::endl << "Nome do arquivo ( " << file_name << " ) diferente do nome da classe ( " << class_file_name << " )" << std::endl << std::endl;
        exit(1);
    }
}

void set_magic_number(FILE* file_pointer, ClassFile* class_file) {
    class_file->magic_number = Reader::read_u4(file_pointer);
    check_magic_number(class_file->magic_number);
}

void set_version(FILE* file_pointer, ClassFile* class_file) {
    // get min version
    class_file->min_version = Reader::read_u2(file_pointer);
    
    // get max version
    class_file->major_version = Reader::read_u2(file_pointer);
}

void set_constant_pool(FILE* file_pointer, ClassFile* class_file) {
    // get constant pool length
    class_file->constant_pool_length = Reader::read_u2(file_pointer);
    class_file->constant_pool = std::vector<Constant_pool_variables>(class_file->constant_pool_length);

    // create constant pool
    for(u2 i = 1; i < class_file->constant_pool_length; i++) {
        Constant_pool_variables current_variable;

        current_variable.tag = Reader::read_u1(file_pointer);

        // std::cout << i << " = " << int(current_variable.tag) << std::endl;

        switch (current_variable.tag) {
            case CONSTANT_CLASS:
                current_variable.info.class_info.name_index = Reader::read_u2(file_pointer);
            break;

            case CONSTANT_FIELD_REF:
                current_variable.info.field_ref_info.class_index = Reader::read_u2(file_pointer);            
                current_variable.info.field_ref_info.name_and_type_index = Reader::read_u2(file_pointer);            
            break;

            case CONSTANT_METHOD_REF:
                current_variable.info.method_ref_info.class_index = Reader::read_u2(file_pointer);            
                current_variable.info.method_ref_info.name_and_type_index = Reader::read_u2(file_pointer);
            break;

            case CONSTANT_INTERFACE_METHOD_REF:
                current_variable.info.interface_method_ref_info.class_index = Reader::read_u2(file_pointer);            
                current_variable.info.interface_method_ref_info.name_and_type_index = Reader::read_u2(file_pointer);
            break;
            
            case CONSTANT_NAME_AND_TYPE:
                current_variable.info.name_and_type_info.name_index = Reader::read_u2(file_pointer);
                current_variable.info.name_and_type_info.descriptor_index = Reader::read_u2(file_pointer);
            break;

            case CONSTANT_UTF8:
                current_variable.info.utf8_info.length = Reader::read_u2(file_pointer);
                current_variable.info.utf8_info.bytes = (u1*) malloc(sizeof(u1) * current_variable.info.utf8_info.length);

                for (u2 i = 0; i < current_variable.info.utf8_info.length; i++) {
                    current_variable.info.utf8_info.bytes[i] = Reader::read_u1(file_pointer);
                }
            break;

            case CONSTANT_STRING:
                current_variable.info.string_info.string_index = Reader::read_u2(file_pointer);
            break;

            case CONSTANT_INTEGER:
                current_variable.info.integer_info.bytes = Reader::read_u4(file_pointer);
            break;

            case CONSTANT_FLOAT:
                current_variable.info.float_info.bytes = Reader::read_u4(file_pointer);
            break;
            
            case CONSTANT_LONG:
                current_variable.info.long_info.high_bytes = Reader::read_u4(file_pointer);
                current_variable.info.long_info.low_bytes = Reader::read_u4(file_pointer);
            break;

            case CONSTANT_DOUBLE:
                current_variable.info.double_info.high_bytes = Reader::read_u4(file_pointer);
                current_variable.info.double_info.low_bytes = Reader::read_u4(file_pointer);
            break;
            
            default:
                std::cout << "Tag invalida no pool de constantes" << std::endl;
                exit(1);
		}

        class_file->constant_pool[i] = current_variable;
        if(current_variable.tag == CONSTANT_LONG || current_variable.tag == CONSTANT_DOUBLE) i++;
    }

}

void set_access_flags(FILE* file_pointer, ClassFile* class_file) {
    class_file->access_flags = Reader::read_u2(file_pointer);
}

void set_this_class(FILE* file_pointer, ClassFile* class_file) {
    class_file->this_class = Reader::read_u2(file_pointer);
}

void set_super_class(FILE* file_pointer, ClassFile* class_file) {
    class_file->super_class = Reader::read_u2(file_pointer);
}

void set_interfaces(FILE* file_pointer, ClassFile* class_file) {
    // get interface size
    class_file->interfaces_count = Reader::read_u2(file_pointer);

    // create each interface
    class_file->interfaces = std::vector<u2>(class_file->interfaces_count);
    for (u2 i = 0; i < class_file->interfaces_count; i++) {
        class_file->interfaces[i] = Reader::read_u2(file_pointer);
    }
}

void set_fields(FILE* file_pointer, ClassFile* class_file) {
    // get field size
    class_file->fields_count = Reader::read_u2(file_pointer);
    class_file->fields = std::vector<FieldInfo>(class_file->fields_count);

    // create each field
    for (u2 i = 0; i < class_file->fields_count; i++) {
        FieldInfo field;
        
        field.access_flags = Reader::read_u2(file_pointer);
        field.name_index = Reader::read_u2(file_pointer);
        field.descriptor_index = Reader::read_u2(file_pointer);
        field.attributes_count = Reader::read_u2(file_pointer);

        field.attributes = std::vector<AttributeInfo>(field.attributes_count);
        
        for (u2 j = 0; j < field.attributes_count; j++) {
            field.attributes[j] = get_attribute_info(file_pointer, class_file->constant_pool);
        }
        
        class_file->fields[i] = field;
    }
}

void set_methods(FILE* file_pointer, ClassFile* class_file) {
    // get method size
    class_file->methods_count = Reader::read_u2(file_pointer);
    class_file->methods = std::vector<MethodInfo>(class_file->methods_count);

    // create each method
    for (u2 i = 0; i < class_file->methods_count; i++) {
        MethodInfo method;
        
        method.access_flags = Reader::read_u2(file_pointer);
        method.name_index = Reader::read_u2(file_pointer);
        method.descriptor_index = Reader::read_u2(file_pointer);
        method.attributes_count = Reader::read_u2(file_pointer);

        method.attributes = std::vector<AttributeInfo>(method.attributes_count);
        
        for (u2 j = 0; j < method.attributes_count; j++) {
            method.attributes[j] = get_attribute_info(file_pointer, class_file->constant_pool);
        }
        
        class_file->methods[i] = method;
    }
}

void set_attributes(FILE* file_pointer, ClassFile* class_file) {
    // get attributes size
    class_file->attributes_count = Reader::read_u2(file_pointer);
    class_file->attributes = std::vector<AttributeInfo>(class_file->attributes_count);

    // create each attribute
    for (u2 i = 0; i < class_file->attributes_count; i++) {
        class_file->attributes[i] = get_attribute_info(file_pointer, class_file->constant_pool);
    }
}