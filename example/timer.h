#include <chrono>
#include <map>

#ifndef TIMER_H
#define TIMER_H

class timer_record{
public:
	std::chrono::steady_clock::time_point start_time;
	std::chrono::steady_clock::duration cumu_time=std::chrono::steady_clock::duration::zero();
	bool started=false;
};

class timer{
public:
	bool exist(const std::string &);
	void del(const std::string &);
	void del();
	void print();
	int num();
	void start(const std::string &);
	void stop(const std::string &);
	double get(const std::string &);
	void zero_out(const std::string &);
	void zero_out();
private:
	std::map<std::string,timer_record> records;
};

#endif
