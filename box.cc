
#include <iostream>
#include <cmath>
#include <stdexcept>

#include <box.h>
#include <random.h>

using namespace dynamo;

Box::Box(double Lx, double Ly, double Lz) : nhits_(0), ptrans_(0.0)
{
  L_[0] = Lx;  L_[1] = Ly; L_[2] = Lz;
}

Box::~Box()
{
}

unsigned long
Box::nhits() const
{
  return nhits_;
}

double
Box::ptrans() const
{
  return ptrans_;
}

void
Box::reset()
{
  nhits_ = 0u;
  ptrans_ = 0.0;
}

const Box&
Box::operator<<(Particle& part) const
{
  static Random random_x(0.0,L_[0]);
  static Random random_y(0.0,L_[1]);
  static Random random_z(0.0,L_[2]);
  // Generate random positions
  double x = random_x();
  double y = random_y();
  double z = random_z();
  part.r() = Vector3(x,y,z);
  return *this;
}

const Box&
Box::operator<<(std::vector<Particle>& pv) const
{
  typedef std::vector<Particle> vtype;
  typedef vtype::const_iterator citer;
  typedef vtype::iterator iter;
  
  const citer end = pv.end();
  // Add each particle to the box
  for(iter i=pv.begin(); i!=end; i++) {
    *this << *i;
  }
  return *this;
}

void
Box::contain(Particle& p)
{
  unsigned int nreflections = 0;   // number of reflections
  double ptrans = 0.0;             // transferred momentum
  Vector3 rfinal;
  Vector3 pfinal;
  
  // For each coordinate
  for(int xyz=0; xyz<3; xyz++) {

    // skip this coordinate if inside the box
    double r1 = p.r_p1()[xyz];
    double p1 = p.p_p1()[xyz];
    if ( r1>= 0.0 && r1<=L_[xyz]) {
      rfinal[xyz] = r1;
      pfinal[xyz] = p1;
      continue;
    }
    pfinal[xyz] = -p1;
    ++nreflections;
    ptrans += 2.0*std::fabs(p1);
    
    // else bounce from the wall
    const double dr = r1 - p.r()[xyz];
    if (dr > 0.0) {
      rfinal[xyz] = 2*L_[xyz] - r1;
    }
    else {
      rfinal[xyz] =  - r1;
    }
    if (rfinal[xyz] < 0.0 || rfinal[xyz] > L_[xyz])
      throw std::runtime_error("Timestep is too large");
  }
  
  if (nreflections > 0) {
    p.r_p1() = rfinal;
    p.p_p1() = pfinal;
    nhits_ += nreflections;
    ptrans_ += ptrans;
  }
  
}

