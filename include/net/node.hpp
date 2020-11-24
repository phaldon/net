#ifndef NET_NODE_HPP
#define NET_NODE_HPP

#include <map>
#include <string>
#include <vector>

namespace net{
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	class node;

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
		NodeKey name;
		/**
		 * \brief 所指向格点连接自身的边的名称
		 */
		EdgeKey ind;
		node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * neighbor;
		/**
		 * \brief 边上的附着信息
		 */
		EdgeVal val;

		edge()=default;
		edge(const NodeKey & s1,const EdgeKey & s2,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * s):
			name(s1),ind(s2),neighbor(s){};
		edge(const NodeKey & s1,const EdgeKey & s2,node<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * s, const EdgeVal & E):
			name(s1),ind(s2),neighbor(s),val(E){};
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
}


#endif
