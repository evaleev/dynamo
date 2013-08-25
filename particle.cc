
#include <particle.h>

using namespace dynamo;

Particle::Particle() : pimpl_(new ParticleImpl)
{
  pimpl_->m_ = 20.17997/(1000.*6.022e23);
}

Particle::Particle(double M, const Vector3& R, const Vector3& P) :
    pimpl_(new ParticleImpl)
{
  pimpl_->m_ = M;
  pimpl_->r_ = R;
  pimpl_->p_ = P;
}

void
Particle::make_step()
{
  pimpl_->r_ = pimpl_->r_p1_;
  pimpl_->p_ = pimpl_->p_p1_;
}

void
Particle::print(std::ostream& os) const
{
  os << "  Particle:" << std::endl
     << "    m=" << pimpl_->m_
     << " r=(" << pimpl_->r_[0]
     << "," << pimpl_->r_[1]
     << "," << pimpl_->r_[2] << ") p=("
     << pimpl_->p_[0] << ","
     << pimpl_->p_[1] << ","
     << pimpl_->p_[2] << ")" << std::endl;
}
