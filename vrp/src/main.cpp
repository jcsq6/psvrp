#include "global.h"

int main()
{
	std::size_t auto_count = 3;
	std::size_t van_count = 4;
	std::size_t drone_count = 2;
	std::size_t truckDrone_count = 2;
	cost_data labor_cost{.cost = 10, .cost_rate = 1};
	cost_data electric_cost{.cost = 10, .cost_rate = 1};
	cost_data fuel_cost{.cost = 10, .cost_rate = 1};
	cost_data emmision_cost{.cost = 10, .cost_rate = 1};
	vehicle auto_data{.capacity = 496, .max_range = 80, .cost = 7};
	vehicle van_data{.capacity = 2000, .max_range = 200, .cost = 20};
	vehicle drone_data{.capacity = 5, .max_range = 24, .cost = 1};
	vehicle truckDrone_data{.capacity = 2000, .max_range = 200, .cost = 30};
	
	instance i(auto_count, van_count, drone_count, truckDrone_count, labor_cost, electric_cost, fuel_cost, emmision_cost, auto_data, van_data, drone_data, truckDrone_data);

}