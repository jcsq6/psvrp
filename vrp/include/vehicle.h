#pragma once
#include "instance.h"

enum class vehicle_type
{
	autonomous,
	van,
	drone,
	truckDrone,
};

class vehicle
{
public:
	constexpr vehicle(double capacity, double max_range, vehicle_type type) :
		M_capacity{capacity}, M_max_range{max_range}, M_type{type}
	{
	}

	constexpr double capacity() const { return M_capacity; }
	constexpr double max_range() const { return M_max_range; }
	constexpr vehicle_type type() const { return M_type; }
private:
	double M_capacity;
	double M_max_range;
	vehicle_type M_type;
};