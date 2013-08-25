
#include <cmath>

#include <potential.h>

using namespace dynamo;

Potential_LJ::Potential_LJ(double sigma, double epsilon) :
  sigma2_(sigma*sigma), foure_(epsilon*4.0)
{
  oosigma2_ = 1.0/sigma2_;
}

double
Potential_LJ::evaluate(std::vector<Particle>& particles)
{
  const unsigned int np = particles.size();
  
  // For now just compute total potential energy
  double U = 0.0;
  std::vector<Vector3> F(np);
  
  // loop over all interactions and compute potential and force on each particle
  for(unsigned int i=0; i<np; i++) {
    const Particle& I = particles[i];
    
    double Ui = 0.0;
    
    for(unsigned int j=0; j<i; j++) {
      
      const Particle& J = particles[j];
      const Vector3 Rij = I.r()-J.r();
      
      // These are unitless quantities (sigma/rij)^n
      const double rij_2 = sigma2_/norm2(Rij);
      const double rij_4 = rij_2*rij_2;
      const double rij_6 = rij_2*rij_4;
      const double rij_8 = rij_4*rij_4;
      const double rij_12 = rij_6*rij_6;
      const double rij_14 = rij_6*rij_8;
      
      // 
      Ui += foure_ * (rij_12 - rij_6);
      Vector3 Fij = (6.0*foure_ * oosigma2_) * (2.0 * rij_14 * Rij - rij_8 * Rij);
      F[i] += Fij;
      F[j] -= Fij;
      
    }
    
    U += Ui;
  }
  
  // set forces on particles
  for(unsigned int i=0; i<np; i++)
    particles[i].f() = F[i];
  
  return U;
}

