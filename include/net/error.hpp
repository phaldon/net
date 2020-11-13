#ifndef ERROR_H
#define ERROR_H

#include <stdexcept>

namespace net{
	class key_exist_error: public std::logic_error{
	public:
		explicit key_exist_error(const std::string & mess): std::logic_error(mess){}
	};

	class key_unfound_error: public std::logic_error{
	public:
		explicit key_unfound_error(const std::string & mess): std::logic_error(mess){}
	};
}
#endif