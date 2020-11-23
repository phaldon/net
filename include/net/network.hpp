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
#include "site.hpp"
#include "traits.hpp"

namespace net{

	template <typename NodeVal,typename EdgeVal,typename EdgeKey,typename Comp>
	using dec_type = std::function<void(NodeVal&,NodeVal&,NodeVal&,
		const std::set<EdgeKey,Comp> &,const EdgeKey & ,const EdgeKey & ,EdgeVal&)>;
	template <typename NodeVal,typename EdgeVal,typename EdgeKey>
	using absorb_type = std::function<void(NodeVal&,EdgeVal&,const EdgeKey &)>;
	template <typename NodeVal,typename EdgeKey,typename Comp>
	using contract_type = std::function<NodeVal(NodeVal&,NodeVal&,const std::set<std::pair<EdgeKey,EdgeKey>,Comp> &)>;
	template <typename NodeVal,typename EdgeKey>
	using init_site_type = std::function<NodeVal(const std::vector<EdgeKey> &)>;
	template <typename EdgeVal,typename EdgeKey>
	using init_bond_type = std::function<EdgeVal(const EdgeKey &,const EdgeKey &)>;

	template<typename NodeVal,typename EdgeVal,typename NodeKey,typename EdgeKey>
	struct default_traits;
   /**
    * \brief 网络是一些点和点间关联的并
    *
    * 每个格点拥有一个名字，整个网络也有自己的名字，网络可以通过格点名称寻找格点
    * \see site, bond
    * \tparam T 每个格点中附着的信息类型
    * \tparam V 每个边上附着的信息类型
    */


	template <typename NodeVal, typename EdgeVal, typename NodeKey=std::string, typename EdgeKey=stdEdgeKey,
		typename Trait = default_traits<NodeVal,EdgeVal,NodeKey,EdgeKey> >
	class network{

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::ostream & operator<<(std::ostream &, const network<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::istream & operator>>(std::istream &, network<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::ostream & operator<(std::ostream &, const network<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeKey1, typename EdgeKey1, typename Trait1>	
	friend std::istream & operator>(std::istream &, network<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> &);

	template<typename NodeVal1,typename EdgeVal1,typename NodeVal2,typename EdgeVal2, typename NodeKey1, typename EdgeKey1, typename Trait1>
	friend network<NodeVal2,EdgeVal2,NodeKey1,EdgeKey1,Trait1> fmap(const network<NodeVal1,EdgeVal1,NodeKey1,EdgeKey1,Trait1> & n,
		std::function<NodeVal2(const NodeVal1 &)> f1,std::function<EdgeVal2(const EdgeVal1 &)> f2);

	public:
		//constructor
		network()=default;
		network(const std::string &);
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

		void fope(std::function<NodeVal(const NodeVal&)>,std::function<EdgeVal(const EdgeVal&)>);

		IterNode add(const NodeKey &);
		void set_bond(const NodeKey &,const NodeKey &,const EdgeKey &,const EdgeKey &);
		void set_bond(IterNode,IterNode,const EdgeKey &,const EdgeKey &);

		void del(const NodeKey &);
		void del(IterNode);
		void del_bond(const NodeKey &,const NodeKey &);
		void del_bond(IterNode,IterNode);
		void del_leg(const NodeKey &,const EdgeKey &);
		void del_leg(IterNode,const EdgeKey &);

		void rename(const NodeKey &,const NodeKey &);

		void absorb(const NodeKey &,const NodeKey &,absorb_type<NodeVal,EdgeVal,EdgeKey>,
			contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less>);
		void split(const NodeKey &,const NodeKey &,const NodeKey &, const std::set<EdgeKey,typename Trait::edgekey_less> &,
			dec_type<NodeVal,EdgeVal,EdgeKey,typename Trait::edgekey_less>);
		void split(const NodeKey &,const NodeKey &, const std::set<EdgeKey,typename Trait::edgekey_less> &,
			dec_type<NodeVal,EdgeVal,EdgeKey,typename Trait::edgekey_less>);
	#ifdef NET_GRAPH_VIZ
		void draw_to_file(const std::string &,const bool);
		void draw_to_file(const std::string &,const std::set<NodeKey,typename Trait::nodekey_less> &,const bool);
	#ifdef NET_SHOW_FIG
		void draw(const bool);
		void draw(const std::set<NodeKey,typename Trait::nodekey_less> &,const bool);
	#endif
	#endif
		std::string gviz(const std::set<NodeKey,typename Trait::nodekey_less> &,const bool);
		bool exist(const NodeKey &);

		NodeVal& operator[](const NodeKey &);
		void set_val(const NodeKey &,const NodeVal& );
		void init_sites(init_site_type<NodeVal,EdgeKey> );
		void init_bonds(init_bond_type<EdgeVal,EdgeKey> );
		NodeVal contract(absorb_type<NodeVal,EdgeVal,EdgeKey>, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less>);

		void tn_contract1(const NodeKey &,const std::set<NodeKey,typename Trait::nodekey_less> &, NodeVal&, 
			absorb_type<NodeVal,EdgeVal,EdgeKey>, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less>);
		NodeVal tn_contract2(const std::set<NodeKey,typename Trait::nodekey_less> &, NodeVal&, const std::set<NodeKey,typename Trait::nodekey_less> &, NodeVal&,
			absorb_type<NodeVal,EdgeVal,EdgeKey>, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less>);

		using IterNode = std::map<NodeKey,site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>,typename Trait::nodekey_less>::iterator
	private:
		std::map<NodeKey,site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>,typename Trait::nodekey_less> sites;
		std::string name="";
	};

	template<typename T>
	std::string to_string(const T& m){
		std::stringstream a;
		a<<m;
		return a.str();
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
		// this is not default because sites[i].bonds[j].neighbor needs redirection
	network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::network(const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& N){
		sites=N.sites;
		name=N.name;
		for(auto & s:sites){
			for (auto & b: s.second.bonds){
				b.second.neighbor=&(sites[b.second.name]);
			}
		}
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
		// this is not default because sites[i].bonds[j].neighbor needs redirection
	network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::operator=
		(const network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>& N){
		if(this != &N){
			sites=N.sites;
			name=N.name;
			for(auto & s:sites){
				for (auto & b: s.second.bonds){
					b.second.neighbor=&(sites[b.second.name]);
				}
			}
		}
		return *this;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>	
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::init_sites(init_site_type<NodeVal,EdgeKey> init_fun){

		for (auto& site:sites){
			std::vector<EdgeKey> inds;
			for (auto& b:site.second.bonds){
				inds.push_back(b.first);
			}
			site.second.val = init_fun(inds);
		}
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::init_bonds(init_bond_type<EdgeVal,EdgeKey> init_fun){

		for (auto& site:sites){
			for (auto& b:site.second.bonds){
				b.second.val=init_fun(b.first,b.second.ind);
			}
		}
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::network(const std::string & s) : name(s){}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	NodeVal& network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::operator[](const NodeKey & name){
		return sites[name].val;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_val(const NodeKey & name,const NodeVal& value){
		sites[name].val=value;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	bool network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::exist(const NodeKey & name){
		return (sites.count(name)==1);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	auto network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::add(const NodeKey & name){
		auto [s1,succ1]=sites.insert(make_pair(name,site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>()));
		if(!succ1){
			throw key_exist_error("In network.add, site "+to_string(name)+" already exists!");
		}
		return s1
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del(const NodeKey & name){

		auto site_itr = sites.find(name);
		if(site_itr == sites.end()){
			throw key_unfound_error("In network.del, site "+to_string(name)+" is not found!");
		}

		del(site_itr);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del(network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::IterNode it){

		for(auto & b: it->second->bonds){
			b.second.neighbor->bonds.erase(b.second.ind);
		}
		sites.erase(it);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del_bond(const NodeKey & name1,const NodeKey& name2){

		auto site_itr = sites.find(name1);
		if(site_itr == sites.end()){
			throw key_unfound_error("In network.del_bond, site "+to_string(name1)+" is not found!");
		}

		if(sites.count(name2)==0){
			throw key_unfound_error("In network.del_bond, site "+to_string(name2)+" is not found!");
		}

		for(auto bond_itr=site_itr->second.bonds.begin(); bond_itr != site_itr->second.bonds.end(); ) {
			if (bond_itr->second.name==name2) {
				bond_itr->second.neighbor->bonds.erase(bond_itr->second.ind);
				bond_itr=site_itr->second.bonds.erase(bond_itr);
			} else {
				++bond_itr;
			}
		}
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::del_leg(const NodeKey & name,const EdgeKey & ind){

		auto site_itr = sites.find(name);
		if(site_itr == sites.end()){
			throw key_unfound_error("In network.del_ind, site "+to_string(name)+" is not found!");
		}

		site_itr->second.bonds.erase(ind);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::rename(const NodeKey & old_name,const NodeKey & new_name){

		auto site_handle = sites.extract(old_name);
		if(site_handle.empty()){
			throw key_unfound_error("In network.rename, site "+to_string(old_name)+" is not found!");
		}

		for(auto & b: site_handle.value.bonds){
			(b.second.neighbor->bonds)[b.second.ind].name=new_name;
		}
		site_handle.key=new_name;
		
		auto status = sites.insert(std::move(site_handle));
		if(!status.inserted)throw key_exist_error("In network.rename, site "+to_string(new_name)+" already exists!");
	}


	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::set_bond(const NodeKey & name1,const NodeKey& name2, const EdgeKey & ind1,const EdgeKey& ind2){
		site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * site1,* site2;
		try{
			site1=&sites.at(name1);
		}catch(std::out_of_range err){
			throw key_unfound_error("In network.set_bond, site "+to_string(name1)+" is not found!");
		}
		try{
			site2=&sites.at(name2);
		}catch(std::out_of_range err){
			throw key_unfound_error("In network.set_bond, site at "+to_string(name2)+" is not found!");
		}
		auto [s1,succ1]=site1->bonds.insert(make_pair(ind1,bond<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(name2,ind2,site2)));
		if(!succ1)throw key_exist_error("In network.set_bond, ind "+to_string(ind1)+" of site "+to_string(name1)+" already linked!");
		auto [s2,succ2]=site2->bonds.insert(make_pair(ind2,bond<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(name1,ind1,site1)));
		if(!succ2)throw key_exist_error("In network.set_bond, ind "+to_string(ind2)+" of site "+to_string(name2)+" already linked!");
	}

	// template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	// void network<T,V>::set_bond(const std::string & name1,const std::string & name2){
	// 	set_bond(name1,name2,name1+'.'+name2,name2+'.'+name1);
	// }

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::absorb(const NodeKey & name1,const NodeKey & name2,
		absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun){
		if (sites.count(name1)==0){
			throw key_unfound_error("In network.absorb, site "+to_string(name1)+" is not found!");
		}
		if (sites.count(name2)==0){
			throw key_unfound_error("In network.absorb, site "+to_string(name2)+" is not found!");
		}
		auto & site1=sites[name1];
		auto & site2=sites[name2];

		std::set<std::pair<EdgeKey,EdgeKey>,typename Trait::edge2key_less> ind_pairs;

		for(auto iter=site1.bonds.begin(); iter != site1.bonds.end(); ) {
			if (iter->second.name==name2) {
				ind_pairs.insert(std::make_pair(iter->first,iter->second.ind));
				absorb_fun(site1.val,iter->second.val,iter->first);
				iter=site1.bonds.erase(iter);
			} else {
				++iter;
			}
		}

		site1.val=contract_fun(site1.val,site2.val,ind_pairs);

		for(auto & b:site2.bonds){
			if(b.second.name != name1){
				site1.bonds[b.first]=b.second;
				sites[b.second.name].bonds[b.second.ind].name=name1;
			}
		}

		sites.erase(name2);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::split(const NodeKey & name1, const NodeKey & name2,const NodeKey & name3
		,const std::set<EdgeKey,typename Trait::edgekey_less> & inds, dec_type<NodeVal,EdgeVal,EdgeKey,typename Trait::edgekey_less> dec_fun){

		auto s1=sites.find(name1);
		auto &b1=s1->bonds;
		auto [s2,succ2]=sites.insert(name2);
		auto &b2=s2->bonds;
		auto [s3,succ3]=sites.insert(name3);
		auto &b3=s3->bonds;

		for(auto & b:b1){
			if(inds.count(b.first)==0){
				b2[b.first]=b.second;
				sites[b.second.name].bonds[b.second.ind].name=name2;
			}else{
				b3[b.first]=b.second;
				sites[b.second.name].bonds[b.second.ind].name=name3;
			}
		}

		EdgeVal env;
		std::string ind2=name2+'.'+name3;
		std::string ind3=name3+'.'+name2;
		dec_fun(s1->val,s2->val,s3->val,inds,ind2,ind3,env);
		b2[ind2]=bond<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(name3,ind3,env);
		b3[ind3]=bond<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(name2,ind2,env);

		sites.erase(s1);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::split(const NodeKey & name1, const NodeKey & name2,
		const std::set<EdgeKey,typename Trait::edgekey_less> & inds,dec_type<NodeVal,EdgeVal,EdgeKey,typename Trait::edgekey_less> dec_fun){

		auto s1=sites.find(name1);
		auto &b1=s1->bonds;
		auto [s2,succ2]=sites.insert(name2);
		auto &b2=s2->bonds;

		for(auto iter=b1.begin(); iter != b1.end(); ) {
			if (inds.count(iter->first)==1) {
				b2[iter->first]=iter->second;
				sites[iter->second.name]=name2;
				iter=b1.erase(iter);
			} else {
				++iter;
			}
		}

		auto temp=s1->val;
		EdgeVal env;
		std::string ind1=name1+'.'+name2;
		std::string ind2=name2+'.'+name1;
		dec_fun(temp,s1->val,s2->val,inds,ind1,ind2,env);
		b1[ind1]=bond<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(name2,ind2,env);
		b2[ind2]=bond<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>(name1,ind1,env);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	NodeVal network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::contract(absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun,
		contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun){
		NodeVal tot;
		std::set<NodeKey,typename Trait::nodekey_less> contracted;
		for(auto s:sites){
			if(contracted.count(s.first)==0){
				tn_contract1(s.first,contracted,tot,absorb_fun,contract_fun);
				contracted.insert(s.first);
			}
		}
		return tot;
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::tn_contract1(const NodeKey & sname,const std::set<NodeKey,typename Trait::nodekey_less> & group, NodeVal& ten,
		absorb_type<NodeVal,EdgeVal,EdgeKey> absorb_fun, contract_type<NodeVal,EdgeKey,typename Trait::edge2key_less> contract_fun){

		site<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait> * site;
		try{
			site=&sites.at(sname);
		}catch(std::out_of_range err){
			throw key_unfound_error("In network.set_bond, site "+to_string(sname)+" is not found!");
		}
		if(group.size()==0){
			ten=site->val;
		}else{
			auto site_t=site->val;
			std::set<std::pair<EdgeKey,EdgeKey>,typename Trait::edge2key_less> ind_pairs;

			for(auto & b:site->bonds) {
				//std::cout<<b.first<<b.second.name<<b.second.ind<<'\n';
				if (group.count(b.second.name)==1) {
					ind_pairs.insert(std::make_pair(b.first,b.second.ind));
					absorb_fun(site_t,b.second.val,b.first);
				}
			}

			ten=contract_fun(site_t,ten,ind_pairs);
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
		for (auto & sname:group1){
			auto & site=sites[sname];
			for(auto & b:site.bonds) {
				if (group2.count(b.second.name)==1) {
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
		for (auto & s:n.sites){
			const auto [its, success] = result.sites.insert({s.first,site<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait>()});
			its->second.val=f1(s.second.val);
			for (auto & b:s.second.bonds){
				its->second.bonds[b.first]=bond<NodeVal2,EdgeVal2,NodeKey,EdgeKey,Trait>
					(b.second.name,b.second.ind,nullptr,f2(b.second.val));
			}
		}
		for (auto & s:result.sites){
			for (auto & b:s.second.bonds){
				b.second.neighbor=&(result.sites[b.second.name]);
			}
		}
		return result;
	}
		//void fope(std::function<NodeVal(const NodeVal&)>,std::function<EdgeVal(const EdgeVal&)>);

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::fope(std::function<NodeVal(const NodeVal &)> f1,
		std::function<EdgeVal(const EdgeVal &)> f2){
		for (auto & s:sites){
			s.second.val=f1(s.second.val);
			for (auto & b:s.second.bonds){
				b.second.val=f2(b.second.val);
			}
		}
	}

}
#endif
