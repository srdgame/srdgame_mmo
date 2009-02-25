#include "mutex.h"

using namespace srdgame;

bool Mutex::s_attr_initialized = false;
pthread_mutexattr_t Mutex::s_attr;

Mutex::Mutex()
{
	if (!s_attr_initialized)
	{
		pthread_mutexattr_init(&s_attr);
		pthread_mutexattr_settype(&s_attr, PTHREAD_MUTEX_RECURSIVE_NP);
		s_attr_initialized = true;
	}

	pthread_mutex_init(&_mutex, &s_attr);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&_mutex);
}
