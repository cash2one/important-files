#ifndef __BOUNDED_QUEUE_HPP
#define __BOUNDED_QUEUE_HPP


#include "../MultiThread/Lock.hpp"
#include <queue>

namespace async
{
	namespace container
	{
		template< typename T, typename A = std::allocator<T> >
		class BoundedBlockingQueue
		{
			typedef async::thread::AutoCriticalSection	Mutex;
			typedef async::thread::AutoLock<Mutex>		AutoLock;
			typedef async::thread::SemaphoreCondition	Condtion;
			typedef std::deque<T, A>					Container;

			Mutex mutex_;
			Condtion notEmpty_;
			Condtion notFull_;
			Container queue_;

			size_t maxSize_;

		public:
			explicit BoundedBlockingQueue(size_t maxSize)
				: maxSize(maxSize_)
			{} 
			BlockingQueue(size_t maxSize, A &allocator)
				: maxSize(maxSize_)
				, queue_(allocator)
			{}

		private:
			BoundedBlockingQueue(const BoundedBlockingQueue &);
			BoundedBlockingQueue &operator=(const BoundedBlockingQueue &);

		public:
			void Put(const T &x)
			{
				{
					AutoLock lock(mutex_);
					while(queue_.size() == maxSize_ )
					{
						notFull_.Wait(mutex_);
					}

					assert(queue_.size() != maxSize_);
					queue_.push(x);
				}

				notEmpty_.Signal();
			}

			T Get()
			{
				T front;
				{
					AutoLock lock(mutex_);
					while(queue_.empty())
					{
						notEmpty_.Wait();
					}
					assert(!queue_.empty());
					front = queue_.front();
					queue_.pop();
				}

				notFull_.Signal();
				return front;
			}

			bool Empty() const
			{
				AutoLock lock(mutex_);
				return queue_.empty();
			}

			bool Full() const
			{
				AutoLock lock(mutex_);
				return queue_.size() == maxSize_;
			}

			size_t Size() const
			{
				AutoLock lock(mutex_);
				return queue_.size();
			}
		};
	}
	

}

#endif  

