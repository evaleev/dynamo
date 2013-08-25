
#ifndef _dynamo_particle_h_
#define _dynamo_particle_h_

#include <ostream>
#include <vector3.h>
#include <memory>

namespace dynamo {

  struct ParticleImpl {
    /// mass
    double m_;
    /// Current position
    Vector3 r_;
    /// Current momentum
    Vector3 p_;
    /// Force on the particle
    Vector3 f_;
    /// Projected position at next timestep
    Vector3 r_p1_;
    /// Projected momentum at next timestep
    Vector3 p_p1_;
    class ParticleType {
      /// type id flag
      int typeid_;
      /// label
      std::string label_;
    };
    ParticleType type_;
  };

  
  /** Describes classical point particle with mass, position, and momentum */
  class Particle {
    public:
    /// Default particle has unit mass, located at the origin & zero momenta
    Particle();
    /// Create particle at R and with optional momenta P
    Particle(double M, const Vector3& R, const Vector3& P = Vector3());
    
    /// Return mass
    double m() const { return pimpl_->m_; }
    
    /// Return position
    const Vector3& r() const { return pimpl_->r_; }
    /// Return position
    Vector3& r() { return pimpl_->r_; }
    /// Return momentum
    const Vector3& p() const { return pimpl_->p_; }
    /// Return momentum
    Vector3& p() { return pimpl_->p_; }
    /// Return force
    const Vector3& f() const { return pimpl_->f_; }
    /// Return force
    Vector3& f() { return pimpl_->f_; }
    
    /// Return position at next step
    const Vector3& r_p1() const { return pimpl_->r_p1_; }
    /// Return position at next step
    Vector3& r_p1() { return pimpl_->r_p1_; }
    /// Return momentum at next step
    const Vector3& p_p1() const { return pimpl_->p_p1_; }
    /// Return momentum at next step
    Vector3& p_p1() { return pimpl_->p_p1_; }
    
    /// Particle moves: r_p1->r, p_p1->p
    void make_step();
    
    /// print to os
    void print(std::ostream& os) const;
    
    private:
    /// pointer to implementation
    std::unique_ptr<ParticleImpl> pimpl_;
    
  };
  
}

#endif  // end of include guard

