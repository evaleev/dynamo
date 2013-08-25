
#include <ostream>
#include <vector3.h>
#include <particle.h>

#ifndef _dynamo_integrator_h_
#define _dynamo_integrator_h_

namespace dynamo {
  
  /** Integrates Newton's equation of motion using contant step */
  class Integrator {
    public:
    Integrator(double timestep);
    ~Integrator();
    
    /// computes particle p's new positions and momenta
    void step(Particle& p);
    
    private:
    /// timestep
    double ts_;
  };

}

#endif // end of include guard

