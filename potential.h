
#ifndef dynamo_potential_h_
#define dynamo_potential_h_

#include <vector>

#include <particle.h>
#include <vector3.h>

namespace dynamo {
  
  /** Lennard-Jones potential */
  class Potential_LJ {
    public:
    /// Characterized by 2 parameters
    Potential_LJ(double sigma, double epsilon);
    /// evaluate potential and force on each particle. Return total potential energy
    double evaluate(std::vector<Particle>& particles);
    
    private:
    // params
    double sigma2_;   // sigma^2
    double foure_;  // 4.0 * epsilon
    // useful to precompute these quantities
    double oosigma2_; // 1/sigma^2
    
  };
}

#endif

