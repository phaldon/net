#ifndef NET_GVIZ_HPP
#define NET_GVIZ_HPP

#include <sstream>
#include <string>
#include <set>
#include <map>
#include "error.hpp"
#include "node.hpp"
#include "network.hpp"

namespace net{

	inline std::map<std::string,std::string> gviz_theme1({
		{"global_bgcolor","transparent"},{"global_fontcolor","White"},{"global_fontname","Monaco"},
		{"node_strokecolor","White"},{"node_fontcolor","White"},{"node_fontname","Monaco"},
		{"node_chosen_strokecolor","Red"},{"node_chosen_fontcolor","White"},{"node_chosen_fontname","Monaco"},
		{"edge_strokecolor","White"},{"edge_fontcolor","White"},{"edge_fontname","Monaco"},
		{"edge_chosen_strokecolor","Red"},{"edge_chosen_fontcolor","White"},{"edge_chosen_fontname","Monaco"}});

	inline std::map<std::string,std::string> gviz_theme2({
		{"global_bgcolor","White"},{"global_fontcolor","Black"},{"global_fontname","Monaco"},
		{"node_strokecolor","Black"},{"node_fontcolor","Black"},{"node_fontname","Monaco"},
		{"node_chosen_strokecolor","Red"},{"node_chosen_fontcolor","Black"},{"node_chosen_fontname","Monaco"},
		{"edge_strokecolor","Black"},{"edge_fontcolor","Black"},{"edge_fontname","Monaco"},
		{"edge_chosen_strokecolor","Red"},{"edge_chosen_fontcolor","Black"},{"edge_chosen_fontname","Monaco"}});

	inline std::map<std::string,std::string> gviz_theme3({
		{"global_bgcolor","transparent"},{"global_fontcolor","Black"},{"global_fontname","Monaco"},
		{"node_strokecolor","Black"},{"node_fontcolor","Black"},{"node_fontname","Monaco"},
		{"node_chosen_strokecolor","Red"},{"node_chosen_fontcolor","Black"},{"node_chosen_fontname","Monaco"},
		{"edge_strokecolor","Black"},{"edge_fontcolor","Black"},{"edge_fontname","Monaco"},
		{"edge_chosen_strokecolor","Red"},{"edge_chosen_fontcolor","Black"},{"edge_chosen_fontname","Monaco"}});

	inline std::map<std::string,std::string> gviz_theme=gviz_theme1;

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	std::string network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::gviz(const std::set<NodeKey,typename Trait::nodekey_less> & contains,const bool label_bond){

		std::stringstream dot_content;
		bool fst_attr;
		std::set<NodeKey,typename Trait::nodekey_less> drawn_nodes;
		
		dot_content<<"digraph G {\n";
		dot_content<<"  scale=0.6\n";
		dot_content<<"  dpi=160\n";
		dot_content<<"  bgcolor=\""+gviz_theme["global_bgcolor"]+"\"\n";
		dot_content<<"  fontcolor="+gviz_theme["global_fontcolor"]+"\n";
		dot_content<<"  fontname=\""+gviz_theme["global_fontname"]+"\"\n";
		if(name==""){
			dot_content<<"  label = \"figure of network\"\n";
		}else{
			dot_content<<"  label = \"figure of "+name+"\"\n";
		}
		

		for(auto& s_it:nodes){
			auto & nodekey1=s_it.first;
			if (contains.count(nodekey1)==1){
				dot_content<<"  "<<Trait::nodekey_brief(nodekey1)<<" [ color="+gviz_theme["node_chosen_strokecolor"]+", label = \""<<
				Trait::nodekey_brief(nodekey1)<<"\", fontcolor="+gviz_theme["node_chosen_fontcolor"]+", fontname=\""+gviz_theme["node_chosen_fontname"]+"\"]\n";
			}else{
				dot_content<<"  "<<Trait::nodekey_brief(nodekey1)<<" [ color="+gviz_theme["node_strokecolor"]+", label = \""<<
				Trait::nodekey_brief(nodekey1)<<"\", fontcolor="+gviz_theme["node_fontcolor"]+", fontname=\""+gviz_theme["node_fontname"]+"\"]\n";
			}
		}
		dot_content<<"subgraph bond {\n";
		dot_content<<"  edge[dir=none]\n";

		for(auto & s_it:nodes){
			auto & nodekey1=s_it.first;
			for(auto & b_it:s_it.second.edges){
				auto & ind1=b_it.first;
				auto & nodekey2=b_it.second.nbkey;
				auto & ind2=b_it.second.nbind;
				if (drawn_nodes.count(nodekey2)==0){
					dot_content<<"  "<<Trait::nodekey_brief(nodekey1)<<" -> "<<Trait::nodekey_brief(nodekey2);
					if(contains.count(nodekey1)==1 && contains.count(nodekey2)==1)
						dot_content<<" [fontcolor="+gviz_theme["edge_chosen_fontcolor"]+", fontname=\""+gviz_theme["edge_chosen_fontname"]+"\", color="+gviz_theme["edge_chosen_strokecolor"];
					else
						dot_content<<" [fontcolor="+gviz_theme["edge_fontcolor"]+", fontname=\""+gviz_theme["edge_fontname"]+"\", color="+gviz_theme["edge_strokecolor"];
					if(label_bond)
						dot_content<<", taillabel = \""<<Trait::edgekey_brief(ind1)<<"\",headlabel =\""<<Trait::edgekey_brief(ind2)<<"\"";
					dot_content<<", len=3]\n";
				}
			}
			drawn_nodes.insert(nodekey1);
		}

		dot_content<<"}\n";
		dot_content<<"}\n";
		return dot_content.str();

	}
}
#endif