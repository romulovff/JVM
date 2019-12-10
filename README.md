# Java virtual machine

## Build

To create the binary file execute the following steps:

1. Create the `bin` folder with the command:
    * ``` mkdir bin```

2. Run the command:
    * ```make main```

## Execution

To run the program execute the command with the path to a java class file: ``` ./bin/main path/to/class_file.class``` 

Nothing will be display on the terminal to do so choose at least one of the following flags:

1. Bytecode viewer:
    * Flag : `-e`
    * Display the java class bytecode  

2. Bytecode interpreter:
    * Flag: `-i`
    * Run the java class file