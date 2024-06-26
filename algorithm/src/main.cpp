#include "info.h"
#include "command_line.h"

#include "utility.h"

#include <iostream>

int main(int argc, char **argv)
{
	program_options options = get_command_line(argc, argv);
	
	vrp::fleet_info vehicles;
	{
		// TODO: read from file
		std::size_t auto_count = 3;
		std::size_t van_count = 4;
		std::size_t drone_count = 2;
		std::size_t truck_drone_count = 2;
		vrp::cost_data labor_cost{.cost = 10, .cost_rate = 1};
		vrp::cost_data electric_cost{.cost = 10, .cost_rate = 1};
		vrp::cost_data fuel_cost{.cost = 10, .cost_rate = 1};
		vrp::cost_data emmision_cost{.cost = 10, .cost_rate = 1};
		vrp::vehicle auto_data{.capacity = 496, .max_range = 80, .cost = 7};
		vrp::vehicle van_data{.capacity = 2000, .max_range = 200, .cost = 20};
		vrp::vehicle drone_data{.capacity = 5, .max_range = 24, .cost = 1};
		vrp::vehicle truck_drone_data{.capacity = 2000, .max_range = 200, .cost = 30};

		vehicles = vrp::fleet_info(auto_count, van_count, drone_count, truck_drone_count, labor_cost, electric_cost, fuel_cost, emmision_cost, auto_data, van_data, drone_data, truck_drone_data);
	}

	vrp::geographic_vec2 knoxville{35.9606, 83.9207};
	vrp::customer_info customers = vrp::random_customers(5, knoxville, 10, 1, 6, options.seed);

	std::cout << std::ranges::transform_view(customers.nodes(), [](const vrp::customer &c) { return c.pos(); }) << '\n';
}