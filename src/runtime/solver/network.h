#pragma once

#include <vector>
#include <algorithm>
#include <unordered_set>
#include <limits>

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

		Node(size_t NodeNumber, int NodeValue = std::numeric_limits<int>::max());
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

class Network {
public:
	Network(const make_network& args);
};

}
}
}
