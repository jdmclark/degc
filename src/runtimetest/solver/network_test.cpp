#include <nullunit/nullunit.h>
#include "runtime/solver/network.h"

using namespace Deg::Runtime::Solver;

BeginSuite(NetworkTest);

Case(NetworkBuild) {
	Network n = make_network().AddRequirement(3)
							  .AddRequirement(6)
							  .AddLimit(6)
							  .AddLimit(3)
							  .AddSources(2)
							  .AddEdgeFromSourceToRequirement(0, 0)
							  .AddEdgeFromSourceToRequirement(0, 1)
							  .AddEdgeFromSourceToRequirement(1, 1)
							  .AddEdgeFromSourceToLimit(1, 0)
							  .AddEdgeFromLimitToLimit(0, 1)
							  .AddEdgeFromRequirementToLimit(0, 1);
	(void)n;
}

EndSuite(NetworkTest);
