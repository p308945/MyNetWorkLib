#ifndef _MYTIMEH__
#define _MYTIMEH__

#include <time.h>

namespace MyNameSpace
{
	class MyTime
	{
		public:
			MyTime()
			{
				time(&start);
			}
			time_t elapse()
			{
				time_t now;
				time(&now);
				return now - start;
			}
			void now()
			{
				time(&start);
			}
		private:
			time_t start;
	};
}
#endif

