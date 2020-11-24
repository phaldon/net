#ifndef NET_NETWORK_DRAW_HPP
#define NET_NETWORK_DRAW_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <initializer_list>
#include <stdexcept>
#include <cstdlib>
#include "error.hpp"
#include "node.hpp"
#include "network.hpp"

#ifdef NET_GRAPH_VIZ
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>

#ifdef NET_SHOW_FIG
#ifdef __APPLE_
// do nothing, iterm2 support graph
#else
#ifdef __CLING__
#include <unistd.h>
#include <map>
#include <string>
extern int pipeToJupyterFD;
// https://github.com/root-project/cling/blob/0c880f8472f5a313095203ccd35561b0097b13ec/tools/Jupyter/Kernel.cpp#L54
bool show_mime(const std::map<std::string, std::string>& contentDict) {
  unsigned char sizeLong = sizeof(long);
  if (write(pipeToJupyterFD, &sizeLong, 1) != 1)
    return false;
  long dictSize = contentDict.size();
  if (write(pipeToJupyterFD, &dictSize, sizeof(long)) != sizeof(long))
    return false;

  for (auto iContent: contentDict) {
    const std::string& mimeType = iContent.first;
    long mimeTypeSize = (long)mimeType.size();
    if (write(pipeToJupyterFD, &mimeTypeSize, sizeof(long)) != sizeof(long))
      return false;
    if (write(pipeToJupyterFD, mimeType.c_str(), mimeType.size() + 1)
        != (long)(mimeType.size() + 1))
      return false;
    const std::string& mimeData = iContent.second;
    const long string_size = mimeData.size();
    if (write(pipeToJupyterFD, &string_size, sizeof(long))
        != sizeof(long))
      return false;
    if (write(pipeToJupyterFD, mimeData.data(), string_size)
        != string_size)
      return false;
  }
  return true;
}
#else
#include <cstdlib>
#endif
#endif
#endif
#endif

namespace net{

#ifdef NET_GRAPH_VIZ
	std::string render(std::string dot_content);
#ifdef NET_SHOW_FIG
	void show_fig(const std::string & fig_content,bool tmux);
#endif
#endif

	static std::string base64_encode(const std::string &in);

#ifdef NET_GRAPH_VIZ
	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::draw_to_file(const std::string & filename,const bool label_bond){

		draw(filename,{},label_bond);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::draw_to_file(const std::string & filename,
		const std::set<NodeKey,typename Trait::nodekey_less> & contains,const bool label_bond){

		std::ofstream fig_file(filename, std::ios::binary);
		if(nodes.size()>0){
			fig_file<<render(gviz(contains,label_bond));
		}

	}

	std::string render(std::string dot_content){
	    GVC_t *gvc;
	    Agraph_t *g;
	    char * cres;
	    std::string res;
	    unsigned int  len;
	    gvc = gvContext();
	    g = agmemread(dot_content.c_str());
	    gvLayout(gvc, g, "sfdp");
	    gvRenderData(gvc, g, "png", &cres,&len);
		res = std::string(cres,len);
		gvFreeRenderData (cres);
	    gvFreeLayout(gvc, g);
	    agclose(g);
	    return res;
	}

#ifdef NET_SHOW_FIG
	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::draw(const bool label_bond){

		draw({},label_bond);
	}

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	void network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::draw(const std::set<NodeKey,typename Trait::nodekey_less> & contains,const bool label_bond){

		if(nodes.size()==0){
			std::cout<<"-----------------------------------------------\n";
			std::cout<<"empty lattice\n";
			std::cout<<"-----------------------------------------------\n";
		}else{
			show_fig(render(gviz(contains,label_bond)),false);
		}

	}

	void show_fig(const std::string & fig_content,bool tmux){
#ifdef __APPLE__
		if(tmux){
			std::cout<<"\033Ptmux;\033\033]";
		}else{
			std::cout<<"\033]";
		}
		std::cout<<"1337;File=;inline=1:";
		std::cout<<base64_encode(fig_content);
		if(tmux){
			std::cout<<"\a\033\\";
		}else{
			std::cout<<"\a";
		}
		std::cout<<"\n";
		std::cout<<"Paused. Please press enter to continue.\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#else
#ifdef __CLING__
      show_mime({{"text/html","<img src='data:image/png;base64, "+base64_encode(fig_content)+"'></img>"}});
#else
      std::ofstream("/tmp/net_tmp.png") << fig_content;
      std::system("gwenview /tmp/net_tmp.png || eog /tmp/net_tmp.png");
#endif
#endif
	}
#endif

#endif

	template<typename NodeVal,typename EdgeVal,typename NodeKey, typename EdgeKey, typename Trait>
	std::string network<NodeVal,EdgeVal,NodeKey,EdgeKey,Trait>::gviz(const std::set<NodeKey,typename Trait::nodekey_less> & contains,const bool label_bond){

		std::stringstream dot_content;
		bool fst_attr;
		std::set<NodeKey,typename Trait::nodekey_less> drawn_nodes;
		
		dot_content<<"digraph G {\n";
		dot_content<<"  scale=0.6\n";
		dot_content<<"  dpi=160\n";
		dot_content<<"  bgcolor=\"transparent\"\n";
		dot_content<<"  fontcolor=White\n";
		dot_content<<"  fontname=\"Monaco\"\n";
		if(name==""){
			dot_content<<"  label = \"figure of network\"\n";
		}else{
			dot_content<<"  label = \"figure of "+name+"\"\n";
		}
		

		for(auto& s_it:nodes){
			auto & nodekey1=s_it.first;
			if (contains.count(nodekey1)==1){
				dot_content<<"  "<<Trait::nodekey_brief(nodekey1)<<" [ color=Red, label = \""<<
				Trait::nodekey_brief(nodekey1)<<"\", fontcolor=White, fontname=\"Monaco\"]\n";
			}else{
				dot_content<<"  "<<Trait::nodekey_brief(nodekey1)<<" [ color=White, label = \""<<
				Trait::nodekey_brief(nodekey1)<<"\", fontcolor=White, fontname=\"Monaco\"]\n";
			}
		}
		dot_content<<"subgraph bond {\n";
		dot_content<<"  edge[dir=none]\n";

		for(auto & s_it:nodes){
			auto & nodekey1=s_it.first;
			for(auto & b_it:s_it.second.edges){
				auto & ind1=b_it.first;
				auto & nodekey2=b_it.second.nbkey;
				auto & ind2=b_it.second.ind;
				fst_attr=true;
				if (drawn_nodes.count(nodekey2)==0){
					dot_content<<"  "<<Trait::nodekey_brief(nodekey1)<<" -> "<<Trait::nodekey_brief(nodekey2)<<" [fontcolor=White, fontname=\"Monaco\",";
					if(label_bond){
						if (!fst_attr) dot_content<<",";
						dot_content<<"taillabel = \""<<Trait::edgekey_brief(ind1)<<"\",headlabel =\""<<Trait::edgekey_brief(ind2)<<"\"";
						fst_attr=false;
					}
					if(contains.count(nodekey1)==1 && contains.count(nodekey2)==1){
						if (!fst_attr) dot_content<<",";
						dot_content<<"color=Red";
						fst_attr=false;
					}else{
						if (!fst_attr) dot_content<<",";
						dot_content<<"color=White";
						fst_attr=false;
					}
					if (!fst_attr) dot_content<<",";
					dot_content<<" len=3]\n";
				}
			}
			drawn_nodes.insert(nodekey1);
		}

		dot_content<<"}\n";
		dot_content<<"}\n";
		return dot_content.str();

	}


	// base64 functions are copied from https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
	// originally by Manuel Martinez

	static std::string base64_encode(const std::string &in) {

	    std::string out;

	    int val=0, valb=-6;
	    for (unsigned char c : in) {
	        val = (val<<8) + c;
	        valb += 8;
	        while (valb>=0) {
	            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
	            valb-=6;
	        }
	    }
	    if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
	    while (out.size()%4) out.push_back('=');
	    return out;
	}

	static std::string base64_decode(const std::string &in) {

	    std::string out;

	    std::vector<int> T(256,-1);
	    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i; 

	    int val=0, valb=-8;
	    for (unsigned char c : in) {
	        if (T[c] == -1) break;
	        val = (val<<6) + T[c];
	        valb += 6;
	        if (valb>=0) {
	            out.push_back(char((val>>valb)&0xFF));
	            valb-=8;
	        }
	    }
	    return out;
	}
}
#endif
