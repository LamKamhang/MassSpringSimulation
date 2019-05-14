#include "spring.hxx"
#include "particle.hxx"
#include "simulation.hxx"
#include "renderer.hxx"

#include <unistd.h>
#include <iostream>
#include <memory>
#include <Eigen/Core>

using namespace std;
using namespace Eigen;

int main(int argc, char const *argv[])
{
	shared_ptr<NetSystem> spring_system(new NetSystem);
	spring_system->add_particle({0,0,0}, {0,0,0}, true);
	spring_system->add_particle({1,0,0});
	spring_system->add_particle({2,0,0});
	// spring_system->add_particle({3,0,0});
	// spring_system->add_particle({1,1,1});
	// spring_system->add_particle({4,1,1});
	// spring_system->add_particle({5,1,3});
	// spring_system->add_particle({6,7,8});
	// spring_system->add_particle({7,8,9});

	spring_system->add_spring(0, 1, 1, 0.5);
	spring_system->add_spring(1, 2, 1, 0.5);
	// spring_system->add_spring(2, 3, 1, 0.5);
	// spring_system->add_spring(1, 4, 3, 1);
	// spring_system->add_spring(4, 5, 3, 1);
	// spring_system->add_spring(3, 6, 3, 1);
	// spring_system->add_spring(2, 3, 3, 1);
		
	spring_system->assemble_complete();

	Renderer renderer;
	renderer.AddTimerEvent(spring_system);

	renderer.render();
	// while (true)
	// {
	// 	spring_system->render();
	// 	spring_system->simulate();
	// 	cout << "----------------" << endl << endl;
	// 	sleep(1);
	// }
	
	return 0;
}
