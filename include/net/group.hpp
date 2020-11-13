#ifndef GROUP_H
#define GROUP_H

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
	class group{
	template <typename T1,typename V1>
	friend group<T1,V1> contract(group<T1,V1> &,group<T1,V1> &,absorb_type<T1,V1>, contract_type<T1>);
	public:
		//constructor
		group()=default;
		//copy constructor
		group(const group<T,V>&)=default;
		//copy assignment
		group<T,V>& operator=(const group<T,V>&)=default;
		//move constructor
		group(group<T,V>&&)=default;
		//move assignment
		group<T,V>& operator=(group<T,V>&&)=default;
		//destructor
		//~network();

		void belong(network<T,V> &);
		void absorb(const std::string &,absorb_type<T,V>,contract_type<T> );
		void draw(const std::string &,const bool);

	private:
		network<T,V> * net;
		T val;
		std::set<std::string> contains;
	};

	template <typename T,typename V>
	void group<T,V>::belong(network<T,V> & n){
		net=&n;
	}

	template <typename T,typename V>
	void group<T,V>::absorb(const std::string & name,absorb_type<T,V> absorb_fun, contract_type<T> contract_fun){
		net->tn_contract1(name,contains,val,absorb_fun,contract_fun);
		contains.insert(name);
	}

	template <typename T,typename V>
	group<T,V> contract(group<T,V> & G1,group<T,V> & G2,absorb_type<T,V> absorb_fun, contract_type<T> contract_fun){
		group<T,V> G3;
		G3.net=G1.net;
		G3.contains=G1.contains;
		G3.insert(G2.begin(),G2.end());
		G3.ten=tn_contract2(G1.contains,G1.val,G2.contains,G2.val,absorb_fun,contract_fun);
	}

	template<typename T,typename V>
	void group<T,V>::draw(const std::string & filename,const bool label_bond){
		net->draw(contains,label_bond);
	}
}
#endif