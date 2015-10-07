#include "AsyncLogging.h"
#include "SyncLogging.h"
#include <functional>
#include <chrono>

namespace Logger
{
	AsyncLogging::AsyncLogging()
		: loggingThread_(std::bind(&AsyncLogging::LoggingThreadFuncCallBack, this), "AsyncLoggingThread")
		, bufferFullMutex_()
	{

	}

	AsyncLogging::~AsyncLogging()
	{

	}

	void AsyncLogging::Append(const char* msg, uint64_t len)
	{
		std::unique_lock<std::mutex> lk(mutex_);
		if (len < currentBuffer_.AvailableSize())
		{
			currentBuffer_.Append(msg, len); 
		}

		//current buffer is full, so put current buffer to the sync queue
		else	
		{
			logSyncQueue_.Put(currentBuffer_);
			currentBuffer_.Clear();
			bufferFullCond_.notify_one();
		}
	}

	void AsyncLogging::Flush()
	{

	}

	void AsyncLogging::LoggingThreadFuncCallBack()
	{
		std::unique_ptr<SyncLogging> syncLogging(new SyncLogging);
		while (true)
		{
			// if there are no log msg in the sync queue, this thread will pending atomaticlly
			// so, no need to delay manually here
			std::unique_lock<std::mutex> lk(bufferFullMutex_);
			if (logSyncQueue_.Empty())
			{
				while (bufferFullCond_.wait_for(lk, std::chrono::seconds(3)) == std::cv_status::timeout)
				{
					if (currentBuffer_.Length() != 0)
					{
						std::unique_lock<std::mutex> lk(mutex_);
						syncLogging->Append(currentBuffer_.GetBufferData(), currentBuffer_.Length());
						syncLogging->Flush();
						currentBuffer_.Clear();
					}
				}
			}
			LogBufferType tmpBuffer(logSyncQueue_.Take());	 
			syncLogging->Append(tmpBuffer.GetBufferData(), tmpBuffer.Length());
		}
	}
}
