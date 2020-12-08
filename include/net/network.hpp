#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <set>
#include <initializer_list>
#include <cstdlib>
#include <stdexcept>
#include "error.hpp"
#include "node.hpp"
#include "traits.hpp"
#include "tensor_tools.hpp"

namespace net{

	/**
	* \brief 格点的分解函数的类型
	*/
	template <typename NodeVal,typename EdgeVal,typename EdgeKey,typename Comp>
	using dec_type = std::function<void(const NodeVal&,NodeVal&,NodeVal&,
		const std::set<EdgeKey,Comp> &,const EdgeKey & ,const EdgeKey & ,EdgeVal&)>;

	/**
	* \brief 格点的吸收函数的类型
	*/
	template <typename NodeVal,typename EdgeVal,typename EdgeKey>
	using absorb_type = std::function<NodeVal(const NodeVal&,const EdgeVal&,const EdgeKey &)>;

	/**
	* \brief 格点的缩并函数的类型
	*/
	template <typename NodeVal,typename EdgeKey,typename Comp>
	using contract_type = std::function<NodeVal(const NodeVal&,const NodeVal&,const std::set<std::pair<EdgeKey,EdgeKey>,Comp> &)>;

	/**
	* \brief 格点信息的初始化函数的类型
	*/
	template <typename NodeVal,typename EdgeKey>
	using init_node_type = std::function<NodeVal(const std::vector<EdgeKey> &)>;

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	using init_node_type_full = std::function<NodeVal(const node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> &)>;

	/**
	* \brief 边信息的初始化函数的类型
	*/
	template <typename EdgeVal,typename EdgeKey>
	using init_edge_type = std::function<EdgeVal(const EdgeKey &,const EdgeKey &)>;

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	using init_edge_type_full = std::function<EdgeVal(const node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> &,
		const node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> &,const EdgeKey &,const EdgeKey &)>;

	template<typename NodeVal,typename EdgeVal,typename NodeKey,typename EdgeKey>
	struct default_traits;
   /**
    * \brief 网络是一些点和点间关联的并
    *
    * 每个格点拥有一个名字，整个网络也有自己的名字，网络可以通过格点名称寻找格点
    * \see node, edge
    * \tparam NodeVal 每个格点中附着的信息类型
    * \tparam EdgeVal 每个边上附着的信息类型
    * \tparam NodeKey 格点名字的类型
    * \tparam EdgeKey 边的名字的类型
    * \tparam Trait 以上类型的特征，包括输入输出和比较 
    */


	template <typename NodeVal, typename EdgeVal, typename NodeKey=std::string, typename EdgeKey=stdEdgeKey,
		typename Trait = default_traits<NodeVal,EdgeVal,NodeKey,EdgeKey> >
	class network{

	/**
	* \brief 网络的字符串输出
	*/
	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::ostream & operator<<(std::ostream &, const network<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	/**
	* \brief 网络的字符串输入
	*/
	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::istream & operator>>(std::istream &, network<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	/**
	* \brief 网络的二进制输出
	*/
	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::ostream & operator<(std::ostream &, const network<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	/**
	* \brief 网络的二进制输出
	*/
	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::istream & operator>(std::istream &, network<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	/**
	* \brief 利用格点上的信息的函数和边上信息的函数从一个网络得到另一个网络
	*/
	template<typename NodeVal1,typename EdgeVal1,typename NodeVal2,typename EdgeVal2, typename NodeKey1, typename EdgeKey1, typename Trait1>
	friend network<NodeVal2,EdgeVal2,NodeKey1,EdgeKey1,Trait1> fmap(const network<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> & n,
		std::function<NodeVal2(const NodeVal1 &)> f1,std::function<EdgeVal2(const EdgeVal1 &)> f2);

	/**
	* \brief 利用格点上的信息的函数和边上信息的函数从一个网络得到另一个网络，同时做sitekey和edgekey的变换
	*/
	template<typename NodeVal1,typename EdgeVal1,typename NodeVal2,typename EdgeVal2, typename NodeKey1, typename EdgeKey1, typename Trait1>
	friend network<NodeVal2,EdgeVal2,NodeKey1,EdgeKey1,Trait1> fmap(const network<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> & n,
		std::function<NodeVal2(const NodeVal1 &)> f1,std::function<EdgeVal2(const EdgeVal1 &)> f2,std::function<NodeKey1(const NodeKey1 &)> f3,
		std::function<EdgeKey1(const EdgeKey1 &)> f4);

	public:
		//constructor
		network()=default;
		network(const std::string & s):name(s){};
		//copy constructor
		network(const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&);
		//copy assignment
		network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& operator=(const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&);
		//move constructor
		network(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&&)=default;
		//move assignment
		network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& operator=(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&&)=default;
		//destructor
		//~network();

		using IterNode = typename std::map<NodeKey,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>,typename Trait::nodekey_less>::iterator;

		/**
		* \brief 对每个格点上的信息做变换
		*/
		void fope(std::function<NodeVal(const NodeVal&)>,std::function<EdgeVal(const EdgeVal&)>);

		/**
		* \brief 加一个格点
		*/
		IterNode add(const NodeKey &);

		/**
		* \brief 查找一个格点
		*/
		IterNode find(const NodeKey &);

		/**
		* \brief 加一个网络
		*/
		void add(const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> &);
		/**
		* \brief 加一条边
		*/
		void set_edge(const NodeKey &,const NodeKey &,const EdgeKey &,const EdgeKey &,const EdgeVal & =EdgeVal());
		void set_edge(IterNode,IterNode,const EdgeKey &,const EdgeKey &,const EdgeVal & =EdgeVal());
		/**
		* \brief 加一条边。根据格点名字和Trait::bind自动生成边的名字
		*/
		void set_edge(const NodeKey &,const NodeKey &,const EdgeVal & =EdgeVal());
		void set_edge(IterNode,IterNode,const EdgeVal & =EdgeVal());

		/**
		* \brief 删除一个格点
		*/
		void del(const NodeKey &);
		void del(IterNode);
		/**
		* \brief 删除两个格点之间的边
		*/
		void del_edge(const NodeKey &,const NodeKey &);
		void del_edge(IterNode,IterNode);
		/**
		* \brief 删除一个腿连着的边
		*/
		void del_leg(const NodeKey &,const EdgeKey &);
		void del_leg(IterNode,const EdgeKey &);

		/**
		* \brief 重命名一个格点
		*/
		IterNode rename(const NodeKey &,const NodeKey &);
		IterNode rename(const IterNode &,const NodeKey &);

		/**
		* \brief 将另一个格点缩入一个格点
		*/
		void absorb(const NodeKey &,const NodeKey &,absorb_type<NodeVal,EdgeVal,EdgeKey>,
			contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less>);
		void absorb(IterNode,IterNode,absorb_type<NodeVal,EdgeVal,EdgeKey>, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less>);
		/**
		* \brief 将另一个格点分解为两个格点
		*/
		void split(const NodeKey &,const NodeKey &,const NodeKey &, const std::set<EdgeKey,typename Trait::edgekey_less> &, const EdgeKey &, const EdgeKey &,
			dec_type<NodeVal,EdgeVal,EdgeKey,typename Trait::edgekey_less>);
		/**
		* \brief 将另一个格点分解为两个格点
		*/
		void split(const NodeKey &,const NodeKey &, const std::set<EdgeKey,typename Trait::edgekey_less> &, const EdgeKey &, const EdgeKey &,
			dec_type<NodeVal,EdgeVal,EdgeKey,typename Trait::edgekey_less>);
	#ifdef NET_GRAPH_VIZ
		/**
		* \brief 画出网络的图并输出到文件
		*/
		void draw_to_file(const std::string &,const bool);
		/**
		* \brief 画出网络的图并输出到文件，强调网络的一部分
		*/
		void draw_to_file(const std::string &,const std::set<NodeKey,typename Trait::nodekey_less> &,const bool);
		/**
		* \brief 画出网络的图并输出
		*/
		void draw(const bool);
		/**
		* \brief 画出网络的图并输出，强调网络的一部分
		*/
		void draw(const std::set<NodeKey,typename Trait::nodekey_less> &,const bool);
	#endif
		/**
		* \brief 将网络转化为graphviz格式的字符串
		*/
		std::string gviz(const std::set<NodeKey,typename Trait::nodekey_less> &,const bool);
		/**
		* \brief 判断网络是否包含一个格点
		*/
		bool exist(const NodeKey &);

		/**
		* \brief 判断网络是否是没有冲突
		*/
		bool consistency();
		bool consistency(std::ostream & diagnosis);

		/**
		* \brief 返回网络的一个格点的信息
		*/
		NodeVal& operator[](const NodeKey &);
		/**
		* \brief 设置网络的一个格点的信息
		*/
		void set_val(const NodeKey &,const NodeVal& );
		/**
		* \brief 初始化网络的格点的信息
		*/
		void init_nodes(init_node_type<NodeVal,EdgeKey> );
		void init_nodes(init_node_type_full<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> );
		/**
		* \brief 初始化网络的边的信息
		*/
		void init_edges(init_edge_type<EdgeVal,EdgeKey> );
		void init_edges(init_edge_type_full<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> );
		/**
		* \brief 缩并整个网络
		*/
		NodeVal contract(absorb_type<NodeVal,EdgeVal,EdgeKey>, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less>);

		/**
		* \brief 缩并的辅助函数
		*/
		void tn_contract1(const NodeKey &,const std::set<NodeKey,typename Trait::nodekey_less> &, NodeVal&, 
			absorb_type<NodeVal,EdgeVal,EdgeKey>, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less>);
		void tn_contract1(IterNode,const std::set<NodeKey,typename Trait::nodekey_less> &, NodeVal&, 
			absorb_type<NodeVal,EdgeVal,EdgeKey>, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less>);
		/**
		* \brief 缩并的辅助函数
		*/
		NodeVal tn_contract2(const std::set<NodeKey,typename Trait::nodekey_less> &, NodeVal&, const std::set<NodeKey,typename Trait::nodekey_less> &, NodeVal&,
			absorb_type<NodeVal,EdgeVal,EdgeKey>, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less>);

	private:
		/**
		* \brief 网络的格点
		*/
		std::map<NodeKey,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>,typename Trait::nodekey_less> nodes;
		/**
		* \brief 网络的名字
		*/
		std::string name="";
	};

	template<typename T>
	std::string to_string(const T& m){
		std::stringstream a;
		a<<m;
		return a.str();
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
		// this is not default because nodes[i].edges[j].neighbor needs redirection
	network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::network(const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& N){
		nodes=N.nodes;
		name=N.name;
		for(auto & s:nodes)
			s.second.relink(nodes);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
		// this is not default because nodes[i].edges[j].neighbor needs redirection
	network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::operator=
		(const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& N){
		if(this != &N){
			nodes=N.nodes;
			name=N.name;
			for(auto & s:nodes)
				s.second.relink(nodes);
		}
		return *this;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>	
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::init_nodes(init_node_type<NodeVal,EdgeKey> init_fun){

		for (auto& node:nodes){
			std::vector<EdgeKey> inds;
			for (auto& b:node.second.edges){
				inds.push_back(b.first);
			}
			node.second.val = init_fun(inds);
		}
	}
	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>	
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::init_nodes(init_node_type_full<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> init_fun){

		for (auto& node:nodes)
			node.second.val = init_fun(node.second);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::init_edges(init_edge_type<EdgeVal,EdgeKey> init_fun){

		for (auto& node:nodes){
			for (auto& b:node.second.edges){
				b.second.val=init_fun(b.first,b.second.nbind);
			}
		}
	}
	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>	
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::init_edges(init_edge_type_full<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> init_fun){

		for (auto& node:nodes){
			for (auto& b:node.second.edges){
				b.second.val=init_fun(node.second,b.second.nbitr->second,b.first,b.second.nbind);
			}
		}
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	NodeVal& network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::operator[](const NodeKey & nodekey){
		return nodes[nodekey].val;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_val(const NodeKey & nodekey,const NodeVal& value){
		nodes[nodekey].val=value;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	bool network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::exist(const NodeKey & nodekey){
		return (nodes.count(nodekey)==1);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	typename network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::find(const NodeKey & nodekey){
		
		auto node_itr = nodes.find(nodekey);
		if(node_itr == nodes.end()){
			throw key_unfound_error("In network.find, node "+to_string(nodekey)+" is not found!");
		}
		return node_itr;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	typename network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::add(const NodeKey & nodekey){
		auto [s1,succ1]=nodes.insert(make_pair(nodekey,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>()));
		if(!succ1){
			throw key_exist_error("In network.add, node "+to_string(nodekey)+" already exists!");
		}
		return s1;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::add(const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & n){
		for(auto & s:n.nodes)
			add(s.first)->second=s.second;
		for(auto & s:nodes)
			s.second.relink(nodes);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del(const NodeKey & nodekey){

		auto node_itr = nodes.find(nodekey);
		if(node_itr == nodes.end()){
			throw key_unfound_error("In network.del, node "+to_string(nodekey)+" is not found!");
		}

		node_itr->second.delete_nbedge();
		nodes.erase(node_itr);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it){

		it->second.delete_nbedge();
		nodes.erase(it);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del_edge(const NodeKey & nodekey1,const NodeKey& nodekey2){

		auto node_itr1 = nodes.find(nodekey1);
		if(node_itr1 == nodes.end()){
			throw key_unfound_error("In network.del_edge, node "+to_string(nodekey1)+" is not found!");
		}

		if(nodes.count(nodekey2)==0){
			throw key_unfound_error("In network.del_edge, node "+to_string(nodekey2)+" is not found!");
		}

		node_itr1->second.delete_edge([&nodekey2](auto & egitr){return egitr->second.nbkey==nodekey2;});

	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del_edge (network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it1,
		network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it2){

		it1->second.delete_edge([&it2](auto & egitr){return egitr->second.nbkey==it2->first;});
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del_leg(const NodeKey & nodekey,const EdgeKey & ind){

		auto node_itr = nodes.find(nodekey);
		if(node_itr == nodes.end()){
			throw key_unfound_error("In network.del_leg, node "+to_string(nodekey)+" is not found!");
		}
		node_itr->second.delete_edge([&ind](auto & egitr){return egitr->first==ind;});
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del_leg(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it,const EdgeKey & ind){

		it->second.delete_edge([&ind](auto & egitr){return egitr->first==ind;});
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	typename network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::rename
		(const NodeKey & old_key,const NodeKey & new_key){

		auto node_handle = nodes.extract(old_key);
		if(node_handle.empty()){
			throw key_unfound_error("In network.rename, node "+to_string(old_key)+" is not found!");
		}
		node_handle.key=new_key;
		node_handle.value.reset_nbkey_of_nb(new_key);
		
		auto status = nodes.insert(std::move(node_handle));
		if(!status.inserted)throw key_exist_error("In network.rename, node "+to_string(new_key)+" already exists!");

		return status.position;
	}


	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	typename network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::rename
		(const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode & it,const NodeKey & new_key){

		auto node_handle = nodes.extract(it);
		if(node_handle.empty()){
			throw key_unfound_error("In network.rename, node "+to_string(it->first)+" is not found!");
		}
		node_handle.key=new_key;
		node_handle.value.reset_nbkey_of_nb(new_key);
		
		auto status = nodes.insert(std::move(node_handle));
		if(!status.inserted)throw key_exist_error("In network.rename, node "+to_string(new_key)+" already exists!");
		
		return status.position;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_edge(const NodeKey & nodekey1,const NodeKey& nodekey2,
		const EdgeKey & ind1,const EdgeKey& ind2, const EdgeVal& edgeval){

		auto node_itr1 = nodes.find(nodekey1);
		if(node_itr1 == nodes.end()){
			throw key_unfound_error("In network.set_edge, node "+to_string(nodekey1)+" is not found!");
		}
		auto node_itr2 = nodes.find(nodekey2);
		if(node_itr2 == nodes.end()){
			throw key_unfound_error("In network.set_edge, node "+to_string(nodekey2)+" is not found!");
		}

		node_itr1->second.set_edge(ind1,nodekey2,ind2,node_itr2,edgeval);
		node_itr2->second.set_edge(ind2,nodekey1,ind1,node_itr1,edgeval);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_edge(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it1,
		network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it2, const EdgeKey & ind1,const EdgeKey& ind2,
		const EdgeVal& edgeval){
		
		it1->second.set_edge(ind1,it2->first,ind2,it2,edgeval);
		it2->second.set_edge(ind2,it1->first,ind1,it1,edgeval);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_edge(const NodeKey & nodekey1,const NodeKey& nodekey2,
		const EdgeVal& edgeval){
		set_edge(nodekey1,nodekey2,Trait::bind(nodekey1,nodekey2),Trait::bind(nodekey2,nodekey1),edgeval);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_edge(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it1,
		network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it2,const EdgeVal& edgeval){
		set_edge(it1,it2,Trait::bind(it1->first,it2->first),Trait::bind(it2->first,it1->first),edgeval);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::absorb(const NodeKey & nodekey1,const NodeKey & nodekey2,
		absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun){

		auto node_itr1 = nodes.find(nodekey1);
		if(node_itr1 == nodes.end()){
			throw key_unfound_error("In network.absorb, node "+to_string(nodekey1)+" is not found!");
		}
		auto node_itr2 = nodes.find(nodekey2);
		if(node_itr2 == nodes.end()){
			throw key_unfound_error("In network.absorb, node "+to_string(nodekey2)+" is not found!");
		}

		node_itr1->second.absorb_nb(nodekey2,node_itr2->second.val,absorb_fun,contract_fun);
		node_itr2->second.transfer_edge(node_itr1,false,[&nodekey1](auto & egitr){return egitr->second.nbkey!=nodekey1;});
		nodes.erase(node_itr2);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::absorb(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode node_itr1,
		network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode node_itr2,
		absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun){

		node_itr1->second.absorb_nb(node_itr2->first,node_itr2->second.val,absorb_fun,contract_fun);
		node_itr2->second.transfer_edge(node_itr1,false,[&node_itr1](auto & egitr){return egitr->second.nbkey!=node_itr1->first;});
		nodes.erase(node_itr2);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::split(const NodeKey & nodekey1, const NodeKey & nodekey2,const NodeKey & nodekey3
		,const std::set<EdgeKey,typename Trait::edgekey_less> & inds, const EdgeKey & ind2, const EdgeKey & ind3,
		dec_type<NodeVal,EdgeVal,EdgeKey,typename Trait::edgekey_less> dec_fun){

		auto s1=nodes.find(nodekey1);
		auto [s2,succ2]=nodes.insert(nodekey2);
		auto [s3,succ3]=nodes.insert(nodekey3);

		s1->second.transfer_edge(s2,s3,false,[&inds](auto & egitr){return inds.count(egitr->first)==0;});

		EdgeVal env;
		dec_fun(s1->val,s2->val,s3->val,inds,ind2,ind3,env);
		s2->second.set_edge(ind2,nodekey3,ind3,s3,env);
		s3->second.set_edge(ind3,nodekey2,ind2,s2,env);
		nodes.erase(s1);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::split(const NodeKey & nodekey1, const NodeKey & nodekey2,
		const std::set<EdgeKey,typename Trait::edgekey_less> & inds, const EdgeKey & ind1, const EdgeKey & ind2,
		dec_type<NodeVal,EdgeVal,EdgeKey,typename Trait::edgekey_less> dec_fun){

		auto s1=nodes.find(nodekey1);
		auto [s2,succ2]=nodes.insert(nodekey2);

		s1->second.transfer_edge(s2,true,[&inds](auto & egitr){return inds.count(egitr->first)==1;});

		auto temp=s1->val;
		EdgeVal env;
		dec_fun(temp,s1->val,s2->val,inds,ind1,ind2,env);
		s1->second.set_edge(ind1,nodekey2,ind2,s2,env);
		s2->second.set_edge(ind2,nodekey1,ind1,s1,env);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	NodeVal network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::contract(absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun,
		contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun){
		NodeVal tot;
		std::set<NodeKey,typename Trait::nodekey_less> contracted;
		for(auto s:nodes){
			if(contracted.count(s.first)==0){
				tn_contract1(s.first,contracted,tot,absorb_fun,contract_fun);
				contracted.insert(s.first);
			}
		}
		return tot;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::tn_contract1(const NodeKey & nodekey,const std::set<NodeKey,typename Trait::nodekey_less> & group, NodeVal& ten,
		absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun){

		auto node_itr1 = nodes.find(nodekey);
		if(node_itr1 == nodes.end()){
			throw key_unfound_error("In network.tn_contract1, node "+to_string(nodekey)+" is not found!");
		}

		if(group.size()==0){
			ten=node_itr1->second.val;
		}else{
			auto node_t=node_itr1->second.val;
			std::set<std::pair<EdgeKey,EdgeKey>,typename Trait::edge2key_less> ind_pairs;
			node_itr1->second.harmless_absorb_nb(node_t,absorb_fun,ind_pairs,
				[&group](auto & eg){return group.count(eg.second.nbkey)==1;});
			ten=contract_fun(node_t,ten,ind_pairs);
		}
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::tn_contract1(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it1,
		const std::set<NodeKey,typename Trait::nodekey_less> & group, NodeVal& ten,
		absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun){

		if(group.size()==0){
			ten=it1->second.val;
		}else{
			auto node_t=it1->second.val;
			std::set<std::pair<EdgeKey,EdgeKey>,typename Trait::edge2key_less> ind_pairs;

			it1->second.harmless_absorb_nb(node_t,absorb_fun,ind_pairs,
				[&group](auto & eg){return group.count(eg.second.nbkey)==1;});

			ten=contract_fun(node_t,ten,ind_pairs);
		}
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	NodeVal network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::tn_contract2(const std::set<NodeKey,typename Trait::nodekey_less> & group1, NodeVal& ten1,
		const std::set<NodeKey,typename Trait::nodekey_less> & group2, NodeVal& ten2,
		absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun){

		if (group1.size()==0)
			return ten2;
		if (group2.size()==0)
			return ten1;
		std::set<std::pair<EdgeKey,EdgeKey>,typename Trait::edge2key_less> ind_pairs;
		for (auto & nk:group1)
			nodes[nk].harmless_absorb_nb(ten1,absorb_fun,ind_pairs,
				[&group2](auto & eg){return group2.count(eg.second.nbkey)==1;});
		return contract_fun(ten1,ten2,ind_pairs);
	}

	template<typename NodeVal1,typename EdgeVal1,typename NodeVal2,typename EdgeVal2, typename NodeKey, typename EdgeKey, typename Trait>
	network<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait> fmap(const network<NodeVal1,EdgeVal1,NodeKey,EdgeKey,Trait> & n,
		std::function<NodeVal2(const NodeVal1 &)> f1,std::function<EdgeVal2(const EdgeVal1 &)> f2){

		network<NodeVal2,EdgeVal2,NodeKey,EdgeKey> result(n.name);
		for (auto & s:n.nodes)
			result.nodes.insert({s.first,fmap(s.second,f1,f2)});
		for (auto & s:result.nodes)
			s.second.relink(result.nodes);
		return result;
	}

	template<typename NodeVal1,typename EdgeVal1,typename NodeVal2,typename EdgeVal2, typename NodeKey, typename EdgeKey, typename Trait>
	network<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait> fmap(const network<NodeVal1,EdgeVal1,NodeKey,EdgeKey,Trait> & n,
		std::function<NodeVal2(const NodeVal1 &)> f1,std::function<EdgeVal2(const EdgeVal1 &)> f2,std::function<NodeKey(const NodeKey &)> f3,
		std::function<EdgeKey(const EdgeKey &)> f4){

		network<NodeVal2,EdgeVal2,NodeKey,EdgeKey> result(n.name);
		for (auto & s:n.nodes)
			result.nodes.insert({f3(s.first),fmap(s.second,f1,f2,f3,f4)});
		for (auto & s:result.nodes)
			s.second.relink(result.nodes);
		return result;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::fope(std::function<NodeVal(const NodeVal &)> f1,
		std::function<EdgeVal(const EdgeVal &)> f2){

		for (auto & s:nodes)
			fope(s.second,f1,f2);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	bool network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::consistency(){
		return consistency(std::cout);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	bool network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::consistency(std::ostream & diagnosis){
		for (auto & s:nodes)
			if(!s.second.consistency(nodes,diagnosis)){
				diagnosis<<"Error at node "<<s.first<<'\n';
				return false;
			}
		return true;
	}


}
#endif
