
#ifndef _dynamo_boltzmann_h_
#define _dynamo_boltzmann_h_

#include <vector>

#include <particle.h>
#include <random.h>

namespace dynamo {
  
  /** Boltzmann distribution */
  class BoltzmannDistribution {
    public:
    /// distribution corresponding to temperature T
    BoltzmannDistribution(double T);
    /// return T
    double T() const;
    
    /// set particle's momentum accoding to this distribution
    const BoltzmannDistribution& operator>>(Particle& p) const;
    
    private:
    double T_;
    Random uniform_random_;
    
    /// generates random Gaussian numbers distributed around 0.0 with variance of 1.0
    double gaussian_random() const;
    
  };
  
  /// Set momenta of the particles according to D
  void operator<<(std::vector<Particle>& particles, const BoltzmannDistribution& D);
  
}

#endif // include guard
