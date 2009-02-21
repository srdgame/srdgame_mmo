/**=============================================================================
 * Copyright (C) 2006 Team RioWow
 *
 * File: common.h
 * Description:
        This file is to define and surpport different complier and system.
 * Author: cch
 * Update: 2009-2-21
 * Message: Copied from my RioWow project
==============================================================================*/
#include "log.h"
#include <iostream>
using namespace std;
//------------------------------------------------------------------------------
void Log::out_string(const string& str)
{
    cout << str;
}
//------------------------------------------------------------------------------
void Log::out_error(const string& err)
{
    cerr << err;
}
//------------------------------------------------------------------------------
void Log::out_chat(const string& chat)
{
    cout << chat;
}

