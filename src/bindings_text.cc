/**
 * bindings_text.cc - Bindings for text-display.
 *
 * This file is part of lumail: http://lumail.org/
 *
 * Copyright (c) 2013 by Steve Kemp.  All rights reserved.
 *
 **
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 dated June, 1991, or (at your
 * option) any later version.
 *
 * On Debian GNU/Linux systems, the complete text of version 2 of the GNU
 * General Public License can be found in `/usr/share/common-licenses/GPL-2'
 *
 */


#include <algorithm>
#include <cursesw.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>



#include "bindings.h"
#include "debug.h"
#include "global.h"
#include "input.h"

#include "maildir.h"
#include "screen.h"
#include "utfstring.h"


/**
 * Scroll the text down.
 */
int scroll_text_down(lua_State *L)
{
    int step = lua_tonumber(L, -1);

    CGlobal *global = CGlobal::Instance();
    int cur = global->get_text_offset();

    cur += step;

    global->set_text_offset(cur);
    return (0);
}

/**
 * Scroll the text up.
 */
int scroll_text_up(lua_State *L)
{
    int step = lua_tonumber(L, -1);
    CGlobal *global = CGlobal::Instance();


    int cur = global->get_text_offset();
    cur -= step;

    if ( cur < 0 )
        cur = 0;

    global->set_text_offset(cur);
    return (0);
}


/**
 * View the contents of the given file.
 */
int show_file_contents(lua_State *L)
{
    const char *file = lua_tostring(L, -1);
    FILE* f = fopen(file, "r");
    std::vector<UTFString> buf;

    char buffer[16384] = { '\0' };
    while(!feof(f))
    {
        if(fgets(buffer, sizeof(buffer)-1, f) != NULL)
            buf.push_back( buffer );
    }
    fclose(f);

    CGlobal *global = CGlobal::Instance();
    global->set_text( buf );
    return 0;
}


/**
 * View the specified text.
 */
int show_text(lua_State *L)
{

    luaL_checktype(L, -1, LUA_TTABLE);

    int len = lua_objlen(L, -1);
    std::vector<UTFString> buf;


    for(int i = 0; i < len; i++)
    {
        lua_pushinteger(L, i+1);
        lua_gettable(L, -2);
        if(lua_isstring(L, -1))
        {
            buf.push_back( lua_tostring(L, -1) );
        }
        else
        {
            assert(false);
        }
        lua_pop(L, 1);
    }


    CGlobal *global = CGlobal::Instance();
    global->set_text( buf );
    return 0;
}