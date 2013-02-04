#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace Deg {
namespace Compiler {
namespace SG {

class Node;

class Scope {
private:
	std::vector<std::unique_ptr<Node>> children;
	std::unordered_map<std::string, Node*> children_map;

public:
	inline bool IsMember(const std::string& name) const {
		return children_map.count(name) > 0;
	}

	template <typename T, typename... Args> void MakeMember(const std::string& name, Args&&... args) {
		children.push_back(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
		children_map.insert(std::make_pair(name, children.back().get()));
	}

	Node& GetMember(const std::string& name) const;

	inline std::vector<std::unique_ptr<Node>>::iterator begin() {
		return children.begin();
	}

	inline std::vector<std::unique_ptr<Node>>::iterator end() {
		return children.end();
	}

	inline size_t children_size() const {
		return children.size();
	}
};

}
}
}
