#pragma once

#include <vector>
#include <algorithm>
#include <unordered_set>
#include <limits>
#include <deque>

namespace Deg {
namespace Runtime {
namespace Solver {

class Network;

class make_network {
private:
	friend class Network;

	class Node {
	public:
		size_t NodeNumber;
		int NodeValue;
		std::vector<size_t> OutEdges;

		explicit Node(size_t NodeNumber, int NodeValue = std::numeric_limits<int>::max());
	};

	size_t num_nodes;
	std::vector<Node> nodes;
	std::vector<size_t> requirement_nodes;
	std::vector<size_t> limit_nodes;
	std::vector<size_t> source_nodes;

public:
	make_network();
	make_network& AddRequirement(int value);
	make_network& AddLimit(int value);
	make_network& AddSources(size_t count);
	make_network& AddEdgeFromSourceToRequirement(size_t src, size_t req);
	make_network& AddEdgeFromSourceToLimit(size_t src, size_t lim);
	make_network& AddEdgeFromRequirementToLimit(size_t req, size_t lim);
	make_network& AddEdgeFromLimitToLimit(size_t lim_a, size_t lim_b);
};

class NetworkSolver;

class Network {
private:
	friend class NetworkSolver;

	class Node {
	public:
		size_t edge_offset;
		size_t edge_count;
		size_t back_edge_offset;
		size_t back_edge_count;

		Node(size_t edge_offset, size_t edge_count, size_t back_edge_offset, size_t back_edge_count);
	};

	class Edge {
	public:
		size_t from, to;

		Edge(size_t from, size_t to);
	};

	std::vector<Node> nodes;
	std::vector<size_t> edge_map;
	std::vector<Edge> edges;

	std::vector<size_t> source_edges;
	std::vector<size_t> requirement_edges;
	std::vector<size_t> limit_edges;

	std::vector<int> requirement_capacities;
	std::vector<int> limit_capacities;

public:
	Network(const make_network& args);

	inline size_t GetEdgeCount() const {
		return edges.size();
	}

	inline size_t GetNodeCount() const {
		return nodes.size();
	}
};

class NetworkSolver {
private:
	class NodeData {
	public:
		int Counter;

		int PushedFlow;

		bool PredecessorBack;
		size_t PredecessorEdge;
	};

	class EdgeFlow {
	public:
		int Capacity;
		int Flow;
	};

	std::deque<size_t> bfs_queue;
	std::vector<NodeData> node_data;
	std::vector<EdgeFlow> flow;

	void internal_solve(const Network& network, const std::vector<int>& source_values, const std::vector<int>& limit_values = {});
	void internal_build_flow_table(const Network& network, const std::vector<int>& source_values, const std::vector<int>& limit_values);
	bool internal_push_flow(const Network& network, int amount, int counter);
	bool internal_bfs(int counter, int amount, const Network& network);

public:
	bool Solve(const Network& network, const std::vector<int>& source_values, const std::vector<int>& limit_values = {});
};

}
}
}
