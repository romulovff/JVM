main: src/main.cpp src/utils.cpp src/class_loader.cpp src/reader.cpp src/attribute.cpp src/display.cpp src/array_obj.cpp src/class_obj.cpp src/string_obj.cpp src/class_run_time.cpp src/frame.cpp src/instructions.cpp src/engine.cpp
	g++ -std=c++11 -o bin/main src/main.cpp src/utils.cpp src/class_loader.cpp src/reader.cpp src/attribute.cpp src/display.cpp src/array_obj.cpp src/class_obj.cpp src/string_obj.cpp src/class_run_time.cpp src/frame.cpp src/instructions.cpp src/engine.cpp

run: main
	bin/main exp/double_aritmetica.class
