#include "spring.hxx"
#include "particle.hxx"
#include "simulation.hxx"

#include <unistd.h>
#include <iostream>
#include <Eigen/Core>

using namespace std;
using namespace Eigen;

int main(int argc, char const *argv[])
{
	NetSystem spring_system;
	spring_system.add_particle({0,0,0});
	spring_system.add_particle({1,0,0});
	spring_system.add_particle({0,1,0});
	spring_system.add_particle({0,0,1});
	spring_system.add_particle({1,1,0});

	spring_system.add_spring(0, 1, 1, 0.5);
	spring_system.add_spring(1, 2, 1, 0.5);
	spring_system.add_spring(2, 3, 1, 0.5);
	spring_system.add_spring(3, 4, 1, 0.5);
	
	spring_system.assemble_complete();

	while (true)
	{
		spring_system.render();
		spring_system.simulate();
		cout << "----------------" << endl << endl;
		// sleep(1);
	}
	
	return 0;
}
