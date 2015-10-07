#include "SyncLogging.h"
#include <chrono>
#include <iomanip> //for std::put_time
#include "FileOperation.h"

namespace Logger
{
	namespace details
	{
		std::string GetCurrentLocalTime()
		{
			time_t rawTime = std::time(&rawTime);
			tm* timeInfo = std::localtime(&rawTime);
			char buffer[25] = { '\0' };
			strftime(buffer, 25, "%Y-%m-%d %H-%M-%S", timeInfo);
			return buffer;
		}

		std::string GetLogFileName()
		{
			return GetCurrentLocalTime() + ".log";
		}
	}

	SyncLogging::SyncLogging()
	{
		CreateLogFileDynamiclly();
	}

	SyncLogging::~SyncLogging()
	{

	}

	void SyncLogging::Append(const char* msg, uint64_t len)
	{
		std::lock_guard<std::mutex> lk(ioMutex_);
		if (fileOperation_->GetWrittenBytes() >= logFileRollSize_)
		{
			fileOperation_->FlushLogFile();
			CreateLogFileDynamiclly();
		}
		fileOperation_->AppendLogFile(msg, len);
	}

	void SyncLogging::Flush()
	{
		std::lock_guard<std::mutex> lk(ioMutex_);
		fileOperation_->FlushLogFile();
	}

	void SyncLogging::CreateLogFileDynamiclly()
	{
		fileOperation_.reset(new FileOperation(details::GetLogFileName().c_str()));
	}
}