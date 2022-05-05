#include <cstdio>
#include "Classes.h"
using namespace N;

//increase screen buffer size. 3000 by 2000 works for me.

int main()
{
    Floor f(300, 0, 300);
    Sphere Sa(250, 100, 50, 25);
    Sphere Sb(120, 110, 80, 20);
    Sphere Sc(80, 50, 40, 30);

    VPO objects = {&f, &Sa, &Sb, &Sc};
    Rayscanner r(150, 75, -300, objects);

    r.scan();

    //st::getchar();
    return 0;
}