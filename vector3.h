
#ifndef _dynamo_vector3_h_
#define _dynamo_vector3_h_

#include <cmath>

namespace dynamo {
  /** Describes a 3-dimensional vector */
  class Vector3 {
    public:
    /// Default vector is (0.0, 0.0, 0.0)
    Vector3() { for(int i=0; i<3; i++) r_[i] = 0.0; }
    /// Construct vector (Rx, Ryx, Rz)
    Vector3(double Rx, double Ry, double Rz) { r_[0] = Rx; r_[1] = Ry; r_[2] = Rz; }
    /// Copy constructor
    Vector3(const Vector3& A) { for(int i=0; i<3; i++) r_[i] = A.r_[i]; }
    
    /// Assignment
    Vector3& operator=(const Vector3& A) { for(int i=0; i<3; i++) r_[i] = A.r_[i]; return *this; }
    /// Scale by a
    Vector3& scale(double a) { for(int i=0; i<3; i++) r_[i] *= a; return *this; }
    /// Element access
    double operator[](unsigned xyz) const { return r_[xyz]; }
    /// Element access
    double& operator[](unsigned xyz) { return r_[xyz]; }
    /// Add A to this
    Vector3& operator+=(const Vector3& A) {
      for(int i=0; i<3; i++) r_[i] += A.r_[i];
      return *this;
    }
    /// Subtract A from this
    Vector3& operator-=(const Vector3& A) {
      for(int i=0; i<3; i++) r_[i] -= A.r_[i];
      return *this;
    }
    
    private:
    double r_[3];
  };
  
  /// compute A+B
  inline Vector3 operator+(const Vector3& A, const Vector3& B) { return Vector3(A[0]+B[0],A[1]+B[1],A[2]+B[2]); }
  /// compute A-B
  inline Vector3 operator-(const Vector3& A, const Vector3& B) { return Vector3(A[0]-B[0],A[1]-B[1],A[2]-B[2]); }
  /// compute a*A
  inline Vector3 operator*(double a, const Vector3& A) { return Vector3(a*A[0],a*A[1],a*A[2]); }
  /// compute A*a
  inline Vector3 operator*(const Vector3& A, double a) { return Vector3(a*A[0],a*A[1],a*A[2]); }
  /// compute |A|^2
  inline double norm2(const Vector3& A) { return A[0]*A[0] + A[1]*A[1] + A[2]*A[2]; }
  /// compute |A|
  inline double norm(const Vector3& A) { return std::sqrt(norm2(A)); }
 
}

#endif // include guards

