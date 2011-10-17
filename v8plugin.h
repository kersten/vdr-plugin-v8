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

#include "v8menusetup.h"

using namespace v8;

class Plugin {
public:
    Plugin() {};
    ~Plugin() {};
    
    void init(char* source_file, cMenuSetupV8 *menu);
    char* getName();
private:
    bool ExecuteString(Handle<String> source,
        v8::Handle<Value> name,
        bool print_result,
        bool report_exceptions);
    
    Handle<Value> VDR(const Arguments& args);
    Handle<String> ReadFile(const char* name);
    Handle<Value> Require(const Arguments& args);
    //Handle<Value> v8dsyslog(const Arguments& args);
    const char* ToCString(const String::Utf8Value& value);
    
    char* pluginname;
};

#endif	/* PLUGIN_H */

