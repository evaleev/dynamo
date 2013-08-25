
#ifndef _dynamo_random_h_
#define _dynamo_random_h_

namespace dynamo {
  
  class Seed;
  
  /** Produces random numbers using C library's rand */
  class Random {
    public:
    /// generator will produce numbers in [min,max) interval
    Random(double min, double max);
    /// return next random number
    double operator()() const;
    
    private:
    /// range = max - min
    double min_, range_;
    /// range/RAND_MAX is precomputed because it's expensive
    double range_over_randmax_;
    /// used to seed the generator
    static Seed seed_;
    
  };
  
  /// Seed is used to seed srand once
  struct Seed {
    /// default constructor uses time() to seed
    Seed();
    /// uses s as a seed
    Seed(unsigned int s);
  };
  
}

#endif // include guard

