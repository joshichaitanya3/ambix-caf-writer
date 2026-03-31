#ifndef AMBIX_CAF_WRITER_AMBIX_ENCODER_H_
#define AMBIX_CAF_WRITER_AMBIX_ENCODER_H_

#include <stdint.h>
#include <cmath>
#include <vector>
#include <numbers>

// Defines conversion factor from degrees to radians.
static const float kRadiansFromDegrees =
    static_cast<float>(std::numbers::pi_v<float> / 180.0);

// Defines conversion factor from radians to degrees.
static const float kDegreesFromRadians =
    static_cast<float>(180.0 / std::numbers::pi_v<float>);

namespace ambix {

/*!\brief Computes the Ambisonic Channel Number (ACN) from a degree and order.
 * For degree n and order m, this is given by n^2 + n + m
 *
 * \param degree Degree of the spherical harmonic (`l` in P^l_m).
 * \param order Order of the spherical harmonic (`m` in P^l_m).
 * \return Ambisonic channel number
 */
inline int Acn(int degree, int order) {
    return degree * degree + degree + order;
}

/*!\brief Returns the factorial of x
 *
 * \param x Input to take factorial of.
 * \return Computed factorial of input; 0 if the input is negative.
 */
inline float Factorial(int x) {
    if (x < 0) {
        return 0.0f;
    }
    float result = 1.0f;
    for (; x > 1; x--) {
        result *= static_cast<float>(x);
    }
    return result;
}

/*!\brief Computes normalization factor for Schmidt semi-normalized harmonics used in AmbiX.
 *
 * \param degree Degree of the spherical harmonic.
 * \param order Order of the spherical harmonic.
 * \return Computed normalization factor.
 */
inline float Sn3dNormalizationFactor(int degree, int order) {
    return std::sqrt(((order == 0) ? 1.0f : 2.0f) * Factorial(degree - std::abs(order)) / Factorial(degree + std::abs(order))); // sqrt( (2 - delta_order) * [(degree - mod_order)! / (degree + mod_order)!] )
}

/*!\brief Calculates the value of the associated Legendre Polynomial of degree L and order M at input x
 * TODO: Consider using double-and-add method for integer exponentiation that Google uses in their obr repository
 * \param x Input to the function
 * \return P_L^M(x)
*/
float AssociatedLegendrePolynomial(int L, int M, float x);

class AmbixEncoder {

private:

    int max_degree_;
    int num_channels_; // (max_degree_ + 1)²
    
public:

    AmbixEncoder() : AmbixEncoder(1) {};

    AmbixEncoder(int max_degree) {
        max_degree_ = max_degree;
        num_channels_ = (max_degree_ + 1) * (max_degree_ + 1);
    }
    
    int NumChannels() {
        return num_channels_;
    }

    /*!\brief Calculates the SN3D-normalized spherical harmonics (SH) coefficients and returns them in the ACN order
    \param output_filename Full path to the output file, with extension .caf
    \param azimuth Horizontal angle in degrees, with 0 being front 
    \param elevation Vertical angle in degrees, with 0 being horizontal and 90 being above
    \return sh_coeffs Vector or SH coefficients in ACN order (sh_coeffs[0] corresponds to 0-th channel and so on)
    */
    std::vector<float> CalculateSphericalHarmonicsCoefficients(float azimuth, float elevation);

};
    
}

#endif // AMBIX_CAF_WRITER_AMBIX_ENCODER_H_
