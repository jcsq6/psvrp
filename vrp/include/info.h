#pragma once
#include <array>
#include <vector>
#include <stdexcept>

#include <ranges>
#include <random>

#include "nodes.h"

enum class cost_type : std::size_t
{
	labor,
	electric,
	fuel,
	emmisions,
};

struct cost_data
{
	double cost;
	double cost_rate;
};

class fleet_info
{
public:
	constexpr fleet_info() :
		M_vehicles{},
		M_cost{},
		M_auto_count{}, M_van_count{}, M_drone_count{}, M_truckDrone_count{}, M_fleet_count{},
		M_fleet_capacity{}
	{
	}

	constexpr fleet_info(std::size_t auto_count, std::size_t van_count, std::size_t drone_count, std::size_t truckDrone_count,
		     		   cost_data labor_cost, cost_data electric_cost, cost_data fuel_cost, cost_data emmision_cost,
		     		   const vehicle &auto_data, const vehicle &van_data, const vehicle &drone_data, const vehicle &truckDrone_data) :
		M_vehicles{{{}, auto_data, van_data, drone_data, truckDrone_data}},
		M_cost{{labor_cost, electric_cost, fuel_cost, emmision_cost}},
		M_auto_count{auto_count}, M_van_count{van_count}, M_drone_count{drone_count}, M_truckDrone_count{truckDrone_count}, M_fleet_count{auto_count + van_count + drone_count}
	{
		if (van_count + drone_count < truckDrone_count)
			throw std::runtime_error("Invalid fleet");

		M_fleet_capacity = auto_count * auto_data.capacity + van_count * van_data.capacity + drone_count * drone_data.capacity + truckDrone_count * truckDrone_data.capacity;
	}

	constexpr fleet_info(std::size_t base_count,
		     		   cost_data labor_cost, cost_data electric_cost, cost_data fuel_cost, cost_data emmision_cost,
		     		   const vehicle &base_data) :
		M_vehicles{{base_data}},
		M_cost{{labor_cost, electric_cost, fuel_cost, emmision_cost}},
		M_auto_count{0}, M_van_count{0}, M_drone_count{0}, M_truckDrone_count{0}, M_fleet_count{base_count}
	{
		M_fleet_capacity = base_count * base_data.capacity;
	}

	constexpr std::size_t fleet_count() const { return M_fleet_count; }
	constexpr std::size_t auto_count() const { return M_auto_count; }
	constexpr std::size_t van_count() const { return M_van_count; }
	constexpr std::size_t drone_count() const { return M_drone_count; }
	constexpr std::size_t truckDrone_count() const { return M_truckDrone_count; }

	constexpr double cost(cost_type type) const { return M_cost[static_cast<std::size_t>(type)].cost; }
	constexpr double cost_rate(cost_type type) const { return M_cost[static_cast<std::size_t>(type)].cost_rate; }

	constexpr double fleet_capacity() const { return M_fleet_capacity; }

private:
	std::array<vehicle, 5> M_vehicles;
	std::array<cost_data, 5> M_cost;

	std::size_t M_auto_count; // number of autonomous vehicles
	std::size_t M_van_count; // number of electric vehicles
	std::size_t M_drone_count; // number of standalone drones (maybe unused?)
	std::size_t M_truckDrone_count; // number of truck-drones

	std::size_t M_fleet_count; // total number of vehicles used

	double M_fleet_capacity;
};

class customer_info
{
public:
	constexpr customer_info() :
		M_customers{}
	{
	}

	template <std::ranges::range Customers>
	constexpr customer_info(vec2 depot, Customers &&customers)
	{
		auto range = std::ranges::single_view(customer{depot, 0}) | customers;
		M_customers.assign(std::ranges::begin(range), std::ranges::end(range));
	}

	// returns a distance matrix of customers where index 0 is the depot
	template <distance_type type>
	matrix distance_matrix() const
	{
		auto size = M_customers.size();
		matrix res(size, size);

		for (std::size_t i = 0; i < size; ++i)
			for (std::size_t j = 0; j < size; ++j)
				res[i][j] = res[j][i] = distance<type>(M_customers[i], M_customers[j]);

		return res;
	}

	constexpr std::size_t size() const { return M_customers.size(); }

	constexpr const customer &node(std::size_t i) const { return M_customers[i]; }
	constexpr const customer &depot() const { return M_customers[0]; }

	constexpr const std::vector<customer> &nodes() const { return M_customers; }

private:
	std::vector<customer> M_customers;

	friend customer_info random_customers(std::size_t count, geographic_vec2 center, double box_size, std::size_t min_demand, std::size_t max_demand, std::size_t seed);
};

/// @param count number of customers
/// @param center center of box in |latitude, longitude|
/// @param box_size size of box in miles
/// @param seed random number generator seed
/// @returns randomly generated customers
customer_info random_customers(std::size_t count, geographic_vec2 center, double box_size, std::size_t min_demand, std::size_t max_demand, std::size_t seed = static_cast<std::size_t>(-1))
{
	if (seed == static_cast<std::size_t>(-1))
		seed = std::random_device{}();

	std::mt19937_64 gen(seed);

	constexpr double circ_earth = 2 * std::numbers::pi * earth_radius;
	double half_size = box_size / 2;
	double half_width_lat = half_size * (360 / circ_earth); // the same as (half_size * 1.60934 / 111) 
	double half_width_long = half_size * (360 / (circ_earth * std::cos(radians(center.latitude)))); // same as (half_size * 1.60934 / (111 * cos(radians(center.latitude))))

	std::uniform_real_distribution<double> latitude_vec(-half_width_lat, half_width_lat);
	std::uniform_real_distribution<double> longitude_vec(-half_width_long, half_width_long);
	std::uniform_int_distribution<std::size_t> demand_dist(min_demand, max_demand);

	customer_info res;
	res.M_customers.reserve(count + 1);
	res.M_customers.emplace_back(vec2{0, 0}, 0); // depot

	for (std::size_t i = 0; i < count; ++i)
	{
		auto new_loc = center + geographic_vec2(latitude_vec(gen), longitude_vec(gen));
		vec2 new_pos = equirectangular_projection(new_loc, center);

		res.M_customers.emplace_back(new_pos, demand_dist(gen));
	}

	return res;
}