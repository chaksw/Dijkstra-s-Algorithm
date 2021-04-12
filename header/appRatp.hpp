#pragma once

#include <iostream>
#include <unordered_map>
#include "Generic_mapper.hpp"

using namespace travel;

class appRatp : public Generic_mapper
{
    public:
        // appRatp(){};
    //  protected:
        virtual void read_stations(const std::string& _filename) override;
        virtual void read_connections(const std::string& _filename) override;

        virtual std::vector<std::pair<uint64_t,uint64_t> > compute_travel(uint64_t _start, uint64_t _end) override;
        virtual std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end) override;
};


