
/**
   MOlecular DYNAmics prototype
*/

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>

#include <box.h>
#include <particle.h>
#include <random.h>
#include <integrator.h>
#include <boltzmann.h>
#include <potential.h>

using std::cout;
using std::endl;
using namespace dynamo;

std::string header();

void try_main(int argc, char** argv);

int
main(int argc, char** argv)
{
  try {
    try_main(argc,argv);
  }
  catch(std::exception& e) {
    std::cerr << "Caught a standard expection" << e.what() << std::endl;
    return 1;
  }
  catch(...) {
    std::cerr << "Caught an unknown expection" << std::endl;
    return 1;
  }

  return 0;
}

void
try_main(int argc, char** argv)
{
  const int np = 1000;               // number of particles
  const double timestep = 0.001;     // dt
  const int nsteps = 10000;          // # of timesteps for simulation
  const int nsteps_relax = 1000;     // # of timesteps for relaxation
  const int nsteps_stat = 1000;      // # of timesteps for thermostat
  const double L = 125.0;         // size of the box
  const double S = 6.0 * L * L;  // surface area of the box
  const double V = L*L*L;        // volume
  const double T = 35.05;          // projected temperature in Kelvin

  const int debug = 0;           // Set to value >= 1 to print out progressively more debugging info
  const unsigned int print_every_nsteps = 100;   // Print info every thismany steps
  
// Define internal units to be Angstroms,g,ps

//kb *( (10e10 Angstroms /1m)**2 (1s/1e12 ps)**2 )
   const double mks2Akps=pow( (1.0e10/1.e12),2);
//Define units for the simulation [kg, Angstrom, ps]
   const double kb=1.38065e-23*mks2Akps;
//sigma in [Angstroms]
   const double sigma = 2.786;
//epsilon in [kg Anstroms ps]
   const double epsilon= kb*36.76;

  cout << header() << endl;
  cout << "  # of particles       = " << np << endl
       << "  Temp                 = " << T << endl
       << "  box size [Angstroms] = " << L << endl
       << "  # of steps           = " << nsteps << endl
       << "  time step [ps]       = " << timestep << endl
       << "  Potential            = Lennard-Jones (sigma [Angstroms] = "
                                      << sigma << endl 
       << "                                        epsilon [Joules]  = " 
                                      << epsilon << ")" << endl
       << "  Print freq           = " << print_every_nsteps << endl;
  
  // Create box
  Box box(L,L,L);
  // Create particles
  std::vector<Particle> particles(np);
  // Put particles in the box
  box << particles;
  cout << "  Density [g/cm^3]     = " 
       << np*particles[1].m()*1000./(L*L*L)*pow(1.0e8,3) << endl 
       << "  Number Density       = " << np/(L*L*L) << std::endl;
  
  
  // Create integrator
  Integrator itor(timestep);
  // Lennard-Jones potential
  Potential_LJ potenLJ(sigma,epsilon);
  //Potential_Coulomb potenC(sigma,epsilon);
  
  // Propagate
  for(int step=0; step<nsteps+nsteps_relax+nsteps_stat; step++) {

    //
    // Pre-step: if relaxing, re-set all momenta to 0
    //
    if (step < nsteps_relax) {
      for(int i=0; i<np; i++)
        particles[i].p() = Vector3();
    }
    // if thermostating, reset momenta accoring to Boltzmann
    else if (step >= nsteps_relax && step < nsteps_relax+nsteps_stat) {
      // Put energy into particles
      particles << BoltzmannDistribution(T);
    }
    else if (step == nsteps_relax+nsteps_stat)
      box.reset();

    if (nsteps_relax > 0 && step == nsteps_relax) {
      std::cout << "STOPPED RELAXATION" << std::endl;
    }
    if (nsteps_stat > 0 && step == nsteps_relax+nsteps_stat) {
      std::cout << "TURNED OFF THERMOSTAT" << std::endl;
    }

    //
    // Step 0: compute potential and forces
    //
    double U = potenLJ.evaluate(particles);
    // also compute kinetic and total energy
    double T = 0.0;
    for(int i=0; i<np; i++) {
      T += norm2(particles[i].p()) / (2.0 * particles[i].m());
    }
// Convert U and T from internal units to J
    T=T/mks2Akps;
    U=U/mks2Akps;
    if (step%print_every_nsteps == 0){
      std::cout << "Step " << step << ":  U = " << U << "  T = " << T
                << "   E = " << U+T <<std::endl;
      std::cout << "         Temp (K) " << 2.*T/(3.*np*kb/mks2Akps) 
      << std::endl;
    }

    //
    // Step 1: compute new coordinates and momenta
    //
    for(int i=0; i<np; i++)
      itor.step(particles[i]);
    
    //
    // Step 2: enforce boundary conditions
    //
    for(int i=0; i<np; i++)
      box.contain(particles[i]);
    
    //
    // Step 3: make a step, i.e. update positions and momenta with their new values
    //
    for(int i=0; i<np; i++)
      particles[i].make_step();
    
    if (debug > 1) {
     for(int i=0; i<np; i++)
       particles[i].print(cout);
    }
    
    //
    // Step 4: compute something here
    //
    // e.g compute_g12(particles);
  }

// # mols
  const double mol=np/6.022e23;
// Volume in m^3;
  const double Vm=V/pow(1.e10,3.);  
  cout << "# of received hits = " << box.nhits() << endl;
// Momentum is in internal units of [kg Angstroms/ps]
  cout << "Transferred this much momentum to box (kg m/s)= " 
       << box.ptrans()*(1.e12/1.e10) << endl;
// 1Pascal=1N/m^2
  cout << "Measured pressure         [N/m^2] = " 
       << box.ptrans()*(1.e12/1.e10)/( (nsteps * timestep)/1.e12 * S/pow(1.e10,2.) ) << endl;
  cout << "Computed pressure (pvnkT) [N/m^2] = " 
       << np * kb/mks2Akps*T/Vm << endl;
// Constants for Ne from Atkins a= 2.107x10^-2 (Pa m^3/mol^2) 
//                              b= 1.709x10^-6 (m^3/mol) 
  const double VdW_a=0.02107;
  const double VdW_b=1.709e-6;
  cout << "Computed pressure (VdW)   [N/m^2] = "
       << mol*8.314*T/(Vm-mol*VdW_b) - VdW_a*(mol/V)*(mol/V)  << endl;
}

std::string
header()
{
  std::ostringstream oss;
  oss << "  ---------------------" << endl;
  oss << "    Welcome to DYNAMO" << endl;
  oss << "  ---------------------" << endl;
  
  return oss.str();
}

