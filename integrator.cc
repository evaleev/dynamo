
#include <integrator.h>

using namespace dynamo;

Integrator::Integrator(double timestep) : ts_(timestep)
{
}

Integrator::~Integrator()
{
}

void
Integrator::step(Particle& p)
{
  // dr = p * dt/m + 0.5 * F * dt * dt/m
  double normstep = ts_ / p.m();
  Vector3 dr = normstep * (p.p() + (0.5*ts_)*p.f());
  // r(new) = r(old) + dr
  p.r_p1() = p.r() + dr;
  // dp = F * dt
  Vector3 dp = ts_ * p.f();
  // p(new) = p(old) + dp
  p.p_p1() = p.p() + dp;
}
