#include "display.hpp"
#include "engine.hpp"
#include "class_loader.hpp"

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("[FALHA] Formato invalido. Utilize o caminho do arquivo \".class\" como argumento.\n");
        exit(1);
    }

    char* file_path;

    bool interpretador = false;
    bool exibidor = false;

    for (size_t optind = 0; optind < argc; optind++) {
        if (argv[optind][0] == '-') {
            if(argv[optind][1] == 'i') interpretador = true;
            if(argv[optind][1] == 'e') exibidor = true; 
        } else {
            file_path = argv[optind];
        }        
    }

    string file_class_name;
    string file_path_prefix;
    format_path(file_path, &file_class_name, &file_path_prefix);
    set_path_files(file_path_prefix);

    ClassRuntime* class_run_time = build_class(file_class_name);

    if(exibidor){
        display::class_file(*class_run_time->class_file);
    }

    if(interpretador) {
        Engine engine;
        engine.start(class_run_time);
    }
}
