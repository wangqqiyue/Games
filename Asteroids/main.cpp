#ifndef INCLUDE_ASTEROID
#define INCLUDE_ASTEROID
#include "Asteroid.h"
#endif 

int main() {
	Asteroid asteroid;
	if (asteroid.Construct(256, 200, 2, 2)) {
		asteroid.Start();
	}
}