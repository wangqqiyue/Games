#include "IceHockey.h"

int main()
{
	IceHockey demo;
	if (demo.Construct(1000, 700, 1, 1))
		demo.Start();
	return 0;
}