
#include <vector>

#include <particle.h>

#ifndef _dynamo_box_h_
#define _dynamo_box_h_

namespace dynamo {
  
  /** Rectagular three-dimensional box of fixed dimensions */
  class Box {
    public:
    /// dimensions of the box are Lx, Ly, and Lz.
    Box(double Lx, double Ly, double Lz);
    ~Box();
    
    /// put a particle in a box
    const Box& operator<<(Particle& p) const;
    /// put set of particles in a box
    const Box& operator<<(std::vector<Particle>& pv) const;
    /** if the particle moves outside the box it will reflect from
        the walls */
    void contain(Particle& p);
    
    /// Number of hits received by the box
    unsigned long nhits() const;
    /// Total momentum transferred by the particles to the box
    double ptrans() const;
    /// Resets the number of hits and total momentum transferred to the box
    void reset();
    
    private:
    /// Dimensions of the box
    double L_[3];
    /// total number of particle hits
    unsigned long nhits_;
    /// total amount of momentum transferred to the box by the particle hits
    double ptrans_;
    
  };

}

#endif // include guard
