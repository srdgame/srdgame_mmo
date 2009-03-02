/**============================================================================
 * Copyright (C) 2009 SrdGame Team
 *
 * File: singleton.h
 * Description:
        This file is to define and surpport different complier and system.
 * Author: cch
 * Update: 2009-2-21
 * Verison: 0.0.2
 * Message: I copy this from my orginal project:RioWow, and adjust the coding
 * style
=============================================================================*/
#ifndef SINGLETON_H
#define SINGLETON_H

namespace srdgame
{

template <class T>
class Singleton
{
public:
    static T* get_instance()
    {
        return &(get_singleton());
    }
    static T& get_singleton()
    {
        static T instance;
        return instance;
    }
    ~Singleton(){};
protected:
    Singleton(const Singleton& sig);
    Singleton& operator = (const Singleton& sig);
    Singleton(){};
}
;
}
#endif
//------------------------------------------------------------------------------
