#include "../inc/my_apue.h"

int main(int argc, char * argv[0])
{
	int n = 1;
	while (n)
	{
		sleep(60);
		if (n % 5 == 0)
		{
			time_t t;
			if (time(&t) < 0)
			{
				error("time error");
			}

			struct tm * ptm = NULL;
			ptm = (struct tm *)gmtime(&t);
			printf("%s", asctime(ptm));
		}
		++n;
	}
	return 0;
}
