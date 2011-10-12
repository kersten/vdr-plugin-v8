#include "v8plugin.h"

void Plugin::init(char* source_file) {
    dsyslog("V8: Initalizing new plugin: %s", source_file);

    Handle<v8::String> file_name = String::New(source_file);
    Handle<v8::String> source = ReadFile(source_file);

    if (source.IsEmpty()) {
        printf("Error reading '%s'\n", source_file);
    }

    if (!ExecuteString(source, file_name, false, true)) {
        //ExitShell(1);
        return;
    }

    Handle<ObjectTemplate> global = ObjectTemplate::New();

    //global->Set(String::New("require"), FunctionTemplate::New(Require));
    //global->Set(String::New("dsyslog"), FunctionTemplate::New(Print));
}

// Extracts a C string from a V8 Utf8Value.
const char* Plugin::ToCString(const String::Utf8Value& value) {
    return *value ? *value : "<string conversion failed>";
}

bool Plugin::ExecuteString(v8::Handle<v8::String> source,
        v8::Handle<v8::Value> name,
        bool print_result,
        bool report_exceptions) {
    v8::HandleScope handle_scope;
    v8::TryCatch try_catch;
    v8::Handle<v8::Script> script = v8::Script::Compile(source, name);
    if (script.IsEmpty()) {
        // Print errors that happened during compilation.
        if (report_exceptions)
            //ReportException(&try_catch);
        return false;
    } else {
        v8::Handle<v8::Value> result = script->Run();
        if (result.IsEmpty()) {
            //assert(try_catch.HasCaught());
            // Print errors that happened during execution.
            if (report_exceptions)
                //ReportException(&try_catch);
            return false;
        } else {
            //assert(!try_catch.HasCaught());
            if (print_result && !result->IsUndefined()) {
                // If all went well and the result wasn't undefined then print
                // the returned value.
                v8::String::Utf8Value str(result);
                const char* cstr = ToCString(str);
                printf("%s\n", cstr);
            }
            return true;
        }
    }
}

// Reads a file into a v8 string.

Handle<v8::String> Plugin::ReadFile(const char* name) {
    FILE* file = fopen(name, "rb");
    if (file == NULL) return v8::Handle<v8::String > ();

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    char* chars = new char[size + 1];
    chars[size] = '\0';
    for (int i = 0; i < size;) {
        int read = fread(&chars[i], 1, size - i, file);
        i += read;
    }
    fclose(file);
    v8::Handle<v8::String> result = v8::String::New(chars, size);
    delete[] chars;
    return result;
}

Handle<Value> Plugin::Require(const Arguments& args) {
    for (int i = 0; i < args.Length(); i++) {
        String::Utf8Value str(args[i]);

        // load_file loads the file with this name into a string,
        // I imagine you can write a function to do this :)
        //std::string js_file = load_file(*str);

        /*if (js_file.length() > 0) {
            Handle<String> source = String::New(js_file.c_str());
            Handle<Script> script = Script::Compile(source);
            return script->Run();
        }*/
    }
    return Undefined();
}

Handle<Value> Plugin::Print(const Arguments& args) {
    bool first = true;

    for (int i = 0; i < args.Length(); i++) {
        HandleScope handle_scope;

        if (first) {
            first = false;
        } else {
            dsyslog(" ");
        }

        String::Utf8Value str(args[i]);
        dsyslog("V8: %s", *str);
    }

    return Undefined();
}