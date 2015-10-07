#ifndef ASYNC_LOGGING_H__
#define ASYNC_LOGGING_H__

#include "Noncopyable.h"
#include <stdint.h>
#include <thread>
#include "Buffer.h"
#include <condition_variable>
#include <chrono>
#include "SyncQueue.h"



namespace Logger
{
	
	class AsyncLogging final : private Noncopyable
	{
	public:
		AsyncLogging();
		~AsyncLogging();

		void Append(const char* msg, uint64_t len);
		void Flush();

	private:
		void LoggingThreadFuncCallBack();

	private:
		std::thread loggingThread_;
		Buffer<kLargeBuffer> currentBuffer_;
		Buffer<kLargeBuffer> nextBuffer_;
		std::atomic<bool> currentBufferFullFlag_{false};
		std::condition_variable bufferFullCond_;
		std::mutex bufferFullMutex_;
		std::mutex mutex_;
		std::chrono::system_clock::time_point lastFlushTime_;
		using LogBufferType = Buffer<kLargeBuffer>;
		using LogQueueType = SyncQueue<LogBufferType>;
		const uint32_t FLUSH_INTERVAL = 3;
		LogQueueType logSyncQueue_;
	};
}



#endif
