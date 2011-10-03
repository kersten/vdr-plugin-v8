/*
 * v8.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include <vdr/plugin.h>
#include <v8.h>

static const char *VERSION = "0.0.1";
static const char *DESCRIPTION = "VDR plugins written in JavaScript";
static const char *MAINMENUENTRY = "V8";

class cPluginV8 : public cPlugin {
private:
	// Add any member variables or functions you may need here.
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
	// Implement command line argument processing here if applicable.
	return true;
}

bool cPluginV8::Initialize(void)
{
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
