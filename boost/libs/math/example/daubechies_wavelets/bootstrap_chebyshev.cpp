#include <iostream>
#include <boost/math/special_functions/daubechies_scaling.hpp>
#include <boost/math/special_functions/chebyshev_transform.hpp>

template<typename Real, int p>
void bootstrap()
{
    std::cout << "Computing phi. . .\n";
    auto phi = boost::math::daubechies_scaling<Real, p>();
    std::cout << "Computing Chebyshev transform of phi.\n";
    auto cheb = boost::math::chebyshev_transform(phi, phi.support().first, phi.support().second);
    std::cout << "Number of coefficients = " << cheb.coefficients().size() << "\n";
}

int main()
{
    bootstrap<long double, 9>();
}