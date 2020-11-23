#ifndef NET_TYPE_HPP
#define NET_TYPE_HPP

#include <type_traits>
#include <complex>

namespace net{

	template <typename T> //definition of template variable
	constexpr bool is_scalar_v=std::is_scalar_v<T>;

	template <typename T> //partial specialization
	constexpr bool is_scalar_v<std::complex<T> > =std::is_scalar_v<T>;

}
#endif