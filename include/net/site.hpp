#ifndef SITE_H
#define SITE_H

#include <map>
#include <vector>

namespace net{
	template <typename T,typename V>
	class site;

	template <typename T,typename V>
	class bond{
	template <typename T1,typename V1>
	friend std::ostream & operator<<(std::ostream &,const bond<T1,V1> &);
	template <typename T1,typename V1>
	friend std::istream & operator>>(std::istream &, bond<T1,V1> &);
	public:
		std::string name;
		std::string ind;
		site<T,V> * neighbor;
		V val;

		bond()=default;
		bond(const std::string & s1,const std::string & s2,site<T,V> * s): name(s1),ind(s2),neighbor(s){};
		bond(const std::string & s1,const std::string & s2,site<T,V> * s, const V & E):name(s1),ind(s2),neighbor(s),val(E){};
		bond(const bond<T,V>&)=default;
	};

	template <typename T,typename V>
	class site{
	template <typename T1,typename V1>
	friend std::ostream & operator<<(std::ostream &,const site<T1,V1> &);
	template <typename T1,typename V1>
	friend std::istream & operator>>(std::istream &, site<T1,V1> &);
	public:
		//constructor
		site()=default;
		site(const T& s):val(s){};
		//copy constructor
		site(const site<T,V>&)=default;
		//copy assignment
		site<T,V>& operator=(const site<T,V>&)=default;
		//move constructor
		site(site<T,V>&&)=default;
		//move assignment
		site<T,V>& operator=(site<T,V>&&)=default;

		void clean();

		T val;
		std::map<std::string,bond<T,V>> bonds;
		std::vector<int> position;
	};
}
#endif