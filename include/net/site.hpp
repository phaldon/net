#ifndef NET_SITE_HPP
#define NET_SITE_HPP

#include <map>
#include <string>
#include <vector>

namespace net{
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	class site;

   /**
    * \brief 描述着连接格点的边上信息，实际上这是一个半边而不是是一个完整的边
    *
    * \see site
    */
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	class bond{
	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::ostream & output_bond_text(std::ostream &, const bond<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::istream & input_bond_text(std::istream &, bond<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::ostream & output_bond_bin(std::ostream &, const bond<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::istream & input_bond_bin(std::istream &, bond<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);
	public:
        /**
         * \brief 所指向的格点的名称
         */
		NodeKey name;
		/**
		 * \brief 所指向格点连接自身的边的名称
		 */
		EdgeKey ind;
		site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * neighbor;
		/**
		 * \brief 边上的附着信息
		 */
		EdgeVal val;

		bond()=default;
		bond(const NodeKey & s1,const EdgeKey & s2,site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * s):
			name(s1),ind(s2),neighbor(s){};
		bond(const NodeKey & s1,const EdgeKey & s2,site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * s, const EdgeVal & E):
			name(s1),ind(s2),neighbor(s),val(E){};
		bond(const bond<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&)=default;
	};

   /**
    * \brief 格点存储了网络中点内的信息
    *
    * 每个格点中有一个中心元素val, 和边的信息
    * 每个边拥有一个名字和一个bond对象
    * \see network
    * \see bond
    */
	template <typename NodeVal, typename EdgeVal, typename NodeKey, typename EdgeKey, typename Trait>
	class site{
	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::ostream & output_site_text(std::ostream &, const site<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::istream & input_site_text(std::istream &, site<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::ostream & output_site_bin(std::ostream &, const site<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::istream & input_site_bin(std::istream &, site<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);
	public:
		//constructor
		site()=default;
		site(const NodeVal& s):val(s){};
		//copy constructor
		site(const site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&)=default;
		//copy assignment
		site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& operator=(const site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&)=default;
		//move constructor
		site(site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&&)=default;
		//move assignment
		site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& operator=(site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>&&)=default;

		void clean();

		/**
		 * \brief 格点所附着的信息
		 */
		NodeVal val;
		/**
		 * \brief 格点所相连的边, 存储了另一测的指针等信息
		 * \see bond
		 */
		std::map<EdgeKey,bond<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>,typename Trait::edgekey_less> bonds;
	};
}


#endif
