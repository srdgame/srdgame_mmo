/**=============================================================================
 * Copyright (C) 2006 Team RioWow
 *
 * File: common.h
 * Description:
        This file is to define and surpport different complier and system.
 * Author: cch
 * Update: 2009-2-21
 * Message:Copy from my RioWow project
==============================================================================*/
#ifndef LOG_H
#define LOG_H

#include "singleton.h"
#include <string>
using namespace std;
//------------------------------------------------------------------------------
class Log : public Singleton<Log>
{
public:
  void out_string(const string& str);
  void out_error(const string& err);
  void out_chat(const string& chat);
};
inline void LogString(const string& str)
{
    Log::get_singleton().out_string(str);
}
inline void LogError(const string& err)
{
    Log::get_singleton().out_error(err);
}
inline void LogChat(const string& chat)
{
    Log::get_singleton().out_chat(chat);
}
#endif
//------------------------------------------------------------------------------
