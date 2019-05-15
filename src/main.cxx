#include "System/spring.h"
#include "System/particle.h"
#include "System/simulation.h"

#include "Render/renderer.h"

#include <unistd.h>
#include <iostream>
#include <memory>
#include <Eigen/Core>

using namespace std;
using namespace Eigen;

#define CLOTH_WIDTH 12
#define CLOTH_LENGTH 12

void assemble(const shared_ptr<NetSystem> &system);

int main(int argc, char const *argv[])
{
	shared_ptr<NetSystem> spring_system(new NetSystem);
	Renderer renderer;

	assemble(spring_system);
	spring_system->assemble_complete();

	renderer.AddTimerEvent(spring_system);

	renderer.render();
	// while (true)
	// {
	// 	spring_system->render();
	// 	spring_system->simulate();
	// 	sleep(1);
	// }
	
	return 0;
}

void assemble(const shared_ptr<NetSystem> &system)
{
	for (unsigned i = 0; i < CLOTH_WIDTH; ++i)
		for (unsigned j = 0; j < CLOTH_LENGTH; ++j)
			if (j == 0 && (i == CLOTH_LENGTH-1 || i == 0))
				system->add_particle({i*1.0, 10, j*1.0}, true);
			else
				system->add_particle({i*1.0, 10, j*1.0});

	for (unsigned i = 0; i < CLOTH_WIDTH; ++i)
		for (unsigned j = 0; j < CLOTH_LENGTH-1; ++j)
			system->add_spring(CLOTH_LENGTH*i+j, CLOTH_LENGTH*i+j+1, 20, 1);

	for (unsigned i = 0; i < CLOTH_WIDTH-1; ++i)
		for (unsigned j = 0; j < CLOTH_LENGTH; ++j)
			system->add_spring(CLOTH_LENGTH*i+j, CLOTH_LENGTH*(1+i)+j, 20, 1);

	for (unsigned i = 0; i < CLOTH_WIDTH-1; ++i)
		for (unsigned j = 0; j < CLOTH_LENGTH-1; ++j)
			system->add_spring(CLOTH_LENGTH*i+j, CLOTH_LENGTH*(1+i)+j+1, 20, sqrt(2));
}