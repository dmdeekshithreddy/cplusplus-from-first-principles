// anything that begings with a `#` is a preprocessor statement. 
// It is used to include the contents of a file or to define macros. 
// In this case, we are including the iostream library, which allows us to use input and output streams.
// compiler will pre-processes all of the preprocessor statements before compiling the code.
// This means that the compiler will replace the line `#include <iostream>` with the contents of the iostream library before compiling the code.
// Compiler literally copies and pastes the contents of the iostream library into our code before compiling it.
// these files we include are called header files. They contain declarations of functions and classes that we can use in our code.

#include <iostream> 

using namespace std;

// The main function is the entry point of a C++ program. It is where the execution of the program begins.
// main function must return an integer value, which is typically used to indicate the success or failure of the program.
// If no return statement is provided, the compiler automatically inserts `return 0;` at the end of main (a special exception only for main).
int main() {
    // The `cout` object is used to output data to the console.
    // The `<<` operator is used to insert data into the output stream.
    // The `endl` manipulator is used to insert a newline character and flush the output buffer.
    // `<<` is an overloaded operator that takes two operands: the output stream (cout) on the left and the data to be inserted on the right (can be a string, number, or any supported type).
    cout << "Hello World in C++" << endl;
}
