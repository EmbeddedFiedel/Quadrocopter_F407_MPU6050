// Newlib stubs implementation

#include <sys/types.h>
#include <stdlib.h>

pid_t _getpid()
{
	return 0;
}

void _exit( int status )
{
	(void)status;
	while( 1 );
}

int _kill( int pid, int sig )
{
	(void)pid; (void)sig;
	return -1;
}

int _open_r(void *reent, const char *file, int flags, int mode)
{
	(void)reent; (void)file; (void)flags; (void)mode;
	return -1;
}

void * _sbrk(
             struct _reent *_s_r,
             ptrdiff_t nbytes)
{

}

void *__dso_handle = NULL;
