#include <string>
#include <vector>
#include <iostream>
#include <variant>
#include <random>
#include <functional>
#include <net/net.hpp>
#include <net/tensor_network.hpp>
#define str std::to_string

int main(){

	net::tensor::TensorNetworkNoEnv<double> lat("square");
	for(int i=0;i<4;++i){
		for (int j=0;j<4;++j){
			lat.add("ten"+str(i)+"_"+str(j));
		}
	}
	for(int i=0;i<4;++i){
		for (int j=0;j<3;++j){
			lat.set_edge("ten"+str(i)+"_"+str(j),"ten"+str(i)+"_"+str(j+1));
		}
	}
	for(int i=0;i<3;++i){
		for (int j=0;j<4;++j){
			lat.set_edge("ten"+str(i)+"_"+str(j),"ten"+str(i+1)+"_"+str(j));
		}
	}

	net::gviz_theme=net::gviz_theme2;
	lat.draw(true);
	int seed =std::random_device()();
	std::default_random_engine random_engine(seed);
	using namespace std::placeholders;
	lat.init_nodes(std::bind(net::tensor::init_node_rand<net::stdEdgeKey>,_1,
		8,-1.,1.,std::ref(random_engine)));

	lat.absorb("ten1_1","ten1_2",net::tensor::no_absorb<double,std::string,std::monostate>,
		net::tensor::contract<double>);
	// double tot = lat.contract(net::tensor::no_absorb<double,std::string,std::monostate>,
	// 	net::tensor::contract<double>,net::tensor::contract_tree);

	net::gviz_theme=net::gviz_theme2;
	lat.draw(true);
	return 0;
}
