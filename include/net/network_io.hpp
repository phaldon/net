#ifndef NETWORK_IO_H
#define NETWORK_IO_H

#include <map>
#include <vector>
#include "site.hpp"
#include "network.hpp"

namespace net{

	template <typename T>
	std::ostream & operator<<(std::ostream & os,const std::set<T> & m);
	template <typename T>
	std::istream & operator>>(std::istream & is,const std::set<T> & m);
	template <typename T,typename V>
	std::ostream & operator<<(std::ostream & os,const std::map<T,V> & m);
	template <typename T,typename V>
	std::istream & operator>>(std::istream & is,std::map<T,V> & m);
	template <typename T>
	std::ostream & operator<<(std::ostream & os,const std::vector<T> & m);
	template <typename T>
	std::istream & operator>>(std::istream & is,const std::vector<T> & m);


	template <typename T,typename V>
	std::ostream & operator<<(std::ostream & os,const bond<T,V> & b){
		os<<b.name<<' '<<b.ind<<' '<<b.val;
		return os;
	}

	template <typename T,typename V>
	std::istream & operator>>(std::istream & is,bond<T,V> & b){
		is>>b.name>>b.ind>>b.val;
		return is;
	}

	template <typename T,typename V>
	std::ostream & operator<<(std::ostream & os,const site<T,V> & s){
		os<<s.bonds<<' '<<s.position<<' '<<s.val;
		return os;
	}

	template <typename T,typename V>
	std::istream & operator>>(std::istream & is,site<T,V> & s){
		is>>s.bonds>>s.position>>s.val;
		return is;
	}

	template <typename T,typename V>
	std::ostream & operator<<(std::ostream & os,const network<T,V> & n){
		os<<n.name<<' '<<n.name_at<<' '<<n.sites;
		return os;
	}

	template <typename T,typename V>
	std::istream & operator>>(std::istream & is,network<T,V> &n){
		is>>n.name>>n.name_at>>n.sites;
		for(auto & s:n.sites){
			for (auto & b: s.bonds){
				b.neighbor=&(n.sites[b.name]);
			}
		}
		return is;
	}

	template <typename T,typename V>
	std::ostream & operator<<(std::ostream & os,const std::map<T,V> & m){
		os<<m.size();
		for(auto & i : m){
			os<<i.first<<' '<<i.second<<' ';
		}
		return os;
	}

	template <typename T,typename V>
	std::istream & operator>>(std::istream & is,std::map<T,V> & m){
		int len;
		is>>len;
		T a;
		V b;
		m.clear();
		for (int i=0;i<len;++i){
			is>>a>>b;
			m[a]=b;
		}
		return is;
	}

	template <typename T>
	std::ostream & operator<<(std::ostream & os,const std::vector<T> & m){
		os<<m.size();
		for(auto & i : m){
			os<<i<<' ';
		}
		return os;
	}

	template <typename T>
	std::istream & operator>>(std::istream & is,const std::vector<T> & m){
		int len;
		is>>len;
		T a;
		m.clear();
		for (int i=0;i<len;++i){
			is>>a;
			m.pushback(a);
		}
		return is;
	}

	template <typename T>
	std::ostream & operator<<(std::ostream & os,const std::set<T> & m){
		os<<m.size();
		for(auto & i : m){
			os<<i<<' ';
		}
		return os;
	}

	template <typename T>
	std::istream & operator>>(std::istream & is,const std::set<T> & m){
		int len;
		is>>len;
		T a;
		m.clear();
		for (int i=0;i<len;++i){
			is>>a;
			m.insert(a);
		}
		return is;
	}

}
#endif