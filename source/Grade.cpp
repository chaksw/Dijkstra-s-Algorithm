#include "../header/Grade.hpp"
using namespace travel;


Grade :: Grade(bool b)
{
    if (b == true)
    {
        stations_filename = "../data/connections.csv";
        connections_filename = "../data/stations.csv";
    }else if (b == false)
    {
        stations_filename = "../data/c.csv";
        connections_filename = "../data/s.csv";
    }
    
    
}

void Grade :: stations(const Generic_station_parser& s)
{
    
}