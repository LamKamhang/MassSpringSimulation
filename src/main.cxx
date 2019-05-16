#include "System/spring.h"
#include "System/particle.h"
#include "System/simulation.h"

#include "Render/renderer.h"

#include <unistd.h>
#include <iostream>
#include <memory>
#include <Eigen/Core>
#include "System/assemble.hxx"
#include "igl/readOBJ.h"

using namespace std;
using namespace Eigen;

#define CLOTH_WIDTH 12
#define CLOTH_LENGTH 12


int main(int argc, char const *argv[])
{
	shared_ptr<NetSystem> spring_system(new NetSystem);
	Renderer renderer;

	assemble(spring_system, "resource/mini_cloth.obj");
	// assemble(spring_system, CLOTH_WIDTH, CLOTH_LENGTH);

	(spring_system->get_particle_vec())[0]->setFix(true);
	(spring_system->get_particle_vec())[CLOTH_LENGTH*(CLOTH_LENGTH-1)]->setFix(true);
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