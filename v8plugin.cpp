#include "v8plugin.h"

Handle<Value> v8dsyslog(const Arguments& args) {
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

Handle<Value> Plugin::VDR(const Arguments& args)
{
    HandleScope handle_scope;
    
    return Undefined();
}

void Plugin::init(char* source_file, cMenuSetupV8 *menu) {
    pluginname = source_file;
    
    HandleScope handle_scope;
    dsyslog("V8: Initalizing new plugin: %s", source_file);
    
    //menu->Add(new cOsdItem(tr("General")));
    
    Handle<String> source = ReadFile(source_file);
    
    Handle<ObjectTemplate> global = ObjectTemplate::New();
    global->Set(String::New("dsyslog"), FunctionTemplate::New(v8dsyslog));
    global->Set(String::New('VDR'), FunctionTemplate::New(Plugin::VDR));
    
    Persistent<Context> context = Context::New(NULL, global);
    
    Context::Scope context_scope(context);

    if (source.IsEmpty()) {
        dsyslog("Error reading '%s'\n", source_file);
        return;
    }
    
    Handle<Script> script = Script::Compile(source);
    
    script->Run();
    
    context.Dispose();

    /*if (!ExecuteString(source, file_name, false, true)) {
        //ExitShell(1);
        dsyslog("Error executing script");
    }*/
}

// Extracts a C string from a V8 Utf8Value.
const char* Plugin::ToCString(const String::Utf8Value& value) {
    return *value ? *value : "<string conversion failed>";
}

bool Plugin::ExecuteString(Handle<String> source,
        Handle<Value> name,
        bool print_result,
        bool report_exceptions) {
    HandleScope handle_scope;
    TryCatch try_catch;
    Handle<Script> script = Script::Compile(source, name);
    if (script.IsEmpty()) {
        // Print errors that happened during compilation.
        if (report_exceptions)
            //ReportException(&try_catch);
        return false;
    } else {
        Handle<Value> result = script->Run();
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
                String::Utf8Value str(result);
                const char* cstr = ToCString(str);
                printf("%s\n", cstr);
            }
            return true;
        }
    }
    
    return false;
}

// Reads a file into a v8 string.

Handle<String> Plugin::ReadFile(const char* name) {
    dsyslog("V8: open file: %s", name);
    FILE* file = fopen(name, "rb");
    if (file == NULL) return v8::Handle<String > ();

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
    Handle<String> result = String::New(chars, size);
    //dsyslog(chars);
    delete[] chars;
    return result;
}

Handle<Value> Plugin::Require(const Arguments& args) {
    for (int i = 0; i < args.Length(); i++) {
        // load_file loads the file with this name into a string,
        // I imagine you can write a function to do this :)
        //std::string js_file = load_file(*str);
        //Handle<v8::String> source = ReadFile(char* args[i]);
        //Handle<Script> script = Script::Compile(source);
        //return script->Run();
        
        /*if (js_file.length() > 0) {
            Handle<String> source = String::New(js_file.c_str());
            Handle<Script> script = Script::Compile(source);
            return script->Run();
        }*/
    }
    return Undefined();
}

char* Plugin::getName()
{
    return pluginname;
}
