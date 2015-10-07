#ifndef SINGLETON_H__
#define SINGLETON_H__

#include <mutex>


template<class T>
class Singleton
{
public:
	Singleton()
	{

	}
	template <typename... Args>
	static T* GetInstance(Args&&... args)
	{
		if (m_pInstance == nullptr)  //double check
		{
			static std::mutex m_Mutex;
			std::lock_guard<std::mutex> lock(m_Mutex);
			if (m_pInstance == nullptr)
			{
				m_pInstance = new T(std::forward<Args>(args)...);
			}
		}
		return m_pInstance;
	}
private:
	static T* m_pInstance;
};

template<class T>
T* Singleton<T>::m_pInstance = nullptr;


#endif