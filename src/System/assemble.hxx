#pragma once
#include "igl/readOBJ.h"
#include "simulation.h"
#include <unordered_set>
#include <memory>
#include <iostream>

static inline unsigned _min(unsigned a, unsigned b) {return a > b ? b : a;}
static inline unsigned _max(unsigned a, unsigned b) {return a < b ? b : a;}

struct myHash 
{
    size_t operator()(std::pair<unsigned, unsigned> __val) const
    {
        return static_cast<size_t>((__val.first << 12)  + __val.second);
    }
};

void assemble(const std::shared_ptr<NetSystem> &system,
              const std::string obj_file_name);

void assemble(const std::shared_ptr<NetSystem> &system, unsigned width, unsigned length);

inline void assemble(const std::shared_ptr<NetSystem> &system, unsigned width, unsigned length)
{
	for (unsigned i = 0; i < width; ++i)
		for (unsigned j = 0; j < length; ++j)
			system->add_particle({i*1.0, 10, j*1.0});

	for (unsigned i = 0; i < width; ++i)
		for (unsigned j = 0; j < length-1; ++j)
			system->add_spring(length*i+j, length*i+j+1, 20, 1);

	for (unsigned i = 0; i < width-1; ++i)
		for (unsigned j = 0; j < length; ++j)
			system->add_spring(length*i+j, length*(1+i)+j, 20, 1);

	for (unsigned i = 0; i < width-1; ++i)
		for (unsigned j = 0; j < length-1; ++j)
			system->add_spring(length*i+j, length*(1+i)+j+1, 20, sqrt(2));
}

inline void assemble(const std::shared_ptr<NetSystem> &system,
              const std::string obj_file_name)
{
	std::vector<std::vector<double > > V;
	std::vector<std::vector<unsigned > > F;

	igl::readOBJ(obj_file_name, V, F);

	std::unordered_set<std::pair<unsigned, unsigned>, myHash> E;
	for (auto &item : F)
	{
		E.emplace(_min(item[0], item[1]), _max(item[0], item[1]));
		E.emplace(_min(item[0], item[2]), _max(item[0], item[2]));
		E.emplace(_min(item[2], item[1]), _max(item[2], item[1]));
	}
	
	for (auto &item : V)
		system->add_particle({item[0], item[1], item[2]});

	const auto & norm = [&V](const unsigned lid, const unsigned rid)->double
	{
		return sqrt((V[lid][0] - V[rid][0]) * (V[lid][0] - V[rid][0])
				+ (V[lid][1] - V[rid][1]) * (V[lid][1] - V[rid][1])
				+ (V[lid][2] - V[rid][2]) * (V[lid][2] - V[rid][2]));
	};

	for (auto &edge : E)
	{
		unsigned lid = edge.first;
		unsigned rid = edge.second;
		system->add_spring(lid, rid, 20, norm(lid, rid));
	}
}