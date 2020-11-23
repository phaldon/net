#ifndef NET_NETWORK_IO_HPP
#define NET_NETWORK_IO_HPP

#include <map>
#include <vector>
#include "site.hpp"
#include "network.hpp"
#include "type.hpp"

namespace net{
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	class network;


	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::ostream & operator<<(std::ostream & os,const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & n){
		os<<n.name<<' '<<n.sites.size();
		for(auto & i : n.sites){
			os<<' ';
			Trait::nodekey_write_text(os,i.first);
			os<<' ';
			output_site_text(os,i.second);
		}
		return os;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::ostream & output_site_text(std::ostream & os,const site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & s){
		os<<s.bonds.size()<<' ';
		for(auto & i : s.bonds){
			Trait::edgekey_write_text(os,i.first);
			os<<' ';
			output_bond_text(os,i.second);
			os<<' ';
		}
		Trait::nodeval_write_text(os,s.val);
		return os;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::ostream & output_bond_text(std::ostream & os,const bond<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & b){
		Trait::nodekey_write_text(os,b.name);
		os<<' ';
		Trait::edgekey_write_text(os,b.ind);
		os<<' ';
		Trait::edgeval_write_text(os,b.val);
		return os;
	}


	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::istream & operator>>(std::istream & is,network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> &n){
		unsigned int len;
		is>>n.name>>len;
		NodeKey a;
		n.sites.clear();
		for (int i=0;i<len;++i){
			Trait::nodekey_read_text(is,a);
			input_site_text(is,n.sites[a]);
		}
		for(auto & s:n.sites){
			for (auto & b: s.second.bonds){
				b.second.neighbor=&(n.sites[b.second.name]);
			}
		}
		return is;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::istream & input_site_text(std::istream & is,site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & s){
		unsigned int len;
		is>>len;
		EdgeKey a;
		s.bonds.clear();
		for (int i=0;i<len;++i){
			Trait::edgekey_read_text(is,a);
			input_bond_text(is,s.bonds[a]);
		}
		Trait::nodeval_read_text(is,s.val);
		return is;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::istream & input_bond_text(std::istream & is,bond<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & b){
		Trait::nodekey_read_text(is,b.name);
		Trait::edgekey_read_text(is,b.ind);
		Trait::edgeval_read_text(is,b.val);
		return is;
	}

}
#endif