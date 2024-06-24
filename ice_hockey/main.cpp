#include "IceHockey.h"

int main()
{
	IceHockey demo;
	if (demo.Construct(300, 200, 2, 2))
		demo.Start();
	return 0;
}