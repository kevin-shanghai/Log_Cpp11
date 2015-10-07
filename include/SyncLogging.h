#ifndef  SYNC_LOGGING_H__
#define SYNC_LOGGING_H__
#include "Noncopyable.h"
#include "FileOperation.h"
#include <memory>
#include <mutex>




namespace Logger
{
	class SyncLogging final : private Noncopyable
	{
	public:
		SyncLogging();
		~SyncLogging();

		void Append(const char* msg, uint64_t len);
		void Flush();

	private:
		void CreateLogFileDynamiclly();

	private:
		std::unique_ptr<FileOperation> fileOperation_;
		std::mutex ioMutex_;
		const uint64_t logFileRollSize_ = 10 * 1024 * 1024;
	};
}


#endif