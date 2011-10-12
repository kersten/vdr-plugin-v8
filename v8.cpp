/*
 * v8.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include <vdr/plugin.h>
#include <v8.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include "Plugin.h"

using namespace v8;

static const char *VERSION = "0.0.1";
static const char *DESCRIPTION = "VDR plugins written in JavaScript";
static const char *MAINMENUENTRY = "V8";

class cPluginV8 : public cPlugin {
private:
        const char* plugin_dir;
public:
	cPluginV8(void);
	virtual ~cPluginV8();
	virtual const char *Version(void) {return VERSION;}
	virtual const char *Description(void) {return DESCRIPTION;}
	virtual const char *CommandLineHelp(void);
	virtual bool ProcessArgs(int argc, char *argv[]);
	virtual bool Initialize(void);
	virtual bool Start(void);
	virtual void Stop(void);
	virtual void Housekeeping(void);
	virtual void MainThreadHook(void);
	virtual cString Active(void);
	virtual time_t WakeupTime(void);
	virtual const char *MainMenuEntry(void) {return MAINMENUENTRY;}
	virtual cOsdObject *MainMenuAction(void);
	virtual cMenuSetupPage *SetupMenu(void);
	virtual bool SetupParse(const char *Name, const char *Value);
	virtual bool Service(const char *Id, void *Data = NULL);
	virtual const char **SVDRPHelpPages(void);
	virtual cString SVDRPCommand(const char *Command, const char *Option, int &ReplyCode);
};

cPluginV8::cPluginV8(void)
{
	// Initialize any member variables here.
	// DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
	// VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
}

cPluginV8::~cPluginV8()
{
	// Clean up after yourself!
}

const char *cPluginV8::CommandLineHelp(void)
{
	// Return a string that describes all known command line options.
	return NULL;
}

bool cPluginV8::ProcessArgs(int argc, char *argv[])
{
    static struct option opts[] = {
        { "plugin-dir", required_argument, NULL, 'p' },
        { 0 }
    };
    
     int optchar, optind = 0;
    while ((optchar = getopt_long( argc, argv, "p:", opts, &optind)) != -1) {
        switch ( optchar ) {
        case 'p':
            plugin_dir = optarg;
            break;
            
        default:
            return false;
        }
    }
    
    dsyslog("V8: Plugin dir: %s", plugin_dir);
    
    // Implement command line argument processing here if applicable.
    return true;
}

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const v8::String::Utf8Value& value) {
    return *value ? *value : "<string conversion failed>";
}

Handle<Value> Print(const Arguments& args)
{
    bool first = true;
    
    for (int i = 0; i < args.Length(); i++) {
        HandleScope handle_scope;
        
        if (first) {
            first = false;
        } else {
            dsyslog(" ");
        } 
        
        String::Utf8Value str(args[i]); dsyslog("V8: %s", *str);
    }

    return Undefined();
}

bool cPluginV8::Initialize(void)
{
    DIR *dp;
    struct dirent *ep;
    unsigned char isFolder =0x4;

    dp = opendir(plugin_dir);
    if (dp != NULL) {
       while (ep = readdir (dp)) {
           if (ep->d_name[0] == '.') {
               continue;
           }
           
           // check if is dir
           if (ep->d_type == isFolder) {
               dsyslog("V8: Found plugin dir: %s", ep->d_name);
               
               char index_file[strlen(plugin_dir) + strlen("//index.js") + strlen(ep->d_name)];
               
               strcpy(index_file, plugin_dir);
               strcat(index_file, "/");
               strcat(index_file, ep->d_name);
               strcat(index_file, "/index.js");
               
               if (access(index_file, F_OK ) == 0) {
                   dsyslog("V8: Found plugin: %s/index.js", ep->d_name);
               } else {
                   dsyslog("V8: Missing index.js file in plugin dir: %s", ep->d_name);
               }
           } else {
               dsyslog("V8: Found plugin: %s", ep->d_name);
               Plugin v8;
               v8.init(ep->d_name);
           }
       }
       
       (void) closedir (dp);
    } else {
        switch (errno) {
        case EACCES:
            dsyslog("V8: Couldn't open the plugin directory (no write access): %s", strerror(errno));
            break;
        
        case ENOENT:
            dsyslog("V8: Couldn't open the plugin directory (file does not exist): %s", strerror(errno));
            break;

        case EROFS:
            dsyslog("V8: Couldn't open the plugin directory (file is on a readonly medium): %s", strerror(errno));
            break;
            
        default:
            dsyslog("V8: Couldn't open the plugin directory (unknown error): %s", strerror(errno));
            break;
        }
    }
    
    HandleScope handle_scope;
    
    Handle<ObjectTemplate> global = ObjectTemplate::New();
    global->Set(String::New("dsyslog"), FunctionTemplate::New (Print));
    
    Persistent<Context> context = Context::New(NULL, global);
    
    Context::Scope context_scope(context);
    
    Handle<String> source = String::New("dsyslog('Hello' + ', World!')");
    
     // Compile the source code.
    Handle<Script> script = Script::Compile(source);

    // Run the script to get the result.
    Handle<Value> result = script->Run();
    
    context.Dispose();

    //// Convert the result to an ASCII string and print it.
    //String::Utf8Value ascii(result);
    //const char* cstr = ToCString(ascii);

    //dsyslog("v8 started %s", cstr);
    
    return true;
}

bool cPluginV8::Start(void)
{
	// Start any background activities the plugin shall perform.
	return true;
}

void cPluginV8::Stop(void)
{
	// Stop any background activities the plugin is performing.
}

void cPluginV8::Housekeeping(void)
{
	// Perform any cleanup or other regular tasks.
}

void cPluginV8::MainThreadHook(void)
{
	// Perform actions in the context of the main program thread.
	// WARNING: Use with great care - see PLUGINS.html!
}

cString cPluginV8::Active(void)
{
	// Return a message string if shutdown should be postponed
	return NULL;
}

time_t cPluginV8::WakeupTime(void)
{
	// Return custom wakeup time for shutdown script
	return 0;
}

cOsdObject *cPluginV8::MainMenuAction(void)
{
	// Perform the action when selected from the main VDR menu.
	return NULL;
}

cMenuSetupPage *cPluginV8::SetupMenu(void)
{
	// Return a setup menu in case the plugin supports one.
	return NULL;
}

bool cPluginV8::SetupParse(const char *Name, const char *Value)
{
	// Parse your own setup parameters and store their values.
	return false;
}

bool cPluginV8::Service(const char *Id, void *Data)
{
	// Handle custom service requests from other plugins
	return false;
}

const char **cPluginV8::SVDRPHelpPages(void)
{
	// Return help text for SVDRP commands this plugin implements
	return NULL;
}

cString cPluginV8::SVDRPCommand(const char *Command, const char *Option, int &ReplyCode)
{
	// Process SVDRP commands this plugin implements
	return NULL;
}

VDRPLUGINCREATOR(cPluginV8); // Don't touch this!
