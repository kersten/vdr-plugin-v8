/* 
 * File:   v8menusetup.h
 * Author: kersten
 *
 * Created on 17. Oktober 2011, 11:31
 */

#ifndef V8MENUSETUP_H
#define	V8MENUSETUP_H

#include <vdr/menuitems.h>

class cMenuSetupV8 : public cMenuSetupPage {
private:
    int newGreetingTime;
    int newUseAlternateGreeting;
protected:
    virtual void Store(void);
public:
    cMenuSetupV8(void);
};


#endif	/* V8MENUSETUP_H */

