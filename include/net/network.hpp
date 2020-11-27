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

namespace net{

	/**
	* \brief 格点的分解函数的类型
	*/
	template <typename NodeVal,typename EdgeVal,typename EdgeKey,typename Comp>
	using dec_type = std::function<void(NodeVal&,NodeVal&,NodeVal&,
		const std::set<EdgeKey,Comp> &,const EdgeKey & ,const EdgeKey & ,EdgeVal&)>;

	/**
	* \brief 格点的吸收函数的类型
	*/
	template <typename NodeVal,typename EdgeVal,typename EdgeKey>
	using absorb_type = std::function<void(NodeVal&,EdgeVal&,const EdgeKey &)>;

	/**
	* \brief 格点的缩并函数的类型
	*/
	template <typename NodeVal,typename EdgeKey,typename Comp>
	using contract_type = std::function<NodeVal(NodeVal&,NodeVal&,const std::set<std::pair<EdgeKey,EdgeKey>,Comp> &)>;

	/**
	* \brief 格点信息的初始化函数的类型
	*/
	template <typename NodeVal,typename EdgeKey>
	using init_node_type = std::function<NodeVal(const std::vector<EdgeKey> &)>;

	/**
	* \brief 边信息的初始化函数的类型
	*/
	template <typename EdgeVal,typename EdgeKey>
	using init_edge_type = std::function<EdgeVal(const EdgeKey &,const EdgeKey &)>;

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
		* \brief 加一条边
		*/
		void set_edge(const NodeKey &,const NodeKey &,const EdgeKey &,const EdgeKey &);
		void set_edge(IterNode,IterNode,const EdgeKey &,const EdgeKey &);
		/**
		* \brief 加一条边。根据格点名字和Trait::bind自动生成边的名字
		*/
		void set_edge(const NodeKey &,const NodeKey &);
		void set_edge(IterNode,IterNode);

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
		IterNode rename(IterNode &,const NodeKey &);
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
		/**
		* \brief 初始化网络的边的信息
		*/
		void init_edges(init_edge_type<EdgeVal,EdgeKey> );
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
		for(auto & s:nodes){
			for (auto & b: s.second.edges){
				b.second.nbnode=&(nodes[b.second.nbkey]);
			}
		}
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
		// this is not default because nodes[i].edges[j].neighbor needs redirection
	network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::operator=
		(const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& N){
		if(this != &N){
			nodes=N.nodes;
			name=N.name;
			for(auto & s:nodes){
				for (auto & b: s.second.edges){
					b.second.nbnode=&(nodes[b.second.nbkey]);
				}
			}
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
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::init_edges(init_edge_type<EdgeVal,EdgeKey> init_fun){

		for (auto& node:nodes){
			for (auto& b:node.second.edges){
				b.second.val=init_fun(b.first,b.second.ind);
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
	typename network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::add(const NodeKey & nodekey){
		auto [s1,succ1]=nodes.insert(make_pair(nodekey,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>()));
		if(!succ1){
			throw key_exist_error("In network.add, node "+to_string(nodekey)+" already exists!");
		}
		return s1;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del(const NodeKey & nodekey){

		auto node_itr = nodes.find(nodekey);
		if(node_itr == nodes.end()){
			throw key_unfound_error("In network.del, node "+to_string(nodekey)+" is not found!");
		}

		// node = node_itr->second
		for(auto & b: node_itr->second.edges){ 
			// edge = b.second, nbnode = *(b.second.nbnode)
			b.second.nbnode->edges.erase(b.second.nbind); 
		}
		nodes.erase(node_itr);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it){

		for(auto & b: it->second.edges){
			b.second.nbnode->edges.erase(b.second.nbind);
		}
		nodes.erase(it);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del_edge(const NodeKey & nodekey1,const NodeKey& nodekey2){

		auto node_itr1 = nodes.find(nodekey1);
		if(node_itr1 == nodes.end()){
			throw key_unfound_error("In network.del_edge, node "+to_string(nodekey1)+" is not found!");
		}

		auto node_itr2 = nodes.find(nodekey2);
		if(node_itr2 == nodes.end()){
			throw key_unfound_error("In network.del_edge, node "+to_string(nodekey2)+" is not found!");
		}

		del_edge(node_itr1,node_itr2);

	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del_edge (network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it1,
		network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it2){

		// node1 = it1->second, node2 = it2->second
		for(auto edge_itr=it1->second.edges.begin(); edge_itr != it1->second.edges.end();) {
			//edge1=edge_itr->second
			if (edge_itr->second.nbkey==it2->first) {
				it2->second.edges.erase(edge_itr->second.nbind);
				edge_itr=it1->second.edges.erase(edge_itr);
			} else {
				++edge_itr;
			}
		}
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del_leg(const NodeKey & nodekey,const EdgeKey & ind){

		auto node_itr = nodes.find(nodekey);
		if(node_itr == nodes.end()){
			throw key_unfound_error("In network.del_leg, node "+to_string(nodekey)+" is not found!");
		}

		auto edge_itr = node_itr->second.edges.find(ind);
		if(edge_itr == node_itr->second.edges.end()){
			throw key_unfound_error("In network.del_leg, leg "+to_string(ind)+" is not found!");
		}

		// nbnode= *(edge_itr->second.nbnode)
		edge_itr->second.nbnode->edges.erase(edge_itr->second.nbind);
		node_itr->second.edges.erase(ind);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del_leg(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it,const EdgeKey & ind){

		auto edge_itr = it->second.edges.find(ind);
		if(edge_itr == it->second.edges.end()){
			throw key_unfound_error("In network.del_leg, leg "+to_string(ind)+" is not found!");
		}

		// nbnode= *(edge_itr->second.nbnode)
		edge_itr->second.nbnode->edges.erase(edge_itr->second.nbind);
		it->second.edges.erase(ind);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	typename network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::rename
		(const NodeKey & old_key,const NodeKey & new_key){

		auto node_handle = nodes.extract(old_key);
		if(node_handle.empty()){
			throw key_unfound_error("In network.rename, node "+to_string(old_key)+" is not found!");
		}
		node_handle.key=new_key;
		
		auto status = nodes.insert(std::move(node_handle));
		if(!status.inserted)throw key_exist_error("In network.rename, node "+to_string(new_key)+" already exists!");

		auto it=status.position;
		for(auto & b: it->second.edges){
			// nbnode= *(b.second.nbnode)
			auto & nbedge=b.second.nbnode->edges[b.second.nbind];
			nbedge.nbkey=new_key;
			nbedge.nbnode=&(it->second);
		}
		return it;
	}


	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	typename network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::rename
		(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode & it,const NodeKey & new_key){

		auto node_handle = nodes.extract(it);
		if(node_handle.empty()){
			throw key_unfound_error("In network.rename, node "+to_string(it->first)+" is not found!");
		}
		node_handle.key=new_key;
		
		auto status = nodes.insert(std::move(node_handle));
		if(!status.inserted)throw key_exist_error("In network.rename, node "+to_string(new_key)+" already exists!");
		
		it=status.position;
		for(auto & b: it->second.edges){
			// nbnode= *(b.second.nbnode)
			auto & nbedge=b.second.nbnode->edges[b.second.nbind];
			nbedge.nbkey=new_key;
			nbedge.nbnode=&(it->second);
		}
		return it;
	}


	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	typename network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::rename
		(const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode & it,const NodeKey & new_key){

		auto node_handle = nodes.extract(it);
		if(node_handle.empty()){
			throw key_unfound_error("In network.rename, node "+to_string(it->first)+" is not found!");
		}
		node_handle.key=new_key;
		
		auto status = nodes.insert(std::move(node_handle));
		if(!status.inserted)throw key_exist_error("In network.rename, node "+to_string(new_key)+" already exists!");
		
		auto newit=status.position;
		for(auto & b: newit->second.edges){
			// nbnode= *(b.second.nbnode)
			auto & nbedge=b.second.nbnode->edges[b.second.nbind];
			nbedge.nbkey=new_key;
			nbedge.nbnode=&(newit->second);
		}
		return newit;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_edge(const NodeKey & nodekey1,const NodeKey& nodekey2, const EdgeKey & ind1,const EdgeKey& ind2){

		auto node_itr1 = nodes.find(nodekey1);
		if(node_itr1 == nodes.end()){
			throw key_unfound_error("In network.set_edge, node "+to_string(nodekey1)+" is not found!");
		}
		auto node_itr2 = nodes.find(nodekey2);
		if(node_itr2 == nodes.end()){
			throw key_unfound_error("In network.set_edge, node "+to_string(nodekey2)+" is not found!");
		}

		auto [s1,succ1]=node_itr1->second.edges.insert(make_pair(ind1,edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(nodekey2,ind2,&(node_itr2->second))));
		if(!succ1)throw key_exist_error("In network.set_edge, ind "+to_string(ind1)+" of node "+to_string(nodekey1)+" already linked!");
		auto [s2,succ2]=node_itr2->second.edges.insert(make_pair(ind2,edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(nodekey1,ind1,&(node_itr1->second))));
		if(!succ2)throw key_exist_error("In network.set_edge, ind "+to_string(ind2)+" of node "+to_string(nodekey2)+" already linked!");
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_edge(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it1,
		network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it2, const EdgeKey & ind1,const EdgeKey& ind2){
		
		auto [s1,succ1]=it1->second.edges.insert(make_pair(ind1,edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(it2->first,ind2,&(it2->second))));
		if(!succ1)throw key_exist_error("In network.set_edge, ind "+to_string(ind1)+" of node "+to_string(it1->first)+" already linked!");
		auto [s2,succ2]=it2->second.edges.insert(make_pair(ind2,edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(it1->first,ind1,&(it1->second))));
		if(!succ2)throw key_exist_error("In network.set_edge, ind "+to_string(ind2)+" of node "+to_string(it2->first)+" already linked!");
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_edge(const NodeKey & nodekey1,const NodeKey& nodekey2){
		set_edge(nodekey1,nodekey2,Trait::bind(nodekey1,nodekey2),Trait::bind(nodekey2,nodekey1));
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_edge(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it1,
		network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it2){
		set_edge(it1,it2,Trait::bind(it1->first,it2->first),Trait::bind(it2->first,it1->first));
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

		auto & node1=node_itr1->second;
		auto & node2=node_itr2->second;

		std::set<std::pair<EdgeKey,EdgeKey>,typename Trait::edge2key_less> ind_pairs;

		// set ind_pairs and erase iterator in node1
		for(auto iter=node1.edges.begin(); iter != node1.edges.end(); ) {
			if (iter->second.nbkey==nodekey2) {
				ind_pairs.insert(std::make_pair(iter->first,iter->second.nbind));
				absorb_fun(node1.val,iter->second.val,iter->first);
				iter=node1.edges.erase(iter);
			} else {
				++iter;
			}
		}

		node1.val=contract_fun(node1.val,node2.val,ind_pairs);

		for(auto & b:node2.edges){
			if(b.second.nbkey != nodekey1){
				node1.edges[b.first]=b.second;
				auto &nb_edge =b.second.nbnode->edges[b.second.nbind];
				nb_edge.nbkey=nodekey1;
				nb_edge.nbnode=&node1;
			}
		}

		nodes.erase(node_itr2);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::absorb(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode node_itr1,
		network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode node_itr2,
		absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun){

		auto & node1=node_itr1->second;
		auto & node2=node_itr2->second;

		std::set<std::pair<EdgeKey,EdgeKey>,typename Trait::edge2key_less> ind_pairs;

		// set ind_pairs and erase iterator in node1
		for(auto iter=node1.edges.begin(); iter != node1.edges.end(); ) {
			if (iter->second.nbkey==node_itr2->first) {
				ind_pairs.insert(std::make_pair(iter->first,iter->second.nbind));
				absorb_fun(node1.val,iter->second.val,iter->first);
				iter=node1.edges.erase(iter);
			} else {
				++iter;
			}
		}

		node1.val=contract_fun(node1.val,node2.val,ind_pairs);

		for(auto & b:node2.edges){
			if(b.second.nbkey != node_itr1->first){
				node1.edges[b.first]=b.second;
				auto &nb_edge =b.second.nbnode->edges[b.second.nbind];
				nb_edge.nbkey=node_itr1->first;
				nb_edge.nbnode=&node1;
			}
		}

		nodes.erase(node_itr2);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::split(const NodeKey & nodekey1, const NodeKey & nodekey2,const NodeKey & nodekey3
		,const std::set<EdgeKey,typename Trait::edgekey_less> & inds, const EdgeKey & ind2, const EdgeKey & ind3,
		dec_type<NodeVal,EdgeVal,EdgeKey,typename Trait::edgekey_less> dec_fun){

		auto s1=nodes.find(nodekey1);
		auto &b1=s1->second.edges;
		auto [s2,succ2]=nodes.insert(nodekey2);
		auto &b2=s2->second.edges;
		auto [s3,succ3]=nodes.insert(nodekey3);
		auto &b3=s3->second.edges;

		for(auto & b:b1){
			if(inds.count(b.first)==0){
				b2[b.first]=b.second;
				auto &nb_edge =b.second.nbnode->edges[b.second.nbind];
				nb_edge.nbkey=s2->first;
				nb_edge.nbnode=&(s2->second);
			}else{
				b3[b.first]=b.second;
				auto &nb_edge =b.second.nbnode->edges[b.second.nbind];
				nb_edge.nbkey=s3->first;
				nb_edge.nbnode=&(s3->second);
			}
		}

		EdgeVal env;
		dec_fun(s1->val,s2->val,s3->val,inds,ind2,ind3,env);
		b2[ind2]=edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(nodekey3,ind3,&(s3->second),env);
		b3[ind3]=edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(nodekey2,ind2,&(s2->second),env);

		nodes.erase(s1);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::split(const NodeKey & nodekey1, const NodeKey & nodekey2,
		const std::set<EdgeKey,typename Trait::edgekey_less> & inds, const EdgeKey & ind1, const EdgeKey & ind2,
		dec_type<NodeVal,EdgeVal,EdgeKey,typename Trait::edgekey_less> dec_fun){

		auto s1=nodes.find(nodekey1);
		auto &b1=s1->second.edges;
		auto [s2,succ2]=nodes.insert(nodekey2);
		auto &b2=s2->second.edges;

		for(auto iter=b1.begin(); iter != b1.end(); ) {
			if (inds.count(iter->first)==1) {
				b2[iter->first]=iter->second;
				auto & nb_edge =iter->second.nbnode->edges[iter->second.nbind];
				nb_edge.nbkey=s2->first;
				nb_edge.nbnode=&(s2->second);
				iter=b1.erase(iter);
			} else {
				++iter;
			}
		}

		auto temp=s1->val;
		EdgeVal env;
		dec_fun(temp,s1->val,s2->val,inds,ind1,ind2,env);
		b1[ind1]=edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(nodekey2,ind2,&(s2->second),env);
		b2[ind2]=edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(nodekey1,ind1,&(s1->second),env);
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

			for(auto & b:node_itr1->second.edges) {
				//std::cout<<b.first<<b.second.name<<b.second.ind<<'\n';
				if (group.count(b.second.nbkey)==1) {
					ind_pairs.insert(std::make_pair(b.first,b.second.nbind));
					absorb_fun(node_t,b.second.val,b.first);
				}
			}

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

			for(auto & b:it1->second.edges) {
				//std::cout<<b.first<<b.second.name<<b.second.ind<<'\n';
				if (group.count(b.second.nbkey)==1) {
					ind_pairs.insert(std::make_pair(b.first,b.second.nbind));
					absorb_fun(node_t,b.second.val,b.first);
				}
			}

			ten=contract_fun(node_t,ten,ind_pairs);
		}
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	NodeVal network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::tn_contract2(const std::set<NodeKey,typename Trait::nodekey_less> & group1, NodeVal& ten1,
		const std::set<NodeKey,typename Trait::nodekey_less> & group2, NodeVal& ten2,
		absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun){

		if (group1.size()==0){
			return ten2;
		}
		if (group2.size()==0){
			return ten1;
		}
		std::set<std::pair<EdgeKey,EdgeKey>,typename Trait::edge2key_less> ind_pairs;
		for (auto & nk:group1){
			auto & node=nodes[nk];
			for(auto & b:node.edges) {
				if (group2.count(b.second.nbkey)==1) {
					ind_pairs.insert(std::make_pair(b.first,b.second.ind));
					absorb_fun(ten1,b.second.val,b.first);
				}
			}

		}
		return contract_fun(ten1,ten2,ind_pairs);
	}

	template<typename NodeVal1,typename EdgeVal1,typename NodeVal2,typename EdgeVal2, typename NodeKey, typename EdgeKey, typename Trait>
	network<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait> fmap(const network<NodeVal1,EdgeVal1,NodeKey,EdgeKey,Trait> & n,
		std::function<NodeVal2(const NodeVal1 &)> f1,std::function<EdgeVal2(const EdgeVal1 &)> f2){
		network<NodeVal2,EdgeVal2,NodeKey,EdgeKey> result;
		result.name=n.name;
		for (auto & s:n.nodes){
			const auto [its, success] = result.nodes.insert({s.first,node<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait>()});
			its->second.val=f1(s.second.val);
			for (auto & b:s.second.edges){
				its->second.edges[b.first]=edge<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait>
					(b.second.nbkey,b.second.nbind,nullptr,f2(b.second.val));
			}
		}
		for (auto & s:result.nodes){
			for (auto & b:s.second.edges){
				b.second.nbnode=&(result.nodes[b.second.nbkey]);
			}
		}
		return result;
	}

	template<typename NodeVal1,typename EdgeVal1,typename NodeVal2,typename EdgeVal2, typename NodeKey, typename EdgeKey, typename Trait>
	network<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait> fmap(const network<NodeVal1,EdgeVal1,NodeKey,EdgeKey,Trait> & n,
		std::function<NodeVal2(const NodeVal1 &)> f1,std::function<EdgeVal2(const EdgeVal1 &)> f2,std::function<NodeKey(const NodeKey &)> f3,
		std::function<EdgeKey(const EdgeKey &)> f4){
		network<NodeVal2,EdgeVal2,NodeKey,EdgeKey> result;
		result.name=n.name;
		for (auto & s:n.nodes){
			const auto [its, success] = result.nodes.insert({f3(s.first),node<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait>()});
			its->second.val=f1(s.second.val);
			for (auto & b:s.second.edges){
				its->second.edges[f4(b.first)]=edge<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait>
					(f3(b.second.nbkey),f4(b.second.nbind),nullptr,f2(b.second.val));
			}
		}
		for (auto & s:result.nodes){
			for (auto & b:s.second.edges){
				b.second.nbnode=&(result.nodes[b.second.nbkey]);
			}
		}
		return result;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::fope(std::function<NodeVal(const NodeVal &)> f1,
		std::function<EdgeVal(const EdgeVal &)> f2){
		for (auto & s:nodes){
			s.second.val=f1(s.second.val);
			for (auto & b:s.second.edges){
				b.second.val=f2(b.second.val);
			}
		}
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	bool network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::consistency(){
		return consistency(std::cout);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	bool network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::consistency(std::ostream & diagnosis){
		for (auto & s:nodes){
			for (auto & b:s.second.edges){ // check if b is consistent
				if(nodes.count(b.second.nbkey)==0){
					diagnosis<<"Network is not consistent, neighbor of node "+to_string(s.first)+
					+" : "+to_string(b.second.nbkey)+" is not found!\n";
					return false;
				}else if(nodes[b.second.nbkey].edges.count(b.second.nbind)==0){
					diagnosis<<"Network is not consistent, neighbor of node "+to_string(s.first)+
					+" : "+to_string(b.second.nbkey)+" has not index named "+to_string(b.second.nbind)+" !\n";
					return false;
				}else if(b.second.nbnode != &(nodes[b.second.nbkey])){
					diagnosis<<"Network is not consistent, pointer to neighbor of node "+to_string(s.first)+
					+" : "+to_string(b.second.nbkey)+" is not correctly pointed !\n";
					return false;
				}
			}
		}
		return true;
	}


}
#endif
