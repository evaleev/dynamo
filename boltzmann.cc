
#include <stdexcept>
#include <cmath>
#include <iostream>

#include <boltzmann.h>

using namespace dynamo;

BoltzmannDistribution::BoltzmannDistribution(double T) : T_(T), uniform_random_(0.0,1.0)
{
  if (T_ < 0.0) throw std::runtime_error("BoltzmannDistribution::BoltzmannDistribution -- temperature must be nonnegative");
}

double
BoltzmannDistribution::T() const
{
  return T_;
}

const BoltzmannDistribution&
BoltzmannDistribution::operator>>(Particle& part) const
{
// kb in units of kg Angstom**2/ps**2 K
  const double mks2Akps=pow((1.0e10/1.0e12),2);
  const double k = 1.38065e-23*mks2Akps;
  const double stddev = sqrt(k * T_* part.m());
//  std::cout << "stddev " << stddev << " mass "<< part.m() << " product "<< stddev*part.m() <<std::endl;
  double px = stddev * gaussian_random();
  double py = stddev * gaussian_random();
  double pz = stddev * gaussian_random();
  Vector3 P(px,py,pz);
  part.p() = P;
  return *this;
}

double
BoltzmannDistribution::gaussian_random() const
{
  static bool need_new_pair = true;
  static double y1;
  static double y2;
  
  if (need_new_pair) {
    double x1, x2, w;
    do {
      x1 = 2.0 * uniform_random_() - 1.0;
      x2 = 2.0 * uniform_random_() - 1.0;
      w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );
    w = sqrt( (-2.0 * log( w ) ) / w );
    y1 = x1 * w;
    y2 = x2 * w;
    need_new_pair = false;
    return y1;
  }
  else {
    need_new_pair = true;
    return y2;
  }
}

void
dynamo::operator<<(std::vector<Particle>& particles, const BoltzmannDistribution& D)
{
  const unsigned int np = particles.size();
  const double mks2Akps=pow((1.0e10/1.0e12),2);
  const double kb = 1.38065e-23*mks2Akps;
  typedef std::vector<Particle>::const_iterator citer;
  typedef std::vector<Particle>::iterator iter;
  const citer end = particles.end();
  
  for(iter i=particles.begin(); i!=end; i++)
    D >> *i;
  
  // compute total energy
  double total_energy = 0.0;
  for(iter i=particles.begin(); i!=end; i++) {
    const double P = norm(i->p());
    total_energy += P*P/(2.0*i->m());
  }
  const double average_energy = total_energy / np;
//  std::cout << "Average energy per particle (J)= "
//            << average_energy/mks2Akps << std::endl;
//  std::cout << "Should be (J) = " << 1.5* kb/mks2Akps * D.T() << std::endl;
//  std::cout << "Instantaneous Temp (K) " << 2.0*average_energy/(3.0*kb)
//  << std::endl;

  // Scale momenta so that average energy per particle is exactly 3kT/2
  const double scale = (1.5 * kb*D.T())/average_energy;
  for(iter i=particles.begin(); i!=end; i++)
    i->p().scale(scale);
}

