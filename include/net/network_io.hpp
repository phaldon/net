#ifndef NET_NETWORK_IO_HPP
#define NET_NETWORK_IO_HPP

#include <map>
#include <vector>
#include "node.hpp"
#include "network.hpp"
#include "type.hpp"

namespace net{
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	class network;


	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::ostream & operator<<(std::ostream & os,const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & n){
		os<<n.name<<' '<<n.nodes.size();
		for(auto & i : n.nodes){
			os<<' ';
			Trait::nodekey_write_text(os,i.first);
			os<<' ';
			output_node_text(os,i.second);
		}
		return os;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::ostream & output_node_text(std::ostream & os,const node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & s){
		os<<s.edges.size()<<' ';
		for(auto & i : s.edges){
			Trait::edgekey_write_text(os,i.first);
			os<<' ';
			output_edge_text(os,i.second);
			os<<' ';
		}
		Trait::nodeval_write_text(os,s.val);
		return os;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::ostream & output_edge_text(std::ostream & os,const edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & b){
		Trait::nodekey_write_text(os,b.nbkey);
		os<<' ';
		Trait::edgekey_write_text(os,b.nbind);
		os<<' ';
		Trait::edgeval_write_text(os,b.val);
		return os;
	}


	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::istream & operator>>(std::istream & is,network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> &n){
		unsigned int len;
		is>>n.name>>len;
		NodeKey a;
		n.nodes.clear();
		for (int i=0;i<len;++i){
			Trait::nodekey_read_text(is,a);
			input_node_text(is,n.nodes[a]);
		}
		for(auto & s:n.nodes)
			s.relink(n.nodes);
		return is;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::istream & input_node_text(std::istream & is,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & s){
		unsigned int len;
		is>>len;
		EdgeKey a;
		s.edges.clear();
		for (int i=0;i<len;++i){
			Trait::edgekey_read_text(is,a);
			input_edge_text(is,s.edges[a]);
		}
		Trait::nodeval_read_text(is,s.val);
		return is;
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	std::istream & input_edge_text(std::istream & is,edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & b){
		Trait::nodekey_read_text(is,b.nbkey);
		Trait::edgekey_read_text(is,b.nbind);
		Trait::edgeval_read_text(is,b.val);
		return is;
	}

}
#endif
