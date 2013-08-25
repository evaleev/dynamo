
#include <propagator.h>

namespace dynamo {
  
  void
  propagate(Particle& part, double timestep, Box& box)
  {
    // dr = p * dt/m
    //double normstep = timestep / part.m();
    //Vector3 dr = normstep*part.p();
    // r(new) = r(old) + dr
    //Vector3 r_new = part.r() + dr;
    // make sure the particle is still inside the box
    box.contain(part,r_new);
  }
  
}
