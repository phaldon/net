#ifndef NET_TENSOR_TOOLS_HPP
#define NET_TENSOR_TOOLS_HPP

// #include "network.hpp"
#include <random>
#include <variant>
#include <functional>
#include <iostream>
#include "traits.hpp"
#include <TAT/TAT.hpp>

namespace net{
	namespace tensor{
		template <typename T,typename EdgeKey=stdEdgeKey>
		using Tensor=TAT::Tensor<T,TAT::NoSymmetry,EdgeKey>;

		template<typename T,typename EdgeKey=stdEdgeKey>
		int get_dim(Tensor<T,EdgeKey> ten, int s){
			return ten.core->edges[s].map[TAT::NoSymmetry()];
		}

		template<typename T,typename EdgeKey=stdEdgeKey>
		int get_dim(Tensor<T,EdgeKey> ten, EdgeKey s){
			return ten.core->edges[ten.name_to_index[s]].map[TAT::NoSymmetry()];
		}
		template<typename T,typename EdgeKey=stdEdgeKey>
		void diminfo(Tensor<T,EdgeKey> ten, std::ostream & os){
			for(auto & s:ten.names)
				os<<s<<' '<<ten.core->edges[ten.name_to_index[s]].map[TAT::NoSymmetry()]<<' ';
			os<<'\n';
		}

		/**
		* \brief 得到tensor的对角元
		*/
		template<typename T,typename EdgeKey=stdEdgeKey>
		Tensor<T,EdgeKey> get_diag(const Tensor<T,EdgeKey> & ten,std::vector<std::pair<EdgeKey,EdgeKey>> names){
			std::vector<EdgeKey> oldname;
			for(auto &n:names)
				oldname.push_back(n.first);
			for(auto &n:names)
				oldname.push_back(n.second);
			auto ten2=ten.transpose(oldname);
			std::vector<EdgeKey> newname;
			std::vector<int> newdims;
			int dim, newsize=1;
			for(int i=0;i<names.size();++i){
				newname.push_back(oldname[i]);
				dim = get_dim(ten2,i);
				newdims.push_back(dim);
				newsize*=dim;
			}
			Tensor<T,EdgeKey> result(newname,newdims);
			for(int i=0;i<newsize;++i){
				result.block()[i]=ten2.block()[i*(newsize+1)];
			}
			return result;
		}

	}
}

#endif
