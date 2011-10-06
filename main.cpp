/* 
 * File:   main.cpp
 * Author: kersten
 *
<<<<<<< HEAD
 * Created on 30. September 2011, 15:09
 */

#include <iostream>
#include <v8.h>
#include <string>

using namespace v8;

/*
 * 
 */
int main() {
    HandleScope handle_scope;

    // Create a new context.
    Persistent<Context> context = Context::New();

    // Enter the created context for compiling and
    // running the hello world script.
    Context::Scope context_scope(context);

    // Create a string containing the JavaScript source code.
    Handle<String> source = String::New("'Hello' + ', World!'");

    // Compile the source code.
    Handle<Script> script = Script::Compile(source);

    // Run the script to get the result.
    Handle<Value> result = script->Run();

    // Dispose the persistent context.
    context.Dispose();

    // Convert the result to an ASCII string and print it.
    String::AsciiValue ascii(result);
    printf("%s\n", *ascii);
=======
 * Created on 30. September 2011, 19:37
 */

#include <cstdlib>
#include <v8.h>

using namespace std;
using namespace v8;
/*
 * 
 */
int main(int argc, char** argv) {
>>>>>>> 2aac7494bd5c4bc646550ef517af1bc222b408fc
    return 0;
}

