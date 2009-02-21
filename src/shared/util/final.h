/**=============================================================================
 * Copyright (C) 2006 Team RioWow
 *
 * File: final.h
 * Description:
        This file is to define a class which subclass is final.
 * Author: Rio Chang
 * Update: 11/03/2006
==============================================================================*/
#ifndef FINAL_H
#define FINAL_H
//------------------------------------------------------------------------------
#ifdef PRIVATE_FINAL_CLASS

#define FINALCLASS( X ) \
class FinalBase$(X)\
{ \
    friend class X; \
private: \
    FinalBase$(X)(){}; \
    virtual ~FinalBase$(X)(){}; \
}; \
class X : virtual private FinalBase$(X)

#endif
//------------------------------------------------------------------------------
#ifdef PROTECTED_FINAL_CLASS

// This can not forbid friend class of the class X.
#define FINALCLASS( X ) \
class FinalBase$(X)\
{ \
protected: \
    FinalBase$(X)(){}; \
    virtual ~FinalBase$(X)(){}; \
}; \
class X : virtual private FinalBase$(X)

#endif
//------------------------------------------------------------------------------
#endif
