#pragma once

#include "czmq.h"
#include "Logger.h"

/**
 \class	ZThread

 \brief	Spawns a thread using czmq. Provides an inproc pipe to the spawned thread
 */
class ZThread
{
public:
	ZThread(zctx_t *ctx);
	~ZThread(void);

	void* run();
	void terminate();

	virtual void eventLoop(void *pipe);

private:
	static void runFunc(void *args, zctx_t *ctx, void *pipe);
	void* m_pipeToThread;
	zctx_t* m_ctx;
	DECLARE_LOGGER();
};

