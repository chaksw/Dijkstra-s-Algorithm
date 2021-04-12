#ifndef GSP_H
#define GSP_H


#include <iostream>
#include <unordered_map>
#include <vector>
#include "Generic_station_parser.hpp"
#include "Generic_connection_parser.hpp"
#include "Generic_mapper.hpp"
using namespace std;
using namespace travel;

class Application_RATP : public Generic_connection_parser //Generic_mapper	//public Generic_station_parser, 
{
	public:
			Application_RATP(){};
			/**
			 *  如果派生类在虚函数声明时使用了override描述符，那么该函数必须重载其基类中的同名函数，否则代码将无法通过编译，
			 *  其实override就是为了保证纯许函数在派生类中进行了重载
			 * */
			virtual void read_stations(const string& _filename) override;
			virtual void read_connections(const string& _filename) override;
			virtual vector<pair<uint64_t,uint64_t> > compute_travel(uint64_t _start, uint64_t _end);
			virtual vector<pair<uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end);
};

#endif		//GSP_H

