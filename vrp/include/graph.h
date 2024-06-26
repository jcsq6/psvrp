#pragma once
#include "info.h"
#include <optional>

VRP_BEG

class graph
{
public:
	constexpr graph() : M_manhattan{}, M_euclidean{}, M_customers{} {}
	constexpr graph(const customer_info &customers) :
		M_manhattan{customers.distance_matrix<distance_type::manhattan>()},
		M_euclidean{customers.distance_matrix<distance_type::euclidean>()},
		M_customers{&customers}
	{
	}

	matrix::value_type drone_distance(std::size_t a, std::size_t b) const { return M_euclidean[a][b]; }
	matrix::value_type van_distance(std::size_t a, std::size_t b) const { return M_manhattan[a][b]; }

	const customer_info &customers() const
	{
		#ifdef DO_CHECKING
		if (!M_customers)
			throw std::logic_error("Graph has no customers");
		#endif
		return *M_customers;
	}
	std::size_t size() const
	{
		#ifdef DO_CHECKING
		if (!M_customers)
			throw std::logic_error("Graph has no customers");
		#endif
		return M_customers->size();
	}
private:
	matrix M_manhattan, M_euclidean;
	const customer_info *M_customers;
};

class abstract_vehicle
{
public:
	// abstract_vehicle() : M_graph{} {}
	abstract_vehicle(const vrp::graph &graph) : M_graph{&graph} {}

	const vrp::graph &graph() const { return *M_graph; }

	virtual double cost() const = 0;
	virtual std::size_t size() const = 0;

	virtual ~abstract_vehicle() = default;
protected:
	const vrp::graph *M_graph;
};

template <vehicle_type type>
class vehicle_route;

template <>
class vehicle_route<vehicle_type::base> : public abstract_vehicle
{
public:
	vehicle_route(const vrp::graph &graph) : abstract_vehicle(graph), M_cost{0}
	{
		M_route.reserve(graph.size());
		M_route.push_back(0);
	}

	void insert(std::size_t index, std::size_t customer)
	{
		#if DO_CHECKING
		if (index > M_route.size())
			throw std::out_of_range("Invalid index");
		if (M_route.size() == M_graph->size())
			throw std::length_error("Route is full");
		if (customer >= M_graph->size() || customer == 0)
			throw std::invalid_argument("Invalid customer");
		#endif
		
		std::size_t before = M_route[(index + M_route.size() - 1) % M_route.size()];

		M_cost -= M_graph->van_distance(before, M_route[index % M_route.size()]); // how much this node and the previous one cost
		M_route.insert(M_route.begin() + index, customer);
		M_cost += M_graph->van_distance(before, customer) + M_graph->van_distance(customer, M_route[(index + 1) % M_route.size()]); // how much this node and its new neighbors cost
	}

	void remove(std::size_t index)
	{
		#if DO_CHECKING
		if (index >= M_route.size())
			throw std::out_of_range("Invalid index");
		if (M_route.size() == 1)
			throw std::length_error("Route is empty");
		#endif

		std::size_t before = M_route[(index + M_route.size() - 1) % M_route.size()];
		std::size_t current = M_route[index % M_route.size()];
		std::size_t after = M_route[(index + 1) % M_route.size()];
		M_cost -= M_graph->van_distance(before, current) + M_graph->van_distance(current, after);
		M_route.erase(M_route.begin() + index);
		M_cost += M_graph->van_distance(before, after);
	}

	double cost() const override { return M_cost; }
	std::size_t size() const override { return M_route.size(); }

	double manual_cost() const // only for testing. for checking to make sure cost calculation is correct
	{
		double sum = 0;
		for (std::size_t i = 0; i < M_route.size() - 1; ++i)
			sum += M_graph->van_distance(M_route[i], M_route[i + 1]);
		sum += M_graph->van_distance(M_route.back(), M_route.front());
		return sum;
	}

	const std::size_t &operator[](std::size_t index) const
	{
		#if DO_CHECKING
		if (index >= M_route.size())
			throw std::out_of_range("Invalid index");
		#endif
		return M_route[index];
	}
	
private:
	std::vector<std::size_t> M_route;
	double M_cost;
};

template <>
class vehicle_route<vehicle_type::autonomous> : public vehicle_route<vehicle_type::base>
{
public:
	using vehicle_route<vehicle_type::base>::vehicle_route;
};

template <>
class vehicle_route<vehicle_type::van> : public vehicle_route<vehicle_type::base>
{
public:
	using vehicle_route<vehicle_type::base>::vehicle_route;
};

template <>
class vehicle_route<vehicle_type::drone> : public abstract_vehicle
{
public:
	vehicle_route(const vrp::graph &graph) : abstract_vehicle(graph), M_cost{0}
	{
		M_route.reserve(graph.size());
	}

	void insert(std::size_t customer)
	{
		#if DO_CHECKING
		if (M_route.size() == M_graph->size())
			throw std::length_error("Route is full");
		if (customer >= M_graph->size() || customer == 0)
			throw std::invalid_argument("Invalid customer");
		#endif

		M_route.push_back(customer);
		M_cost += 2 * M_graph->drone_distance(0, customer); // how much it costs to go from the depot to the customer and back
	}

	void remove(std::size_t index)
	{
		#if DO_CHECKING
		if (index >= M_route.size())
			throw std::out_of_range("Invalid index");
		#endif

		M_cost -= 2 * M_graph->drone_distance(0, M_route[index]); // how much it costs to go from the depot to the customer and back
		M_route.erase(M_route.begin() + index);
	}

	double cost() const override { return M_cost; }
	std::size_t size() const override { return M_route.size(); }

	double manual_cost() const // only for testing. for checking to make sure cost calculation is correct
	{
		double sum = 0;
		for (std::size_t i = 0; i < M_route.size(); ++i)
			sum += 2 * M_graph->drone_distance(0, M_route[i]);
		return sum;
	}

	const std::size_t &operator[](std::size_t index) const { return M_route[index]; }
private:
	std::vector<std::size_t> M_route;
	double M_cost;
};

template <>
class vehicle_route<vehicle_type::truck_drone> : public abstract_vehicle
{
public:
	struct drone_node
	{
		std::size_t departure; // customer where the drone departs from the truck
		std::size_t service; // customer where the drone delivers the package
		std::size_t reunion; // customer where the drone returns to the truck
	};

	vehicle_route(const vrp::graph &graph) : abstract_vehicle(graph), M_truck_route(graph), M_drone_cost{0}
	{
		M_drones.reserve(graph.size()); // actual max capacity should be less than graph size
	}

	void insert(std::size_t index, std::size_t customer) { M_truck_route.insert(index, customer); }
	void insert_rendevous(std::size_t departure_customer, std::size_t service_customer, std::size_t reunion_customer)
	{
		#if DO_CHECKING
		// if (M_drones.size() == M_graph->size())
		// 	throw std::runtime_error("Route is full");
		if (departure_customer == 0 || service_customer == 0 || reunion_customer == 0 ||
			departure_customer >= M_graph->size() || service_customer >= M_graph->size() || reunion_customer >= M_graph->size())
			throw std::invalid_argument("Invalid customer");
		#endif

		M_drones.push_back({.departure = departure_customer, .service = service_customer, .reunion = reunion_customer});

		M_drone_cost += M_graph->drone_distance(departure_customer, service_customer) + M_graph->drone_distance(service_customer, reunion_customer);
	}

	void remove(std::size_t index) { M_truck_route.remove(index); }
	void remove_rendevous(std::size_t index)
	{
		#if DO_CHECKING
		if (index >= M_drones.size())
			throw std::out_of_range("Invalid index");
		#endif
		drone_node &node = M_drones[index];
		M_drone_cost -= M_graph->drone_distance(node.departure, node.service) + M_graph->drone_distance(node.service, node.reunion);
		M_drones.erase(M_drones.begin() + index);
	}

	double cost() const override { return M_drone_cost + M_truck_route.cost(); }
	std::size_t size() const override { return M_truck_route.size(); }
	std::size_t size_rendevous() const { return M_drones.size(); }

	double manual_cost() const // only for testing. for checking to make sure cost calculation is correct
	{
		double sum = M_truck_route.manual_cost();
		for (const drone_node &node : M_drones)
			sum += M_graph->drone_distance(node.departure, node.service) + M_graph->drone_distance(node.service, node.reunion);
		return sum;
	}

	const std::size_t &truck_stop(std::size_t index) const { return M_truck_route[index]; }
	const drone_node &rendevous(std::size_t index) const { return M_drones[index]; }

private:
	vehicle_route<vehicle_type::base> M_truck_route;
	std::vector<drone_node> M_drones;
	double M_drone_cost;
};

using base_route = vehicle_route<vehicle_type::base>;
using autonomous_route = vehicle_route<vehicle_type::autonomous>;
using van_route = vehicle_route<vehicle_type::van>;
using drone_route = vehicle_route<vehicle_type::drone>;
using truck_drone_route = vehicle_route<vehicle_type::truck_drone>;

class solution
{
public:
	constexpr solution() : M_graph{}, M_fleet{} {}
	constexpr solution(const graph &graph, const fleet_info &fleet) : M_graph{&graph}, M_fleet{&fleet} {}

	const vrp::graph &graph() const { return *M_graph; }
	const fleet_info &fleet() const { return *M_fleet; }
private:
	const vrp::graph *M_graph;
	const fleet_info *M_fleet;
};

VRP_END