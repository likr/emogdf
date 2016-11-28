/** \file
 * \brief Declaration of class FMEThread.
 *
 * \author Martin Gronemann
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * \par
 * Copyright (C)<br>
 * See README.txt in the root directory of the OGDF installation for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation;
 * see the file LICENSE.txt included in the packaging of this file
 * for details.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/

#pragma once

#include <ogdf/basic/Thread.h>
#include <ogdf/basic/Barrier.h>

#include <ogdf/internal/energybased/FastUtils.h>
#include <ogdf/internal/energybased/ArrayGraph.h>
#include <ogdf/internal/energybased/LinearQuadtree.h>

namespace ogdf {

class FMEThreadPool;

/*!
 * The thread task class
 * used only as an interface
*/
class FMETask
{
public:
	virtual ~FMETask() { }

	virtual void doWork() = 0;
};


/*!
 * Class used to invoke a functor or function inside a thread.
*/
template<typename FuncInvokerType>
class FMEFuncInvokerTask : public FMETask
{
public:
	//! constructor with an invoker
	FMEFuncInvokerTask(FuncInvokerType f) : funcInvoker(f) { }

	//! overrides the task doWork() method and invokes the function or functor
	void doWork() {	funcInvoker(); }
private:
	//! the invoker
	FuncInvokerType funcInvoker;
};


/*!
 * The fast multipole embedder work thread class
*/
class FMEThread /*: public Thread*/
{
public:
	//! construtor
	FMEThread(FMEThreadPool* pThreadPool, uint32_t threadNr);

	//! returns the index of the thread ( 0.. numThreads()-1 )
	inline uint32_t threadNr() const { return m_threadNr; }

	//! returns the total number of threads in the pool
	inline uint32_t numThreads() const { return m_numThreads; }

	//! returns true if this is the main thread ( the main thread is always the first thread )
	inline bool isMainThread() const { return (m_threadNr == 0); }

	//! returns the ThreadPool this thread belongs to
	inline FMEThreadPool* threadPool() const { return m_pThreadPool; }

	//! thread sync call
	void sync();

#if defined(OGDF_SYSTEM_UNIX) && defined(OGDF_FME_THREAD_AFFINITY)
	void unixSetAffinity();
#else
	void unixSetAffinity() { }
#endif

	//! the main work function
	void operator()() {
		unixSetAffinity();
		m_pTask->doWork();
		delete m_pTask;
		m_pTask = 0;
	}

	//! sets the actual task
	void setTask(FMETask* pTask) { m_pTask = pTask; }

private:
	uint32_t m_threadNr;

	uint32_t m_numThreads;

	FMEThreadPool* m_pThreadPool;

	FMETask* m_pTask;

	FMEThread(const FMEThread &); // = delete
	FMEThread &operator=(const FMEThread &); // = delete
};


class FMEThreadPool
{
public:
	FMEThreadPool(uint32_t numThreads);

	~FMEThreadPool();

	//! returns the number of threads in this pool
	inline uint32_t numThreads() const { return m_numThreads; }

	//! returns the threadNr-th thread
	inline FMEThread* thread(uint32_t threadNr) const { return m_pThreads[threadNr]; }

	//! returns the barrier instance used to sync the threads during execution
	inline Barrier *syncBarrier() const { return m_pSyncBarrier; }

	//! runs one iteration. This call blocks the main thread
	void runThreads();

	template<typename KernelType, typename ArgType1>
	void runKernel(ArgType1 arg1)
	{
		for (uint32_t i=0; i < numThreads(); i++)
		{
			KernelType kernel(thread(i));
			FuncInvoker<KernelType, ArgType1> invoker(kernel, arg1);
			thread(i)->setTask(new FMEFuncInvokerTask< FuncInvoker< KernelType, ArgType1 > >(invoker));
		}
		runThreads();
	}

private:
	void allocate();

	void deallocate();

	uint32_t m_numThreads;

	FMEThread** m_pThreads;

	Barrier *m_pSyncBarrier;
};

}
