
#ifndef _dynamo_propagator_h_
#define _dynamo_propagator_h_

#include <particle.h>
#include <box.h>

namespace dynamo {
  
  void propagate(Particle& part, double timestep, Box& box);
  
};

#endif // include guard
