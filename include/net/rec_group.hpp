#ifndef REC_GROUP_H
#define REC_GROUP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <map>
#include <vector>
#include <set>
#include <initializer_list>
#include <cstdlib>
#include "network.hpp"
#include "network_draw.hpp"

namespace net{
	template <typename T,typename V>
	class rec_group{
	template <typename T1,typename V1>
	friend rec_group<T1,V1> contract(rec_group<T1,V1> &,rec_group<T1,V1> &,absorb_type<T1,V1>, contract_type<T1>);
	public:
		//constructor
		rec_group()=default;
		//copy constructor
		rec_group(const rec_group<T,V>&)=default;
		//copy assignment
		rec_group<T,V>& operator=(const rec_group<T,V>&)=default;
		//move constructor
		rec_group(rec_group<T,V>&&)=default;
		//move assignment
		rec_group<T,V>& operator=(rec_group<T,V>&&)=default;
		//destructor
		//~network();

		void belong(network<rec_group<T,V>,V> &);
		void absorb(const std::string &,absorb_type<T,V>, contract_type<T>);
		void draw(const std::string &,const bool);
		const T& get_val(const rec_group<T,V>&);

	private:
		network<rec_group<T,V>,V> * shallow_net;
		network<rec_group<T,V>,V> * deep_net;
		T val;
		std::set<std::string> shallow_contains;
		std::set<std::string> deep_contains;
	};

	template <typename T,typename V>
	const T& rec_group<T,V>::get_val(const rec_group<T,V> & n){
		return val;
	}

	template <typename T,typename V>
	void rec_group<T,V>::belong(network<rec_group<T,V>,V> & n){
		shallow_net=&n;
		deep_net=&n;
	}

	template <typename T,typename V>
	void rec_group<T,V>::absorb(const std::string & name,absorb_type<T,V> absorb_fun, contract_type<T> contract_fun){
		//shallow net: absorb
		//deep net: deep absorb
	}

	template <typename T,typename V>
	rec_group<T,V> contract(rec_group<T,V> & G1,rec_group<T,V> & G2,absorb_type<T,V> absorb_fun, contract_type<T> contract_fun){
		//shallow net: contract
		//deep net: deep contract
	}

	template<typename T,typename V>
	void rec_group<T,V>::draw(const std::string & filename,const bool label_bond){
		std::cout<<"For shallow network:"<<std::endl;
		shallow_net->draw(contains,label_bond);
		std::cout<<"For deep network:"<<std::endl;
		deep_net->draw(contains,label_bond);
	}
}
#endif