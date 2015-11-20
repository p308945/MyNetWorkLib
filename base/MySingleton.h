/*************************************************************************
  > File Name: MySingleton.h
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Fri 26 Jun 2015 11:09:53 PM
 ************************************************************************/
#ifndef __MYSINGLETONH__
#define __MYSINGLETONH__

#include <pthread.h>

namespace MyNameSpace
{
	template <typename T>
		class MySingleton
		{
			public:
				static T & getInstance()
				{
					pthread_once(&ponce_, &MySingleton::init);
					return *instance;
				}

			protected:
				MySingleton() {}
				MySingleton(const MySingleton&) {}
			private:
				static void init()
				{
					instance = new T();
				}
			private:
				static pthread_once_t ponce_;
				static T *instance;
		};

	template<typename T>
		pthread_once_t MySingleton<T>::ponce_ = PTHREAD_ONCE_INIT;

	template<typename T>
		T *MySingleton<T>::instance = nullptr;
}

#endif


