#pragma once
#include <array>
#include <vector>
#include "vehicle.h"

enum class cost_type : std::size_t
{
	labor,
	electric,
	fuel,
	emmisions,
};

class instance
{
public:
	constexpr instance(std::size_t auto_count, std::size_t van_count, std::size_t drone_count, std::size_t truckDrone_count, std::size_t base_count,
		     		   double labor_cost, double electric_cost, double fuel_cost, double emmision_cost,
			 		   std::size_t cost_rate_labor, std::size_t cost_rate_electric, std::size_t cost_rate_fuel, std::size_t cost_rate_emmision) :
		M_cost{{labor_cost, electric_cost, fuel_cost, emmision_cost}},
		M_cost_rate{{cost_rate_labor, cost_rate_electric, cost_rate_fuel, cost_rate_emmision}},
		M_auto_count{auto_count}, M_van_count{van_count}, M_drone_count{drone_count}, M_truckDrone_count{truckDrone_count}, M_fleet_count{auto_count + van_count + drone_count + base_count},
		M_fleet_capacity{0.0}
	{
	}

	constexpr std::size_t fleet_count() const { return M_fleet_count; }
	constexpr std::size_t auto_count() const { return M_auto_count; }
	constexpr std::size_t van_count() const { return M_van_count; }
	constexpr std::size_t drone_count() const { return M_drone_count; }
	constexpr std::size_t truckDrone_count() const { return M_truckDrone_count; }

	constexpr double cost(cost_type type) const { return M_cost[static_cast<std::size_t>(type)]; }
	constexpr std::size_t cost_rate(cost_type type) const { return M_cost_rate[static_cast<std::size_t>(type)]; }

	constexpr double fleet_capacity() const { return M_fleet_capacity; }

	constexpr const std::vector<vehicle> &vehicles() const { return M_vehicles; }

	void add_vehicle(double capacity, double max_range, vehicle_type type)
	{
		M_vehicles.emplace_back(capacity, max_range, type);
		M_fleet_capacity += capacity;
	}

private:
	std::array<double, 4> M_cost; // total costs of each cost_type
	std::array<std::size_t, 4> M_cost_rate; // cost rate of each cost_type

	std::size_t M_auto_count; // number of autonomous vehicles
	std::size_t M_van_count; // number of electric vehicles
	std::size_t M_drone_count; // number of standalone drones (maybe unused?)
	std::size_t M_truckDrone_count; // number of truck-drones

	std::size_t M_fleet_count; // total number of vehicles used

	std::vector<vehicle> M_vehicles;
	double M_fleet_capacity;
};