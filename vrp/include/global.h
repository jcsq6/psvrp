#pragma once
#include <array>
#include <vector>
#include <stdexcept>

enum class cost_type : std::size_t
{
	labor,
	electric,
	fuel,
	emmisions,
};

enum class vehicle_type
{
	base,
	autonomous,
	van,
	drone,
	truckDrone,
};

struct vehicle
{
	double capacity;
	double max_range;
	double cost;
};

struct cost_data
{
	double cost;
	double cost_rate;
};

class instance
{
public:
	constexpr instance(std::size_t auto_count, std::size_t van_count, std::size_t drone_count, std::size_t truckDrone_count,
		     		   cost_data labor_cost, cost_data electric_cost, cost_data fuel_cost, cost_data emmision_cost,
		     		   const vehicle &auto_data, const vehicle &van_data, const vehicle &drone_data, const vehicle &truckDrone_data) :
		M_vehicles{{{}, auto_data, van_data, drone_data, truckDrone_data}},
		M_cost{{labor_cost, electric_cost, fuel_cost, emmision_cost}},
		M_auto_count{auto_count}, M_van_count{van_count}, M_drone_count{drone_count}, M_truckDrone_count{truckDrone_count}, M_fleet_count{auto_count + van_count + drone_count}
	{
		if (van_count + drone_count >= truckDrone_count)
			throw std::runtime_error("Invalid fleet");

		M_fleet_capacity = auto_count * auto_data.capacity + van_count * van_data.capacity + drone_count * drone_data.capacity + truckDrone_count * truckDrone_data.capacity;
	}

	constexpr instance(std::size_t base_count,
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