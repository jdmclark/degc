#include "network.h"
#include <limits>
#include <algorithm>

using namespace Deg::Runtime::Solver;

static const size_t source_index = 0;
static const size_t sink_index = 1;

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
	for(size_t i = 0; i < count; ++i) {
		nodes.emplace_back(num_nodes);
		source_nodes.push_back(num_nodes);
		nodes[source_index].OutEdges.push_back(num_nodes);
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

make_network& make_network::AddEdgeFromLimitToLimit(size_t lim_a, size_t lim_b) {
	Node& lim_source = nodes[limit_nodes[lim_a]];
	Node& lim_target = nodes[limit_nodes[lim_b]];

	lim_source.OutEdges.clear();
	lim_source.OutEdges.push_back(lim_target.NodeNumber);

	return *this;
}

Network::Node::Node(size_t edge_offset, size_t edge_count, size_t back_edge_offset, size_t back_edge_count)
	: edge_offset(edge_offset), edge_count(edge_count), back_edge_offset(back_edge_offset), back_edge_count(back_edge_count) {
	return;
}

Network::Edge::Edge(size_t from, size_t to) : from(from), to(to) {
	return;
}

Network::Network(const make_network& args) {
	// Populate edges list
	for(const make_network::Node& n : args.nodes) {
		for(size_t fwd_e : n.OutEdges) {
			edges.emplace_back(n.NodeNumber, fwd_e);
		}
	}

	// Build edge map
	for(const make_network::Node& n : args.nodes) {
		size_t fwd_offset = edge_map.size();
		size_t fwd_ct = 0;

		for(size_t fwd_e : n.OutEdges) {
			// Find edge in list
			for(size_t i = 0; i < edges.size(); ++i) {
				if(edges[i].from == n.NodeNumber && edges[i].to == fwd_e) {
					edge_map.push_back(i);
					++fwd_ct;
				}
			}
		}

		size_t back_offset = edge_map.size();
		size_t back_ct = 0;

		for(size_t i = 0; i < edges.size(); ++i) {
			if(edges[i].to == n.NodeNumber) {
				edge_map.push_back(i);
				++back_ct;
			}
		}

		nodes.emplace_back(fwd_offset, fwd_ct, back_offset, back_ct);
	}

	// Populate source edges
	for(size_t src_n_idx : args.source_nodes) {
		Node& n = nodes[src_n_idx];

		size_t src_in_max = n.back_edge_offset + n.back_edge_count;
		for(size_t i = n.back_edge_offset; i < src_in_max; ++i) {
			source_edges.push_back(edge_map[i]);
		}
	}

	// Populate requirement edges
	for(size_t req_n_idx : args.requirement_nodes) {
		Node& n = nodes[req_n_idx];

		size_t req_out_max = n.edge_offset + n.edge_count;
		for(size_t i = n.edge_offset; i < req_out_max; ++i) {
			requirement_edges.push_back(edge_map[i]);
			requirement_capacities.push_back(args.nodes[req_n_idx].NodeValue);
		}
	}

	// Populate limit edges
	for(size_t lim_n_idx : args.limit_nodes) {
		Node& n = nodes[lim_n_idx];

		size_t lim_out_max = n.edge_offset + n.edge_count;
		for(size_t i = n.edge_offset; i < lim_out_max; ++i) {
			limit_edges.push_back(edge_map[i]);
			limit_capacities.push_back(args.nodes[lim_n_idx].NodeValue);
		}
	}
}

void NetworkSolver::internal_build_flow_table(const Network& network, const std::vector<int>& source_values, const std::vector<int>& limit_values) {
	// Build flow table
	node_data.resize(network.nodes.size());
	for(NodeData& n : node_data) {
		n.Counter = 0;
	}

	flow.resize(network.edges.size());
	for (EdgeFlow& f : flow) {
		f.Flow = 0;
		f.Capacity = std::numeric_limits<int>::max();
	}

	// Populate sources
	for (size_t i = 0; i < network.source_edges.size() && i < source_values.size(); ++i) {
		EdgeFlow& f = flow[network.source_edges[i]];
		f.Capacity = source_values[i];
	}

	// Populate requirements
	for (size_t i = 0; i < network.requirement_edges.size(); ++i) {
		EdgeFlow& f = flow[network.requirement_edges[i]];
		f.Capacity = network.requirement_capacities[i];
	}
	// Populate limits
	for (size_t i = 0; i < network.limit_edges.size() && i < limit_values.size(); ++i) {
		EdgeFlow& f = flow[network.limit_edges[i]];
		f.Capacity = limit_values[i] - network.limit_capacities[i];
	}
}

bool NetworkSolver::internal_bfs(int counter, int amount, const Network& network) {
	bfs_queue.clear();

	NodeData& source_data = node_data[source_index];
	source_data.Counter = counter;
	source_data.PushedFlow = amount;
	bfs_queue.push_back(source_index);

	while(!bfs_queue.empty()) {
		size_t next = bfs_queue.front();
		bfs_queue.pop_front();

		int flow_amount = node_data[next].PushedFlow;
		const Network::Node& n = network.nodes[next];

		// Enqueue forward edges
		size_t edge_offset = n.edge_offset;
		size_t edge_limit = edge_offset + n.edge_count;
		for(size_t next_e = edge_offset; next_e < edge_limit; ++next_e) {
			size_t next_e_idx = network.edge_map[next_e];
			size_t next_n_idx = network.edges[next_e_idx].to;
			EdgeFlow& next_e_flow = flow[next_e_idx];
			NodeData& next_n_data = node_data[next_n_idx];
			if(next_n_data.Counter != counter && next_e_flow.Capacity > next_e_flow.Flow) {
				next_n_data.Counter = counter;
				next_n_data.PushedFlow = std::min(flow_amount, next_e_flow.Capacity - next_e_flow.Flow);
				next_n_data.PredecessorEdge = next_e_idx;
				next_n_data.PredecessorBack = false;

				if(next_n_idx == sink_index) {
					return true;
				}
				else {
					bfs_queue.push_back(next_n_idx);
				}
			}
		}

		// Enqueue back edges
		size_t back_edge_offset = n.back_edge_offset;
		size_t back_edge_limit = back_edge_offset + n.back_edge_count;
		for(size_t next_e = back_edge_offset; next_e < back_edge_limit; ++next_e) {
			size_t next_e_idx = network.edge_map[next_e];
			size_t next_n_idx = network.edges[next_e_idx].from;
			EdgeFlow& next_e_flow = flow[next_e_idx];
			NodeData& next_n_data = node_data[next_n_idx];
			if(next_n_data.Counter != counter && next_e_flow.Flow > 0) {
				next_n_data.Counter = counter;
				next_n_data.PushedFlow = -std::min(flow_amount, next_e_flow.Flow);
				next_n_data.PredecessorEdge = next_e_idx;
				next_n_data.PredecessorBack = true;

				bfs_queue.push_back(next_n_idx);
			}
		}
	}

	return false;
}

bool NetworkSolver::internal_push_flow(const Network& network, int amount, int counter) {
	if(internal_bfs(counter, amount, network)) {
		size_t node = sink_index;
		int actual_pushed_flow = node_data[sink_index].PushedFlow;

		while(node != source_index) {
			// Apply flows
			NodeData& n = node_data[node];
			EdgeFlow& f = flow[n.PredecessorEdge];
			if(n.PredecessorBack) {
				f.Flow -= actual_pushed_flow;
				node = network.edges[n.PredecessorEdge].to;
			}
			else {
				f.Flow += actual_pushed_flow;
				node = network.edges[n.PredecessorEdge].from;
			}
		}

		return true;
	}

	return false;
}

void NetworkSolver::internal_solve(const Network& network, const std::vector<int>& source_values, const std::vector<int>& limit_values) {
	internal_build_flow_table(network, source_values, limit_values);

	// Solve with modified Ford-Fulkerson.

	int Counter = 1;
	while(internal_push_flow(network, std::numeric_limits<int>::max(), Counter)) {
		++Counter;
	}
}

bool NetworkSolver::Solve(const Network& network, const std::vector<int>& source_values, const std::vector<int>& limit_values) {
	internal_solve(network, source_values, limit_values);

	// Validate answer
	for(size_t i : network.requirement_edges) {
		const EdgeFlow& f = flow[i];
		if(f.Capacity != f.Flow) {
			return false;
		}
	}

	for(size_t i : network.limit_edges) {
		const EdgeFlow& f = flow[i];
		if(f.Capacity > f.Flow) {
			return false;
		}
	}

	return true;
}
