#include <unistd.h>
#include <signal.h>
#include "error.h"
#include "sync.h"

static volatile sig_atomic_t sigflag; /* set nozero by sig handler */
static sigset_t newmask, oldmask, zeromask;

static void sig_usr(int signo) /* one signal handler for SIGUSR1 and SIGUSR2 */
{
	sigflag = 1;
}

void TELL_WAIT(void)
{
	if (signal (SIGUSR1, sig_usr) == SIG_ERR)
	{
		error("signal (SIGUSR1) error");
	}
	
	if (signal (SIGUSR2, sig_usr) == SIG_ERR)
	{
		error("signal (SIGUSR2) error");
	}

	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
	sigaddset(&newmask, SIGUSR2);

	/*
	 * Block SIGUSR1 and SIGUSR2, and save current signal mask.
	 */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		error("SIG_BLOCK error");
}

void TELL_PARENT(pid_t pid)
{
	kill(pid, SIGUSR2);
}

void WAIT_PARENT(void)
{
	while (sigflag == 0)
	{
		sigsuspend(&zeromask); /* and wait for parent */
	}
	/* Now we got the signal */
	sigflag = 0;

	/*
	 *  Reset signal mask to original value.
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
	{
		error("SIG_SETMASK error");
	}
}

void TELL_CHILD(pid_t pid)
{
	kill(pid, SIGUSR1);
}

void WAIT_CHILD(void)
{
	while (sigflag == 0)
	{
		sigsuspend(&zeromask);
	}
	sigflag = 0;

	/*
	 * Reset signal mask to original value.
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
	{
		error("SIG_SETMASK error");
	}
}
