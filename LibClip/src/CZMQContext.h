#pragma  once

#include "czmq.h"

/**
 \class	CZMQContext

 \brief	Provides a singleton for accessing the current CZMQ context
	
 \note  This class is not thread safe. Therefore the first call getContext should occur before 
		other threads are created.

 \author	Jared
 \date	4/12/2013
 */

class CZMQContext
{
private:
	static zctx_t* m_context;
	CZMQContext();
public:
	static zctx_t* getContext()
	{
		if (!m_context)
			m_context = zctx_new();

		return m_context;
	}
	
};