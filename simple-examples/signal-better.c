// Scott Kuhl
//
// Use sigaction() instead of signal()! Even "man 2 signal" says at
// the top of the file (in bold) that signal() should be avoided and
// that sigaction() should be used instead.
//
// This example is here for informational purposes only. If you are
// writing production code, see sigaction.c for a better example.

#define _GNU_SOURCE
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static volatile sig_atomic_t unprocessedSig = 0;

void sighandler(int signo)
{
	if(signo == SIGINT)
		unprocessedSig=1;

	/* This signal handler does not call any functions. If we did call
	 * a function in this handler, we should only call
	 * async-signal-safe functions as described in "man 7 signal". The
	 * manual clearly states: "If a signal interrupts the execution of
	 * an unsafe function, and handler calls an unsafe function, then
	 * the behavior of the program is undefined." */

	/* Another important note: A signal can occur when another signal
	 * is still being processed! */
}

int main(void)
{
	// Note: From "man signal" page: "Using signal() in a
	// multithreaded process is unspecified" (i.e., anything could
	// happen, including a crash).

	if(signal(SIGINT,  sighandler) == SIG_ERR) // interrupt, ctrl+c
		printf("Failed to set up one or more signal handlers\n");
	
	printf("IMPORTANT: Kill this process by running 'killall -9 signal-better' in another terminal window.\n");

	while(1)
	{
		if(unprocessedSig)
		{
			unprocessedSig = 0;
			printf("SIGINT signal occurred.\n");
		}
	}

	return 0;
}
