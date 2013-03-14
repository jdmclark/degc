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
							  .AddEdgeFromLimitToLimit(0, 1);

	Test_Assert_Eq(n.GetNodeCount(), 8);
	Test_Assert_Eq(n.GetEdgeCount(), 10);
}

Case(MakeNetworkAddsSourceEdges) {
	Network m = make_network().AddSources(1);
	Test_Assert_Eq(m.GetNodeCount(), 3);
	Test_Assert_Eq(m.GetEdgeCount(), 1);

	Network n = make_network().AddSources(2);
	Test_Assert_Eq(n.GetNodeCount(), 4);
	Test_Assert_Eq(n.GetEdgeCount(), 2);
}

Case(SinglePathNetwork) {
	Network n = make_network().AddRequirement(3)
							  .AddSources(1)
							  .AddEdgeFromSourceToRequirement(0, 0);
	Test_Assert_Eq(n.GetNodeCount(), 4);
	Test_Assert_Eq(n.GetEdgeCount(), 3);

	NetworkSolver ns;

	Test_Assert(ns.Solve(n, { 9999 }));
	Test_Assert(ns.Solve(n, { 3 }));
	Test_Assert(!ns.Solve(n, { 1 }));
}

Case(TwoRequirementNetwork) {
	Network n = make_network().AddRequirement(3)
							  .AddRequirement(6)
							  .AddSources(2)
							  .AddEdgeFromSourceToRequirement(0, 0)
							  .AddEdgeFromSourceToRequirement(1, 1);
	Test_Assert_Eq(n.GetNodeCount(), 6);
	Test_Assert_Eq(n.GetEdgeCount(), 6);

	NetworkSolver ns;
	Test_Assert(ns.Solve(n, { 999, 555 } ));
	Test_Assert(ns.Solve(n, { 6, 6 } ));
	Test_Assert(!ns.Solve(n, { 6, 3 } ));
}

Case(SimpleLimitNetwork) {
	Network n = make_network().AddRequirement(3)
							  .AddRequirement(6)
							  .AddLimit(3)
							  .AddSources(3)
							  .AddEdgeFromSourceToRequirement(0, 0)
							  .AddEdgeFromSourceToRequirement(1, 1)
							  .AddEdgeFromSourceToRequirement(2, 1)
							  .AddEdgeFromSourceToLimit(2, 0);
	Test_Assert_Eq(n.GetNodeCount(), 8);
	Test_Assert_Eq(n.GetEdgeCount(), 10);

	NetworkSolver ns;
	Test_Assert(ns.Solve(n, { 3, 3, 3 }, { 3 }));
	Test_Assert(!ns.Solve(n, { 3, 0, 6 }, { 6 }));
}

Case(BackflowingNetwork) {
	Network n = make_network().AddRequirement(3)
							  .AddRequirement(6)
							  .AddSources(2)
							  .AddEdgeFromSourceToRequirement(0, 0)
							  .AddEdgeFromSourceToRequirement(0, 1)
							  .AddEdgeFromSourceToRequirement(1, 0);
	Test_Assert_Eq(n.GetNodeCount(), 6);
	Test_Assert_Eq(n.GetEdgeCount(), 7);

	NetworkSolver ns;;
	Test_Assert(!ns.Solve(n, { 3, 3 }));
	Test_Assert(ns.Solve(n, { 9, 0 }));
	Test_Assert(ns.Solve(n, { 6, 3 }));
}

EndSuite(NetworkTest);
