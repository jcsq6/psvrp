#include "command_line.h"

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

program_options get_command_line(int argc, char **argv)
{
	namespace po = boost::program_options;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("instance,i", po::value<std::string>(), "Instance file")
		("seed,s", po::value<std::size_t>(), "Random number generator seed")
		("weight1", po::value<int>()->default_value(24), "Weight 1")
		("weight2", po::value<int>()->default_value(22), "Weight 2")
		("weight3", po::value<int>()->default_value(20), "Weight 3")
		("weight4", po::value<int>()->default_value(4), "Weight 4")
		("rf", po::value<double>()->default_value(.16), "Reaction factor")
		("dod", po::value<double>()->default_value(.41), "Percent of customers removed")
		("W", po::value<double>()->default_value(2.04), "Temperature control parameter")
		("d_param", po::value<double>()->default_value(5), "Determinism parameter")
		("CI", po::value<std::string>()->default_value("true"), "Enable (true/false)")
		("II", po::value<std::string>()->default_value("true"), "Enable (true/false)")
		("RD", po::value<std::string>()->default_value("true"), "Enable (true/false)")
		("WD", po::value<std::string>()->default_value("true"), "Enable (true/false)")
		("CD", po::value<std::string>()->default_value("true"), "Enable (true/false)")
		("GR", po::value<std::string>()->default_value("true"), "Enable (true/false)")
		("RR", po::value<std::string>()->default_value("true"), "Enable (true/false)");
	
	try
	{
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("help"))
		{
			std::cout << desc << '\n';
			std::exit(0);
		}

		program_options res;

		if (vm.count("instance"))
			res.instance = vm["instance"].as<std::string>();

		if (vm.count("seed"))
			res.seed = vm["seed"].as<std::size_t>();
		else
			res.seed = static_cast<std::size_t>(-1);

		res.weight1 = vm["weight1"].as<int>();
		res.weight2 = vm["weight2"].as<int>();
		res.weight3 = vm["weight3"].as<int>();
		res.weight4 = vm["weight4"].as<int>();

		res.rf = vm["rf"].as<double>();
		res.dod = vm["dod"].as<double>();
		res.W = vm["W"].as<double>();
		res.d_param = vm["d_param"].as<double>();

		auto get_bool = [](const std::string& str)
		{
			std::string lower = boost::to_lower_copy(str);
			if (lower == "true" || lower == "1")
				return true;
			if (lower == "false" || lower == "0")
				return false;
			throw std::runtime_error("Invalid boolean value, \"" + str + '"');
		};

		res.CI = get_bool(vm["CI"].as<std::string>());
		res.II = get_bool(vm["II"].as<std::string>());
		res.RD = get_bool(vm["RD"].as<std::string>());
		res.WD = get_bool(vm["WD"].as<std::string>());
		res.CD = get_bool(vm["CD"].as<std::string>());
		res.GR = get_bool(vm["GR"].as<std::string>());
		res.RR = get_bool(vm["RR"].as<std::string>());

		return res;
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << '\n';
		std::exit(1);
	}
}