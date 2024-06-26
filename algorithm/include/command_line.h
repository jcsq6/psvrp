#pragma once
#include <string>

struct program_options
{
	std::string instance;
	std::size_t seed;
	int weight1, weight2, weight3, weight4;
	double rf, dod, W, d_param;
	bool CI, II, RD, WD, CD, GR, RR;
};

program_options get_command_line(int argc, char **argv);