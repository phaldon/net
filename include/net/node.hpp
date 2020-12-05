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
		void delete_edge_with_nbkey(const NodeKey &);
		void delete_edge_with_ind(const EdgeKey &);
		void reset_nbkey_of_nb(const NodeKey & newkey);
		void absorb_nb(const NodeKey & nbkey,const NodeVal & nbval, absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun,
			contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun);
		void transfer_edge(const NodeKey & newkey,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * newnode);
		void set_edge(const EdgeKey & ind,const NodeKey & nbkey, const EdgeKey & nbind, node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * nbnode);
		/**
		 * \brief 格点所附着的信息
		 */
		NodeVal val;
		/**
		 * \brief 格点所相连的边, 存储了另一测的指针等信息
		 * \see edge
		 */
		std::map<EdgeKey,edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>,
			typename Trait::edgekey_less> edges;
	};

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::delete_edge_with_nbkey(const NodeKey & s){

		for(auto edge_itr=edges.begin(); edge_itr != edges.end();) {
			//edge1=edge_itr->second
			if (edge_itr->second.nbkey==s) {
				edge_itr->second.nbnode.edges.erase(edge_itr->second.nbind);
				edge_itr=edges.erase(edge_itr);
			} else {
				++edge_itr;
			}
		}
	}

	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::delete_edge_with_ind(const EdgeKey & ind){

		auto edge_itr = edges.find(ind);
		if(edge_itr == edges.end()){
			throw key_unfound_error("In edge.delete_edge_with_ind, leg "+to_string(ind)+" is not found!");
		}
		edge_itr->second.nbnode->edges.erase(edge_itr->second.nbind);
		edges.erase(edge_itr);
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
	* \brief 对格点的每一条边，当边的nbkey!=newkey时，将它和邻居的这条边转移为格点(newkey,newnode)和邻居的边
	*
	*
	*  A(newkey,newnode) <----> B(this) <----> C   =>  A <----> B ----> C 
	*                                                  ^                ^
	*                                                  |                |
	*                                                   ----------------
	*/
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::transfer_edge(const NodeKey & newkey,
		node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * newnode){

		for(auto & b: edges){
			if(b.second.nbkey!=newkey){
				newnode->edges[b.first]=b.second;
				b.second.nbnode->edges[b.second.nbind].nbkey=newkey;
				b.second.nbnode->edges[b.second.nbind].nbnode=newnode;
			}
		}
	}


	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	void node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_edge(const EdgeKey & ind,const NodeKey & nbkey,
		const EdgeKey & nbind, node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * nbnode){
		auto [s1,succ1]=edges.insert(make_pair(ind,edge<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(nbkey,nbind,nbnode)));
		if(!succ1)throw key_exist_error("In node.add_edge, ind "+to_string(ind)+" already linked!");
	}
}


#endif
