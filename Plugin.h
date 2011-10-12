/* 
 * File:   Plugin.h
 * Author: kersten
 *
 * Created on 12. Oktober 2011, 11:53
 */

#ifndef PLUGIN_H
#define	PLUGIN_H

#include <vdr/tools.h>

class Plugin {
public:
    Plugin();
    ~Plugin();
    
    void init(char* source_file);
private:
    
};

#endif	/* PLUGIN_H */

