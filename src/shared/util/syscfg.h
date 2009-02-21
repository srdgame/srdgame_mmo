/**============================================================================
 * Copyright (C) 2006 Team RioWow
 *
 * File: syscfg.h
 * Description:
        This file is to define and surpport different system.
 * Author: Rio Chang
 * Update: 10/28/2006
=============================================================================*/
#ifndef SYSCFG_H
#define SYSCFG_H

#define PLATFORM_WIN32 0
#define PLATFORM_UNIX  1
#define PLATFORM_APPLE 2
#define PLATFORM_INTEL 3
//------------------------------------------------------------------------------
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#   define PLATFORM PLATFORM_WIN32
#elif defined( __APPLE_CC__ )
#   define PLATFORM PLATFORM_APPLE
#elif defined( __INTEL_COMPILER )
#   define PLATFORM PLATFORM_INTEL
#else
#   define PLATFORM PLATFORM_UNIX
#endif
//------------------------------------------------------------------------------
#if PLATFORM == PLATFORM_WIN32
#   define STRCASECMP stricmp
#else
#   define STRCASECMP strcasecmp
#endif

#endif
//------------------------------------------------------------------------------
