#include "graph.h"
#include "utility.h"

#include <iostream>

int main()
{
	vrp::customer_info customers = vrp::random_customers(10, {}, 20, 1, 6, 0);
	vrp::graph graph(customers);

	// base_route test
	{
		vrp::base_route base_route(graph);

		std::mt19937_64 gen(0);
		std::vector<std::size_t> route(customers.size() - 1);
		std::iota(std::begin(route), std::end(route), 1);
		std::shuffle(std::begin(route), std::end(route), gen);

		std::cout << "Testing base route class insert...\n";
		std::size_t i = 0;
		for (; i < customers.size() - 1; ++i)
		{
			std::uniform_int_distribution<std::size_t> route_insert_loc(1, base_route.size());
			base_route.insert(route_insert_loc(gen), route[i]);

			if (std::abs(base_route.cost() - base_route.manual_cost()) > .0001)
			{
				std::cout << "Failed on iteration " << i << '\n';
				break;
			}
		}

		if (i == customers.size() - 1)
		{
			std::cout << "Success\n";
			std::cout << "\nTesting base route class remove...\n";
			for (i = 0; i < customers.size() - 1; ++i)
			{
				std::uniform_int_distribution<std::size_t> route_remove_loc(0, base_route.size() - 1);
				base_route.remove(route_remove_loc(gen));

				if (std::abs(base_route.cost() - base_route.manual_cost()) > .0001)
				{
					std::cout << "Failed on iteration " << i << '\n';
					break;
				}
			}

			if (i == customers.size() - 1)
				std::cout << "Success\n";
		}
	}

	// drone_route test
	{
		vrp::drone_route drone_route(graph);

		std::mt19937_64 gen(0);

		std::vector<std::size_t> route(customers.size() - 1);
		std::iota(std::begin(route), std::end(route), 1);
		std::shuffle(std::begin(route), std::end(route), gen);

		std::cout << "\nTesting drone route class insert...\n";
		std::size_t i = 0;
		for (; i < customers.size() - 1; ++i)
		{
			drone_route.insert(route[i]);

			if (std::abs(drone_route.cost() - drone_route.manual_cost()) > .0001)
			{
				std::cout << "Failed on iteration " << i << '\n';
				break;
			}
		}

		if (i == customers.size() - 1)
		{
			std::cout << "Success\n";
			std::cout << "\nTesting drone route class remove...\n";
			for (i = 0; i < customers.size() - 1; ++i)
			{
				std::uniform_int_distribution<std::size_t> route_remove_loc(0, drone_route.size() - 1);
				drone_route.remove(route_remove_loc(gen));

				if (std::abs(drone_route.cost() - drone_route.manual_cost()) > .0001)
				{
					std::cout << "Failed on iteration " << i << '\n';
					break;
				}
			}

			if (i == customers.size() - 1)
				std::cout << "Success\n";
		}
	}

	// truck_drone_route test
	{
		vrp::truck_drone_route truck_drone_route(graph);

		std::mt19937_64 gen(0);
		std::vector<std::size_t> route(customers.size() - 1);
		std::iota(std::begin(route), std::end(route), 1);
		std::shuffle(std::begin(route), std::end(route), gen);

		std::cout << "\nTesting truck_drone route class insert...\n";
		std::size_t rendevous_i = 1;
		std::size_t i = 0;
		for (; i < customers.size() - 1; ++i)
		{
			std::uniform_int_distribution<std::size_t> route_insert_loc(1, truck_drone_route.size());
			truck_drone_route.insert(route_insert_loc(gen), route[i]);

			if (std::abs(truck_drone_route.cost() - truck_drone_route.manual_cost()) > .0001)
			{
				std::cout << "Failed on iteration " << i << '\n';
				break;
			}
			
			if (truck_drone_route.size_rendevous() < customers.size() / 4)
			{
				truck_drone_route.insert_rendevous(rendevous_i, rendevous_i + 1, rendevous_i + 2);
				
				if (std::abs(truck_drone_route.cost() - truck_drone_route.manual_cost()) > .0001)
				{
					std::cout << "Failed on iteration " << i << '\n';
					break;
				}
			}
		}

		if (i == customers.size() - 1)
		{
			std::cout << "\nTesting base route class remove...\n";

			for (std::size_t i = 0; i < customers.size() - 1; ++i)
			{
				std::uniform_int_distribution<std::size_t> route_remove_loc(0, truck_drone_route.size() - 1);
				truck_drone_route.remove(route_remove_loc(gen));

				if (std::abs(truck_drone_route.cost() - truck_drone_route.manual_cost()) > .0001)
				{
					std::cout << "Failed on iteration " << i << '\n';
					break;
				}
				
				if (truck_drone_route.size_rendevous() > 0)
				{
					std::uniform_int_distribution<std::size_t> rendevous_remove_loc(0, truck_drone_route.size_rendevous() - 1);
					truck_drone_route.remove_rendevous(rendevous_remove_loc(gen));
					if (std::abs(truck_drone_route.cost() - truck_drone_route.manual_cost()) > .0001)
					{
						std::cout << "Failed on iteration " << i << '\n';
						break;
					}
				}
			}

			if (i == customers.size() - 1)
				std::cout << "Success\n";
		}
	}
}