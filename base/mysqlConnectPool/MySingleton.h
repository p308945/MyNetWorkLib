/*************************************************************************
  > File Name: MySingleton.h
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Fri 26 Jun 2015 11:09:53 PM
 ************************************************************************/
#ifndef __MYSINGLETONH__
#define __MYSINGLETONH__

#include<iostream>
using namespace std;

namespace MyNameSpace
{
	template <typename T>
		class MySingleton
		{
			public:
				static T & getInstance()
				{
					static T instance;
					return instance;
				}
			private:
		};
}

#endif


