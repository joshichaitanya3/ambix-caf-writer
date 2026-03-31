#include "ambix_encoder.h"
#include <stdexcept>
#include <vector>

namespace ambix {

float AssociatedLegendrePolynomial(int L, int M, float x) {
    if (L == 0 && M == 0) {
        return 1;
    }
    else if (M < 0) {
        return static_cast<float>(std::pow(-1, -M)) * Factorial(L+M) / Factorial(L-M) * AssociatedLegendrePolynomial(L, -M, x);
    }
    else if (L == 1 && M == 0) {
        return x;
    }
    else if (L == 1 && M == 1) {
        return -std::sqrt(1.0f - x * x);
    }
    else if (L == 2 && M == 0) {
        return 0.5f * (3.0f * x * x - 1.0f);
    }
    else if (L == 2 && M == 1) {
        return -3.0f * x * std::sqrt(1.0f - x * x);
    }
    else if (L == 2 && M == 2) {
        return 3.0f * (1.0f - x * x);
    }
    else if (L == 3 && M == 0) {
        return 0.5f * (5.0f * std::pow(x, 3) - 3.0f * x);
    }
    else if (L == 3 && M == 1) {
        return -3.0f / 2.0f * (5.0f * x * x - 1.0f) * std::sqrt(1.0f - x * x);
    }
    else if (L == 3 && M == 2) {
        return 15.0f * x * (1.0f - x * x);
    }
    else if (L == 3 && M == 3) {
        return -15.0f * std::pow(1.0f - x * x, 3.0f / 2.0f);
    }
    else if (L == 4 && M == 0) {
        return 1.0f / 8.0f * (35.0f * std::pow(x, 4) - 30.0f * x * x + 3.0f);

    }
    else if (L == 4 && M == 1) {
        return -5.0f / 2.0f * (7.0f * std::pow(x, 3) - 3.0f * x) * std::sqrt(1.0f - x * x);
    }
    else if (L == 4 && M == 2) {
        return 15.0f / 2.0f * (7.0f * x * x - 1.0f) * (1.0f - x * x);
    }
    else if (L == 4 && M == 3) {
        return -105.0f * x * std::pow(1.0f - x * x, 3.0f / 2.0f);
    }
    else if (L == 4 && M == 4) {
        return 105.0f * std::pow(1.0f - x * x, 2);
    }
    else {
        throw std::runtime_error("0 <= L < 5 and -4 <= M <= 4 is required!");
    }
}

std::vector<float> AmbixEncoder::CalculateSphericalHarmonicsCoefficients(float azimuth, float elevation) {

    float azimuth_rad = kRadiansFromDegrees * azimuth;
    float elevation_rad = kRadiansFromDegrees * elevation;

    std::vector<float> sh_coeffs(num_channels_);
    for (int degree = 0; degree <= max_degree_; degree++) {
        for (int order = -degree; order <= degree; order++) {
            const int row = Acn(degree, order);
            
            const float last_term =
                (order >= 0) ? std::cos(static_cast<float>(order) * azimuth_rad)
                            : std::sin(static_cast<float>(-order) * azimuth_rad);
            
            float x = std::sin(elevation_rad);
            sh_coeffs.at(row) =
                Sn3dNormalizationFactor(degree, order) * AssociatedLegendrePolynomial(degree, order, x) *
                last_term;
        }
    }

    return sh_coeffs;
}

}
