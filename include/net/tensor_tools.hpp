#ifndef NET_TENSOR_TOOLS_HPP
#define NET_TENSOR_TOOLS_HPP

#include <TAT/TAT.hpp>
#include "network.hpp"
#include "traits.hpp"
#include <random>
#include <variant>

namespace net{
	namespace tensor{
		template <typename T,typename EdgeKey=stdEdgeKey>
		using Tensor=TAT::Tensor<T,TAT::NoSymmetry,EdgeKey>;

		template <typename T,typename SiteKey=std::string,typename EdgeKey=stdEdgeKey>
		using TensorNetworkEnv=network<Tensor<T,EdgeKey>,Tensor<T,EdgeKey>,SiteKey,EdgeKey,default_traits<Tensor<T,EdgeKey>,Tensor<T,EdgeKey>,SiteKey,EdgeKey>>;
		template <typename T,typename SiteKey=std::string,typename EdgeKey=stdEdgeKey>
		using TensorNetworkNoEnv=network<Tensor<T,EdgeKey>,std::monostate,SiteKey,EdgeKey,default_traits<Tensor<T,EdgeKey>,Tensor<T,EdgeKey>,SiteKey,EdgeKey>>;

		template <typename EdgeKey=stdEdgeKey>
		Tensor<double,EdgeKey> init_node_rand(const std::vector<EdgeKey> & str_inds,
			const unsigned int D,const double min,const double max,std::default_random_engine & R){

			auto distribution = std::uniform_real_distribution<double>(min,max);
			std::vector<unsigned int> dims;
			for (auto& ind:str_inds){
				dims.push_back(D);
			}
			Tensor<double> result(str_inds,{dims.begin(), dims.end()});
			return result.set([&distribution, &R]() { return distribution(R); });
		}

		template<typename T,typename EdgeKey=stdEdgeKey>
		int get_dim(Tensor<T,EdgeKey> ten, int s){
			return ten.core->edges[s].map[TAT::NoSymmetry()];
		}

		template<typename T,typename EdgeKey=stdEdgeKey>
		int get_dim(Tensor<T,EdgeKey> ten, EdgeKey s){
			return ten.core->edges[ten.name_to_index[s]].map[TAT::NoSymmetry()];
		}

		template<typename T,typename EdgeKey=stdEdgeKey>
		Tensor<T,EdgeKey> init_edge_one(const unsigned int D, const EdgeKey & edge1, const EdgeKey & edge2){
			Tensor<T,EdgeKey> result({edge1,edge2},{D,D});
			result.zero();
			for(int i=0;i<D;++i){
				result.block()[i*(D+1)]=1.;
			}
		}

		template <typename T,typename EdgeKey=stdEdgeKey,typename V=Tensor<T,EdgeKey>>
		void default_dec(Tensor<T,EdgeKey>& ten1,Tensor<T,EdgeKey>& ten2,Tensor<T,EdgeKey>& ten3,
			const std::set<EdgeKey> & inds,const EdgeKey & ind1,const EdgeKey & ind2,V& env){
			ten2=ten1;
			ten3=ten1;
		}

		template <typename T,typename EdgeKey=stdEdgeKey>
		void absorb(Tensor<T,EdgeKey>& ten1,Tensor<T,EdgeKey>& ten2,const EdgeKey & ind){
			ten1.contract(ten2,{{ind,ten2.names[0]}});
			ten1.edge_rename({{ten2.names[1],ind}});
		}

		template <typename T,typename EdgeKey=stdEdgeKey,typename V=Tensor<T,EdgeKey>>
		void no_absorb(Tensor<T,EdgeKey>& ten1,V& ten2,const EdgeKey & ind){ }

		template <typename T,typename EdgeKey=stdEdgeKey>
		Tensor<T,EdgeKey> contract(Tensor<T,EdgeKey>& ten1,Tensor<T,EdgeKey>& ten2,
			const std::set<std::pair<EdgeKey,EdgeKey>> & inds){
			return Tensor<T,EdgeKey>::contract(ten1,ten2,{inds.begin(),inds.end()});
		}

		template <typename T,typename EdgeKey=stdEdgeKey>
		std::monostate zero_map(const Tensor<T,EdgeKey> &ten){
			return std::monostate();
		}
	}
}

#endif
