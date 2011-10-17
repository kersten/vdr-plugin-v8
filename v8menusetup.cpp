#include "v8menusetup.h"

cMenuSetupV8::cMenuSetupV8()
{
    newGreetingTime = 3;
    
    Add(new cMenuEditIntItem( tr("V8 Plugins"), &newGreetingTime));
}

void cMenuSetupV8::Store()
{
    
}
