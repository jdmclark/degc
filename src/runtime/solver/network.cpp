#include "network.h"
#include <limits>

using namespace Deg::Runtime::Solver;

static const int source_index = 0;
static const int sink_index = 1;

make_network::Node::Node(size_t NodeNumber, int NodeValue)
	: NodeNumber(NodeNumber), NodeValue(NodeValue) {
	return;
}

make_network::make_network()
	: num_nodes(2) {
	// Add source and sink nodes
	nodes.emplace_back(source_index);
	nodes.emplace_back(sink_index);
}

make_network& make_network::AddRequirement(int value) {
	nodes.emplace_back(num_nodes, value);
	nodes[num_nodes].OutEdges.push_back(sink_index);
	requirement_nodes.push_back(num_nodes);
	++num_nodes;

	return *this;
}

make_network& make_network::AddLimit(int value) {
	nodes.emplace_back(num_nodes, value);
	nodes[num_nodes].OutEdges.push_back(sink_index);
	limit_nodes.push_back(num_nodes);
	++num_nodes;

	return *this;
}

make_network& make_network::AddSources(size_t count) {
	Node& source_node = nodes[source_index];
	for(size_t i = 0; i < count; ++i) {
		nodes.emplace_back(num_nodes);
		source_nodes.push_back(num_nodes);
		source_node.OutEdges.push_back(num_nodes);
		++num_nodes;
	}

	return *this;
}

make_network& make_network::AddEdgeFromSourceToLimit(size_t src, size_t lim) {
	Node& source_node = nodes[source_nodes[src]];
	Node& limit_node = nodes[limit_nodes[lim]];

	source_node.OutEdges.push_back(limit_node.NodeNumber);

	return *this;
}

make_network& make_network::AddEdgeFromSourceToRequirement(size_t src, size_t req) {
	Node& source_node = nodes[source_nodes[src]];
	Node& req_node = nodes[requirement_nodes[req]];

	source_node.OutEdges.push_back(req_node.NodeNumber);

	return *this;
}

make_network& make_network::AddEdgeFromRequirementToLimit(size_t req, size_t lim) {
	Node& req_node = nodes[requirement_nodes[req]];
	Node& lim_node = nodes[limit_nodes[lim]];

	req_node.OutEdges.clear();
	req_node.OutEdges.push_back(lim_node.NodeNumber);

	return *this;
}

make_network& make_network::AddEdgeFromLimitToLimit(size_t lim_a, size_t lim_b) {
	Node& lim_source = nodes[limit_nodes[lim_a]];
	Node& lim_target = nodes[limit_nodes[lim_b]];

	lim_source.OutEdges.clear();
	lim_source.OutEdges.push_back(lim_target.NodeNumber);

	return *this;
}

Network::Network(const make_network& args) {

}
