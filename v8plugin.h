/* 
 * File:   Plugin.h
 * Author: kersten
 *
 * Created on 12. Oktober 2011, 11:53
 */

#ifndef PLUGIN_H
#define	PLUGIN_H

#include <vdr/tools.h>
#include <v8.h>

using namespace v8;

class Plugin {
public:
    Plugin() {};
    ~Plugin() {};
    
    void init(char* source_file);
private:
    bool ExecuteString(v8::Handle<v8::String> source,
        v8::Handle<v8::Value> name,
        bool print_result,
        bool report_exceptions);
    Handle<v8::String> ReadFile(const char* name);
    Handle<Value> Require(const Arguments& args);
    Handle<Value> Print(const Arguments& args);
    const char* ToCString(const String::Utf8Value& value);
};

#endif	/* PLUGIN_H */

