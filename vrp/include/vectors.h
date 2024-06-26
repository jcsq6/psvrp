#pragma once
#include <cmath>
#include <vector>
#include <numbers>
#include <type_traits>
#include "macro.h"

VRP_BEG
class vec2
{
public:
	using value_type = double;

	value_type x, y;

	constexpr vec2() : x{}, y{} {}
	constexpr vec2(value_type _x, value_type _y) : x{_x}, y{_y} {}

	constexpr value_type &operator[](unsigned int i) { return (&x)[i]; }
	constexpr const value_type &operator[](unsigned int i) const { return (&x)[i]; }
};

class geographic_vec2
{
public:
	using value_type = double;

	value_type latitude, longitude;
	constexpr geographic_vec2() : latitude{}, longitude{} {}
	constexpr geographic_vec2(value_type _latitude, value_type _longitude) : latitude{_latitude}, longitude{_longitude} {}

	constexpr value_type &operator[](unsigned int i) { return (&latitude)[i]; }
	constexpr const value_type &operator[](unsigned int i) const { return (&latitude)[i]; }
};

template <typename V>
concept vector = std::is_same_v<vec2, std::remove_cvref_t<V>> || std::is_same_v<geographic_vec2, std::remove_cvref_t<V>>;

template <vector vec> constexpr vec operator+(vec a, vec b) { return vec{a[0] + b[0], a[1] + b[1]}; }
template <vector vec> constexpr vec operator-(vec a, vec b) { return vec{a[0] - b[0], a[1] - b[1]}; }
template <vector vec> constexpr vec operator*(vec a, typename vec::value_type b) { return vec{a[0] * b, a[1] * b}; }
template <vector vec> constexpr vec operator*(typename vec::value_type a, vec b) { return vec{b[0] * a, b[1] * a}; }
template <vector vec> constexpr vec operator/(vec a, typename vec::value_type b) { return vec{a[0] / b, a[1] / b}; }

enum class distance_type
{
	euclidean,
	manhattan,	
};

template <distance_type type>
inline vec2::value_type distance(vec2 a, vec2 b);

template <>
inline vec2::value_type distance<distance_type::euclidean>(vec2 a, vec2 b) { return std::hypot(a.x - b.x, a.y - b.y); }

template <>
inline vec2::value_type distance<distance_type::manhattan>(vec2 a, vec2 b) { return std::abs(a.x - b.x) + std::abs(a.y - b.y); }

inline vec2::value_type length(vec2 a) { return std::hypot(a.x, a.y); }

constexpr double radians(double degrees) { return degrees * std::numbers::pi / 180.0; }

static constexpr double earth_radius = 3958.8; // miles

inline vec2 equirectangular_projection(geographic_vec2 pos, geographic_vec2 center)
{
	auto center_lat_rad = radians(center.latitude);

	vec2 res{
		earth_radius * (radians(pos.longitude) - radians(center.longitude)) * std::cos(center_lat_rad),
		earth_radius * (radians(pos.latitude) - center_lat_rad),
	};

	return res;
}

class matrix
{
public:
	using value_type = double;

	constexpr matrix() : M_rows{}, M_cols{} {}
	constexpr matrix(std::size_t rows, std::size_t cols) : M_data(rows * cols), M_rows{rows}, M_cols{cols} {}

	constexpr std::size_t rows() const { return M_rows; }
	constexpr std::size_t cols() const { return M_cols; }

	constexpr value_type *operator[](std::size_t row) { return M_data.data() + row * M_cols; }
	constexpr const value_type *operator[](std::size_t row) const { return M_data.data() + row * M_cols; }

private:
	std::vector<value_type> M_data;
	std::size_t M_rows, M_cols;
};

VRP_END