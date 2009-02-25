#include "threadcontroller.h"

using namespace srdgame;

volatile unsigned int g_thread_id_count = 0;

unsigned int GenerateThreadId()
{
	return ++g_thread_id_count;
}
