#include "indices.h"
#include <string>
#include <iostream>
#include <complex>
#include <type_traits>
// using ind::Index;
// using ind::Indices;

	template <typename T> //definition of template variable
	constexpr bool is_scalar_v=std::is_scalar_v<T>;

	template <typename T> //partial specialization
	constexpr bool is_scalar_v<std::complex<T> > =std::is_scalar_v<T>;

	template <typename T> //definition of template variable
	constexpr bool is_not_scalar_v=!std::is_scalar_v<T>;

	template <typename T> //partial specialization
	constexpr bool is_not_scalar_v<std::complex<T> > =!std::is_scalar_v<T>;

	template <typename T, std::enable_if_t<is_scalar_v<T>, int> = 0>
	std::ostream & output_bin(std::ostream & os,const T & m){
		//return os.write(reinterpret_cast<const char*>(&m), sizeof m);
		std::cout<<"called"<<std::endl;
		os<<m;
		std::cout<<"called2"<<std::endl;
		return os;
	}

	template <typename T, std::enable_if_t<is_not_scalar_v<T>, int> = 0>
	std::ostream & output_bin(std::ostream & os,const T & m){
		return os<<m;
	}

	template <typename T, std::enable_if_t<is_scalar_v<T>, int> = 0>
	std::istream & input_bin(std::istream & is,T & m){
		return is.read(reinterpret_cast<char*>(&m), sizeof m);
	}

	template <typename T, std::enable_if_t<is_not_scalar_v<T>, int> = 0>
	std::istream & input_bin(std::istream & is,T & m){
		return is>>m;
	}

int main(){
	// Indices ind({2,3,4,5,6});
	// ind.set_name(0,"T.1");
	// ind.set_name(1,"T.2");
	// ind.set_name(2,"T.3");
	// ind.set_name(3,"T.4");
	// ind.set_name(4,"T.5");
	// std::cout<<ind;
	// ind.forward(3);
	// std::cout<<ind;
	// std::cout<<ind.dim("T.2")<<std::endl;
	// ind.dim("T.2")=4;
	// std::cout<<ind.dim("T.2")<<std::endl;
	// std::cout<<ind.dim()<<std::endl;

	// int ld,md,rd;
	// ind.dims("T.2",ld,md,rd);
	std::cout<<"sss"<<std::endl;
	unsigned int len=2;

	output_bin(std::cout,len);

	enum Color { red, green, blue };
Color r = red;
std::cout<<red;


	return 0;
}