#ifndef NET_NODE_HPP
#define NET_NODE_HPP

#include <map>
#include <string>
#include <vector>
#include "error.hpp"
#include "network.hpp"

namespace net{
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	class node;
	template <typename NodeVal,typename EdgeVal,typename EdgeKey>
	using absorb_type = std::function<NodeVal(const NodeVal&,const EdgeVal&,const EdgeKey &)>;
	template <typename NodeVal,typename EdgeKey,typename Comp>
	using contract_type = std::function<NodeVal(const NodeVal&,const NodeVal&,const std::set<std::pair<EdgeKey,EdgeKey>,Comp> &)>;

	template<typename T>
	std::string to_string(const T& m);

   /**
    * \brief 描述着连接格点的边上信息，实际上这是一个半边而不是是一个完整的边
    *
    * \see node
    */
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	class edge{
	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::ostream & output_edge_text(std::ostream &, const edge<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::istream & input_edge_text(std::istream &, edge<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::ostream & output_edge_bin(std::ostream &, const edge<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::istream & input_edge_bin(std::istream &, edge<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);
	public:
        /**
         * \brief 所指向的格点的名称
         */
		NodeKey nbkey;
		/**
		 * \brief 所指向格点连接自身的边的名称
		 */
		EdgeKey nbind;
        /**
         * \brief 所指向的格点的指针
         */
		node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * nbnode;
		/**
		 * \brief 边上的附着信息
		 */
		EdgeVal val;

		edge()=default;
		edge(const NodeKey & s1,const EdgeKey & s2,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * s):
			nbkey(s1),nbind(s2),nbnode(s){};
		edge(const NodeKey & s1,const EdgeKey & s2,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * s, const EdgeVal & E):
			nbkey(s1),nbind(s2),nbnode(s),val(E){};
		edge(const edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&)=default;
	};

   /**
    * \brief 格点存储了网络中点内的信息
    *
    * 每个格点中有一个中心元素val, 和边的信息
    * 每个边拥有一个名字和一个edge对象
    * \see network
    * \see edge
    */
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	class node{
	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::ostream & output_node_text(std::ostream &, const node<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::istream & input_node_text(std::istream &, node<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::ostream & output_node_bin(std::ostream &, const node<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::istream & input_node_bin(std::istream &, node<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeVal2,typename EdgeVal2, typename NodeKey1, typename EdgeKey1, typename Trait1>
	friend node<NodeVal2,EdgeVal2,NodeKey1,EdgeKey1,Trait1> fmap(const node<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> & n,
		std::function<NodeVal2(const NodeVal1 &)> f1,std::function<EdgeVal2(const EdgeVal1 &)> f2);

	template<typename NodeVal1,typename EdgeVal1,typename NodeVal2,typename EdgeVal2, typename NodeKey1, typename EdgeKey1, typename Trait1>
	friend node<NodeVal2,EdgeVal2,NodeKey1,EdgeKey1,Trait1> fmap(const node<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> & n,
		std::function<NodeVal2(const NodeVal1 &)> f1,std::function<EdgeVal2(const EdgeVal1 &)> f2,std::function<NodeKey1(const NodeKey1 &)> f3,
		std::function<EdgeKey1(const EdgeKey1 &)> f4);
	public:
		//constructor
		node()=default;
		node(const NodeVal& s):val(s){};
		//copy constructor
		node(const node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&)=default;
		//copy assignment
		node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& operator=(const node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&)=default;
		//move constructor
		node(node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&&)=default;
		//move assignment
		node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& operator=(node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&&)=default;

		void clean();

		template <typename Condition>
		void delete_edge(Condition &&);
		void delete_nbedge();

		void reset_nbkey_of_nb(const NodeKey &);

		void absorb_nb(const NodeKey &,const NodeVal &, absorb_type<NodeVal,EdgeVal,EdgeKey>,
			contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less>);
		template <typename Condition>
		void harmless_absorb_nb(NodeVal &,absorb_type<NodeVal,EdgeVal,EdgeKey>, std::set<std::pair<EdgeKey,EdgeKey>,
			typename Trait::edge2key_less>,Condition&&);

		void transfer_edge(const NodeKey &,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> &,bool );
		template <typename Condition>
		void transfer_edge(const NodeKey &,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> &,bool ,Condition &&);
		template <typename Condition>
		void transfer_edge(const NodeKey &,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> &,
			const NodeKey &,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> &,bool ,Condition &&);

		void set_edge(const EdgeKey & ind,const NodeKey & nbkey, const EdgeKey & nbind,
			node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * nbnode, const EdgeVal & edgev);

		void relink(const std::map<NodeKey,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>,typename Trait::nodekey_less> &);

		bool consistency(const std::map<NodeKey,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>,typename Trait::nodekey_less> &,std::ostream &);

		void fope(std::function<NodeVal(const NodeVal &)> f1, std::function<EdgeVal(const EdgeVal &)> f2);
		/**
		 * \brief 格点所附着的信息
		 */
		NodeVal val;
		/**
		 * \brief 格点所相连的边, 存储了另一测的指针等信息
		 * \see edge
		 */
		std::map<EdgeKey,edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>,typename Trait::edgekey_less> edges;
	};

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	template <typename Condition>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::delete_edge(Condition&& cond){

		for(auto edge_itr=edges.begin(); edge_itr != edges.end();) {
			if (cond(edge_itr)) {
				edge_itr->second.nbnode.edges.erase(edge_itr->second.nbind);
				edge_itr=edges.erase(edge_itr);
			} else {
				++edge_itr;
			}
		}
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::delete_nbedge(){

		for(auto & b: edges){
			b.second.nbnode->edges.erase(b.second.nbind);
		}
	}

	/*
	* \brief 对格点的每一条边，更新邻居对应的边的nbkey为newkey
	*/
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::reset_nbkey_of_nb(const NodeKey & newkey){

		for(auto & b: edges){
			b.second.nbnode->edges[b.second.nbind].nbkey=newkey;
		}
	}

	/*
	* \brief 对格点的指定nbkey每一条边，记录这条边的(ind,nbind)，吸收边的val到格点的val，然后删掉这条边。
	*
	*
	*  A(this) <----> B(nb) <----> C   =>  A <---- B <----> C 
	*/
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::absorb_nb(const NodeKey & nbkey,const NodeVal & nbval,
		absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun){

		std::set<std::pair<EdgeKey,EdgeKey>,typename Trait::edge2key_less> ind_pairs;

		// set ind_pairs and erase iterator in node1
		for(auto iter=edges.begin(); iter != edges.end(); ) {
			if (iter->second.nbkey==nbkey) {
				ind_pairs.insert(std::make_pair(iter->first,iter->second.nbind));
				val=absorb_fun(val,iter->second.val,iter->first);
				iter=edges.erase(iter);
			} else {
				++iter;
			}
		}
		val=contract_fun(val,nbval,ind_pairs);
	}

	/*
	* \brief 对格点的指定nbkey每一条边，如果符合条件，记录这条边的(ind,nbind)，吸收边的val到给定的的val，将ind-pair填入ind_pairs
	*
	*/
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	template <typename Condition>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::harmless_absorb_nb(NodeVal & thisval, absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun,
		std::set<std::pair<EdgeKey,EdgeKey>,typename Trait::edge2key_less> ind_pairs,Condition&& cond){

		for(auto & b:edges) {
			if (cond(b)) {
				ind_pairs.insert(std::make_pair(b.first,b.second.nbind));
				thisval=absorb_fun(thisval,b.second.val,b.first);
			}
		}
	}

	/*
	* \brief 对格点的每一条边，将它和邻居的这条边转移为格点(newkey,newnode)和邻居的边
	*
	*/

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::transfer_edge(const NodeKey & newkey,
		node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & newnode, bool delete_flag){

		for(auto & b: edges){
			newnode.edges[b.first]=b.second;
			b.second.nbnode->edges[b.second.nbind].nbkey=newkey;
			b.second.nbnode->edges[b.second.nbind].nbnode=&newnode;
		}
		if(delete_flag) edges.clear();
	}

	/*
	* \brief 对格点的每一条边，如果条件成立，将它和邻居的这条边转移为格点(newkey,newnode)和邻居的边
	*
	*/

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	template <typename Condition>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::transfer_edge(const NodeKey & newkey,
		node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & newnode, bool delete_flag ,Condition&& cond){

		for(auto iter=edges.begin(); iter != edges.end(); ) {
			if (cond(iter)) {
				newnode.edges[iter->first]=iter->second;
				auto & nb_edge =iter->second.nbnode->edges[iter->second.nbind];
				nb_edge.nbkey=newkey;
				nb_edge.nbnode=&newnode;
				if(delete_flag)
					iter=edges.erase(iter);
				else
					++iter;
			} else {
				++iter;
			}
		}
	}

	/*
	* \brief 对格点的每一条边，如果条件成立，将它和邻居的这条边转移为格点(newkey,newnode)和邻居的边
	*                       否则，将它和邻居的这条边转移为格点(newkey2,newnode2)和邻居的边
	*
	*/
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	template <typename Condition>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::transfer_edge(const NodeKey & newkey,
		node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & newnode,const NodeKey & newkey2,
		node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> & newnode2, bool delete_flag ,Condition&& cond){

		for(auto iter=edges.begin(); iter != edges.end(); ) {
			if (cond(iter)) {
				newnode.edges[iter->first]=iter->second;
				auto & nb_edge =iter->second.nbnode->edges[iter->second.nbind];
				nb_edge.nbkey=newkey;
				nb_edge.nbnode=&newnode;
				if(delete_flag)
					iter=edges.erase(iter);
				else
					++iter;
			} else {
				newnode2.edges[iter->first]=iter->second;
				auto & nb_edge =iter->second.nbnode->edges[iter->second.nbind];
				nb_edge.nbkey=newkey2;
				nb_edge.nbnode=&newnode2;
				if(delete_flag)
					iter=edges.erase(iter);
				else
					++iter;
			}
		}

	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_edge(const EdgeKey & ind,const NodeKey & nbkey,
		const EdgeKey & nbind, node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * nbnode,const EdgeVal & edgev){
		auto [s1,succ1]=edges.insert(make_pair(ind,edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(nbkey,nbind,nbnode,edgev)));
		if(!succ1)throw key_exist_error("In node.add_edge, ind "+to_string(ind)+" already linked!");
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::relink(const std::map<NodeKey,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>,typename Trait::nodekey_less> & nodes){
		for (auto & b: edges)
			b.second.nbnode=&(nodes[b.second.nbkey]);
	}

	template<typename NodeVal1,typename EdgeVal1,typename NodeVal2,typename EdgeVal2, typename NodeKey, typename EdgeKey, typename Trait>
	node<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait> fmap(const node<NodeVal1,EdgeVal1,NodeKey,EdgeKey,Trait> & n,
		std::function<NodeVal2(const NodeVal1 &)> f1,std::function<EdgeVal2(const EdgeVal1 &)> f2){

		node<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait> result;
		result.val=f1(n.val);
		for (auto & b:n.edges)
			result.edges[b.first]=edge<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait>
				(b.second.nbkey,b.second.nbind,nullptr,f2(b.second.val));
		return result;
	}

	template<typename NodeVal1,typename EdgeVal1,typename NodeVal2,typename EdgeVal2, typename NodeKey, typename EdgeKey, typename Trait>
	node<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait> fmap(const node<NodeVal1,EdgeVal1,NodeKey,EdgeKey,Trait> & n,
		std::function<NodeVal2(const NodeVal1 &)> f1,std::function<EdgeVal2(const EdgeVal1 &)> f2,std::function<NodeKey(const NodeKey &)> f3,
		std::function<EdgeKey(const EdgeKey &)> f4){

		node<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait> result;
		result.val=f1(n.val);
		for (auto & b:n.edges)
			result.edges[f4(b.first)]=edge<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait>
				(f3(b.second.nbkey),f4(b.second.nbind),nullptr,f2(b.second.val));
		return result;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::fope(std::function<NodeVal(const NodeVal &)> f1,
		std::function<EdgeVal(const EdgeVal &)> f2){
		
		val=f1(val);
		for (auto & b:edges)
			b.second.val=f2(b.second.val);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	bool node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::consistency(const std::map<NodeKey,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>,typename Trait::nodekey_less> & nodes,
		std::ostream & diagnosis){

		for (auto & b:edges){ // check if b is consistent
			if(nodes.count(b.second.nbkey)==0){
				diagnosis<<"Network is not consistent, neighbor "+to_string(b.second.nbkey)+" is not found!\n";
				return false;
			}else if(nodes[b.second.nbkey].edges.count(b.second.nbind)==0){
				diagnosis<<"Network is not consistent, neighbor "+to_string(b.second.nbkey)+" has no index named "+to_string(b.second.nbind)+" !\n";
				return false;
			}else if(b.second.nbnode != &(nodes[b.second.nbkey])){
				diagnosis<<"Network is not consistent, pointer to neighbor "+to_string(b.second.nbkey)+" is not correctly pointed !\n";
				return false;
			}
		}
		
		return true;
	}
}


#endif
