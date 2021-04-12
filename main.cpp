// #include "header/Application_RATP.hpp"
#include "header/appRatp.hpp"
#include "header/Grade.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace travel;


int main()
{
	bool b = false;	// true for small data, false for huge data
	string stations_filename;
	string connections_filename;
	if (b == true)
    {
        stations_filename = "../data/stations.csv";
        connections_filename = "../data/connections.csv";
    }else if (b == false)
    {
        stations_filename = "../data/s.csv";
        connections_filename = "../data/c.csv";
    }
	appRatp p;
	p.read_stations(stations_filename);
	p.read_connections(connections_filename);
    // p.compute_and_display_travel(1,8);
    p.compute_and_display_travel(1629,2519);


	return 0;
}
