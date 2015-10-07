#ifndef SYNC_QUEUE_H__
#define SYNC_QUEUE_H__
#include <deque>
#include <condition_variable>
#include <mutex>
#include <atomic>


namespace Logger
{
	template <class T, uint64_t QUEUE_SIZE = 1000>
	class SyncQueue
	{
	public:
		SyncQueue() : queue_()
		{

		}

		void Put(T&& value)
		{
			std::unique_lock<std::mutex> lk(mutex_);
			while (Full())
			{
				notFullCond_.wait(lk);
			}

			queue_.emplace_back(std::move(value));
			notEmptyCond_.notify_one();
		}

		void Put(T value)
		{
			std::unique_lock<std::mutex> lk(mutex_);
			while (Full())
			{
				queue_.emplace_back(value);
			}
			queue_.emplace_back(value);
			notEmptyCond_.notify_one();
		}

		T Take()
		{
			std::unique_lock<std::mutex> lk(mutex_);
			while (Empty())
			{
				notEmptyCond_.wait(lk);
			}
			T frontValue(std::move(queue_.front()));   //T must have move constructor
			queue_.pop_front();
			return frontValue;
		}

		uint64_t Size() const
		{
			return queue_.size();
		}


		bool Empty() const
		{
			///std::unique_lock<std::mutex> lk(mutex_);
			bool empty = queue_.empty();
			return empty;
		}

	private:
		bool Full()	const
		{
			//std::unique_lock<std::mutex> lk(mutex_);
			bool full = (queue_.size() >= QUEUE_SIZE);
			return full;
		}


	private:
		std::condition_variable notEmptyCond_;
		std::condition_variable notFullCond_;
		mutable std::mutex mutex_;
		std::deque<T> queue_;
	};
}


#endif