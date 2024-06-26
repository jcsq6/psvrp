#pragma once
#include "vectors.h"

class customer
{
public:
	constexpr customer() : M_pos{}, M_demand{} {}
	constexpr customer(vec2 pos, double demand) : M_pos{pos}, M_demand{demand} {}

	constexpr vec2 pos() const { return M_pos; }
	constexpr double demand() const { return M_demand; }
private:
	vec2 M_pos;
	double M_demand;
};

template <distance_type type>
constexpr double distance(const customer& a, const customer& b) { return distance<type>(a.pos(), b.pos()); }

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