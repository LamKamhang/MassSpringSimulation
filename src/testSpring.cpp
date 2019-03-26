#include <iostream>

#include "spring.hpp"
#include "particle.hpp"

using namespace std;
int main(int argc, char const *argv[])
{
    Spring myspring(5, 4);
    std::shared_ptr<Particle> m1(new Particle(2));
    std::shared_ptr<Particle> m2(new Particle(4, true, {3, 0, 0}));

    cout << *m1 << endl;
    cout << *m2 << endl;

    myspring.bindLeft(m1);
    myspring.bindRight(m2);
    myspring.update();

    cout << *m1 << endl;
    cout << *m2 << endl;
    return 0;
}
