#include "System/spring.h"
#include "System/particle.h"
#include "System/simulation.h"
#include "System/assemble.hxx"

#include "Render/renderer.h"

#include "igl/readOBJ.h"

// #include <unistd.h>
#include <iostream>
#include <string>
#include <memory>
#include <Eigen/Core>

using namespace std;
using namespace Eigen;

#define CLOTH_WIDTH 15
#define CLOTH_LENGTH 15


int main(int argc, char const *argv[])
{
	shared_ptr<NetSystem> spring_system(new NetSystem);
	Renderer renderer;

	string obj_path;
	cout << "the obj file path: ";
	cin >> obj_path;

	assemble(spring_system, obj_path);
	// assemble(spring_system, CLOTH_WIDTH, CLOTH_LENGTH);

	(spring_system->get_particle_vec())[0]->setFix(true);
	(spring_system->get_particle_vec())[CLOTH_LENGTH*(CLOTH_WIDTH-1)]->setFix(true);
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