#include "ZThreaded.h"
#include "Logger.h"


ZThread::ZThread(zctx_t* ctx)
	:m_ctx(ctx), m_pipeToThread (NULL)
{
}


ZThread::~ZThread(void)
{
}

/**
 \fn	void* ZThread::run()

 \brief	Spawns a new thread and returns a pipe to the thread.
		
		This function checks if the thread is readable before returning pipe

 \return	Null if fails, else pipe to thread
 */

void* ZThread::run()
{
	LOG()<< "Spawning new ZThread" << std::endl;

	m_pipeToThread = zthread_fork (m_ctx,  runFunc, this);

	if (m_pipeToThread )
	{
		char *status = zstr_recv (m_pipeToThread);
		if (strneq (status, "OK"))
		{
			terminate();
		}
		zstr_free (&status);
	}

	return m_pipeToThread ;
}

void ZThread::runFunc(void *args, zctx_t *ctx, void *pipe)
{
	ZThread* thread = (ZThread*) args;

	thread->eventLoop(pipe);
}

void ZThread::eventLoop(void* pipe)
{
	LOG_WARN() << "Empty ZThread::eventLoop" << std::endl;
}
/**
 \fn	void terminate()

 \brief	Prompts a thread to terminate
 */
void ZThread::terminate()
{
	LOG() << "Terminating thread" << std::endl;

	zstr_send (m_pipeToThread , "TERMINATE");
	char *reply = zstr_recv (m_pipeToThread );
	zstr_free (&reply);
	m_pipeToThread = NULL;
}
