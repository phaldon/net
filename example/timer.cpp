#include <chrono>
#include <stdexcept>
#include <iostream>
#include <map>
#include "timer.h"

int timer::num(){
	return records.size();
}

void timer::del(){
	records.clear();
}

void timer::del(const std::string & name){
	records.erase(name);
}

bool timer::exist(const std::string & name){
	return(records.count(name)==1);
}

void timer::start(const std::string & name){
	auto & rec=records[name];
	if(!rec.started){
		rec.start_time=std::chrono::steady_clock::now();
		rec.started=true;
	}
}

void timer::stop(const std::string & name){
	auto & rec=records[name];
	if(rec.started){
		rec.cumu_time+=(std::chrono::steady_clock::now()-rec.start_time);
		rec.started=false;
	}
}

double timer::get(const std::string & name){
	timer_record * rec;
	std::chrono::duration<double> diff;
	try{
		rec=&records.at(name);
	}catch(std::out_of_range err){
		return 0;
	}
	if(rec->started){
		diff=rec->cumu_time+(std::chrono::steady_clock::now()-rec->start_time);
	}else{
		diff=rec->cumu_time;
	}
	return diff.count();
}

void timer::zero_out(const std::string & name){
	auto & rec=records[name];
	rec.started=false;
	rec.cumu_time=std::chrono::steady_clock::duration::zero();
}

void timer::zero_out(){
	for(auto & rec:records){
		rec.second.started=false;
		rec.second.cumu_time=std::chrono::steady_clock::duration::zero();
	}
}

void timer::print(){
	std::chrono::duration<double> diff;
	std::cout << ' '<<std::endl << "===================="<<std::endl;
	for(auto & rec:records){
		std::cout << rec.first<<"  ";
		if(rec.second.started){
			diff=rec.second.cumu_time+(std::chrono::steady_clock::now()-rec.second.start_time);
		}else{
			diff=rec.second.cumu_time;
		}
		std::cout << diff.count()<<'s'<<std::endl;
	}
	std::cout << "===================="<<std::endl<< ' '<<std::endl ;

}