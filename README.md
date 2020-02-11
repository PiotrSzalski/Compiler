# Compiler-JFTT2019

Compiler of a simple imperative language, created as a final project for 'Formal languages and translation techniques' course. \
Specyfication is located in [specification.pdf](https://github.com/PiotrSzalski/Compiler/blob/master/specification.pdf) file. \
The compiler provides features such as:
- arrays and variables declarations
- for, down for, while and do while loops
- if/else blocks
- user input/output
- mathematical operations: addition, subtraction, multiplication, division, modulo (definition of modulo and division you can read in [specification.pdf](https://github.com/PiotrSzalski/Compiler/blob/master/specification.pdf) file)
- working only with integers both positive and negative

# Building
Source files are located in `./compiler` directory. To build the compiler you will need:
| tool  | version |
|-------|---------|
| flex  | 2.6.4   |
| bison | 3.0.4   |
| g++   | 7.4.0   |
| make  | 4.1     |

To compile compiler run command `make` in `./compiler` folder. This will create exec file `kompilator`.

# Usage
You can write your own program according to specification or choose one from `./examples` folder. 
To compile program run command `./kompilator <input_file> <output_file>`. 
If program is correct output file should appear, if not error message.\
To run compiled program you can use virtual machine located in `./machine` folder, to build machine run `make` command in `./machine` folder.

#
My compiler took 9'th place in final ranking of all compilers. Ranking evaluated correctness and speed of the resulting code 
on the virtual machine. Ranking contained over 100 compilers.
