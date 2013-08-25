
#include <cstdlib>
#include <ctime>
#include <stdexcept>

#include <random.h>

using namespace dynamo;

Random::Random(double min, double max) :
min_(min), range_(max-min), range_over_randmax_(range_/RAND_MAX)
{
  if (range_ <= 0.0)
    throw std::runtime_error("Random::Random -- min must be less than max");
}

double
Random::operator()() const
{
  return range_over_randmax_ * rand() + min_;
}

// seed_ is static and created only once
Seed Random::seed_;

Seed::Seed()
{
  srand(time(NULL));
}

Seed::Seed(unsigned int s)
{
  srand(s);
}

