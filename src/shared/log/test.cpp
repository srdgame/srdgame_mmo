#include "log.h"
#include "logfile.h"

int main ()
{
	LogFile file("test.log");
	LOG.bind(&file);
}
