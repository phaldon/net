#ifndef NET_NETWOTK_BIO_HPP
#define NET_NETWOTK_BIO_HPP

#include <map>
#include <vector>
#include <complex>
#include "site.hpp"
#include "network.hpp"
#include "type.hpp"

namespace net{
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	class network;

	template <typename T, std::enable_if_t<is_scalar_v<T>, int> = 0>
	std::ostream & output_bin(std::ostream & os,const T & m){
		return os.write(reinterpret_cast<const char*>(&m), sizeof m);
	}

	std::ostream & output_bin(std::ostream & os,const std::string & m){
		unsigned int len=m.size();
		output_bin(os,len);
		return os.write(&m[0], len);
	}

	template <typename T, std::enable_if_t<is_scalar_v<T>, int> = 0>
	std::istream & input_bin(std::istream & is,T & m){
		return is.read(reinterpret_cast<char*>(&m), sizeof m);
	}

	std::istream & input_bin(std::istream & is,std::string & m){
		unsigned int len;
		input_bin(is,len);
		m.resize(len);
		return is.read(&m[0], len);
	}


	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::ostream & operator<(std::ostream & os,const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & n){
		output_bin(os,n.name);
		output_bin(os,n.sites.size());
		for(auto & i : n.sites){
			Trait::nodekey_write_bin(os,i.first);
			output_site_bin(os,i.second);
		}
		return os;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::ostream & output_site_bin(std::ostream & os,const site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & s){
		output_bin(os,s.bonds.size());
		for(auto & i : s.bonds){
			Trait::edgekey_write_bin(os,i.first);
			output_bond_bin(os,i.second);
		}
		Trait::nodeval_write_bin(os,s.val);
		return os;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::ostream & output_bond_bin(std::ostream & os,const bond<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & b){
		Trait::nodekey_write_bin(os,b.name);
		Trait::edgekey_write_bin(os,b.ind);
		Trait::edgeval_write_bin(os,b.val);
		return os;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::istream & operator>(std::istream & is,network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> &n){
		input_bin(is,n.name);
		unsigned int len;
		input_bin(is,len);
		NodeKey a;
		n.sites.clear();
		for (int i=0;i<len;++i){
			Trait::nodekey_read_bin(is,a);
			input_site_bin(is,n.sites[a]);
		}
		for(auto & s:n.sites){
			for (auto & b: s.second.bonds){
				b.second.neighbor=&(n.sites[b.second.name]);
			}
		}
		return is;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::istream & input_site_bin(std::istream & is,site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & s){
		unsigned int len;
		input_bin(is,len);
		EdgeKey a;
		s.bonds.clear();
		for (int i=0;i<len;++i){
			Trait::edgekey_read_bin(is,a);
			input_bond_bin(is,s.bonds[a]);
		}
		Trait::nodeval_read_bin(is,s.val);
		return is;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::istream & input_bond_bin(std::istream & is,bond<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & b){
		Trait::nodekey_read_bin(is,b.name);
		Trait::edgekey_read_bin(is,b.ind);
		Trait::edgeval_read_bin(is,b.val);
		return is;
	}

}
#endif