

#ifndef POLYNOMIAL_HPP_
#define POLYNOMIAL_HPP_

#include <vector>
#include <algorithm>
#include <iostream>

namespace r0 {

template<class T>
class single_variable_polynomial {
public:

	single_variable_polynomial();

	template<class C> //Container
	single_variable_polynomial(const C& c);

	template<class T2>
	friend std::ostream& operator<<(std::ostream& os, const single_variable_polynomial<T2>& p);

private:

	void normalize();

	//coeffs[i]x^i
	//so coeffs[0] is the constant
	//coeffs should always be normalized :
	// - coeffs.back() != 0 (if exists)

	std::vector<T> coeffs;
};


template<class T>
single_variable_polynomial<T>::single_variable_polynomial() : coeffs() {}

template<class T>
template<class C> //Container
single_variable_polynomial<T>::single_variable_polynomial(const C& c) : coeffs(c.begin(), c.end()) {
	normalize();
}

template<class T>
void single_variable_polynomial<T>::normalize() {
	int i;
	for ( i = coeffs.size() - 1; i >= 0 && coeffs[i] == 0; --i );

	coeffs.resize(i + 1);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const single_variable_polynomial<T>& p) {

	const char plus = '+';
	const char power = '^';
	const char variable = 'x';

	for (unsigned i = 0; i < p.coeffs.size(); ++i) {
		switch (i) {
		case 0:
			os << p.coeffs[i];
			break;
		case 1:
			os << p.coeffs[i] << variable;
			break;
		default:
			os << p.coeffs[i] << variable << power << i;
			break;
		}
		if ( p.coeffs.size()-1 != i ) {
			os << plus;
		}
	}
	return os;
}

} //namespace r0


#endif /* POLYNOMIAL_HPP_ */
