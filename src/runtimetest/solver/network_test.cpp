#include <nullunit/nullunit.h>
#include "runtime/solver/network.h"

using namespace Deg::Runtime::Solver;
using Deg::Runtime::Math::DefaultFixed;

BeginSuite(NetworkTest);

Case(NetworkBuild) {
	Network n = make_network().AddRequirement(DefaultFixed("3"))
							  .AddRequirement(DefaultFixed("6"))
							  .AddLimit(DefaultFixed("6"))
							  .AddLimit(DefaultFixed("3"))
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
	Network n = make_network().AddRequirement(DefaultFixed("3"))
							  .AddSources(1)
							  .AddEdgeFromSourceToRequirement(0, 0);
	Test_Assert_Eq(n.GetNodeCount(), 4);
	Test_Assert_Eq(n.GetEdgeCount(), 3);

	NetworkSolver ns;

	Test_Assert(ns.Solve(n, { DefaultFixed("9999") }));
	Test_Assert(ns.Solve(n, { DefaultFixed("3") }));
	Test_Assert(!ns.Solve(n, { DefaultFixed("1") }));
}

Case(TwoRequirementNetwork) {
	Network n = make_network().AddRequirement(DefaultFixed("3"))
							  .AddRequirement(DefaultFixed("6"))
							  .AddSources(2)
							  .AddEdgeFromSourceToRequirement(0, 0)
							  .AddEdgeFromSourceToRequirement(1, 1);
	Test_Assert_Eq(n.GetNodeCount(), 6);
	Test_Assert_Eq(n.GetEdgeCount(), 6);

	NetworkSolver ns;
	Test_Assert(ns.Solve(n, { DefaultFixed("999"), DefaultFixed("555") } ));
	Test_Assert(ns.Solve(n, { DefaultFixed("6"), DefaultFixed("6") } ));
	Test_Assert(!ns.Solve(n, { DefaultFixed("6"), DefaultFixed("3") } ));
}

Case(SimpleLimitNetwork) {
	Network n = make_network().AddRequirement(DefaultFixed("3"))
							  .AddRequirement(DefaultFixed("6"))
							  .AddLimit(DefaultFixed("3"))
							  .AddSources(3)
							  .AddEdgeFromSourceToRequirement(0, 0)
							  .AddEdgeFromSourceToRequirement(1, 1)
							  .AddEdgeFromSourceToRequirement(2, 1)
							  .AddEdgeFromSourceToLimit(2, 0);
	Test_Assert_Eq(n.GetNodeCount(), 8);
	Test_Assert_Eq(n.GetEdgeCount(), 10);

	NetworkSolver ns;
	Test_Assert(ns.Solve(n, { DefaultFixed("3"), DefaultFixed("3"), DefaultFixed("3") }, { DefaultFixed("3") }));
	Test_Assert(!ns.Solve(n, { DefaultFixed("3"), DefaultFixed("0"), DefaultFixed("6") }, { DefaultFixed("6") }));
}

Case(BackflowingNetwork) {
	Network n = make_network().AddRequirement(DefaultFixed("3"))
							  .AddRequirement(DefaultFixed("6"))
							  .AddSources(2)
							  .AddEdgeFromSourceToRequirement(0, 0)
							  .AddEdgeFromSourceToRequirement(0, 1)
							  .AddEdgeFromSourceToRequirement(1, 0);
	Test_Assert_Eq(n.GetNodeCount(), 6);
	Test_Assert_Eq(n.GetEdgeCount(), 7);

	NetworkSolver ns;;
	Test_Assert(!ns.Solve(n, { DefaultFixed("3"), DefaultFixed("3") }));
	Test_Assert(ns.Solve(n, { DefaultFixed("9"), DefaultFixed("0") }));
	Test_Assert(ns.Solve(n, { DefaultFixed("6"), DefaultFixed("3") }));
}

Case(StackedLimit) {
	Network n = make_network().AddRequirement(DefaultFixed("3"))
							  .AddRequirement(DefaultFixed("6"))
							  .AddLimit(DefaultFixed("3"))
							  .AddLimit(DefaultFixed("6"))
							  .AddSources(3)
							  .AddEdgeFromSourceToRequirement(0, 0)
							  .AddEdgeFromSourceToRequirement(1, 0)
							  .AddEdgeFromSourceToRequirement(1, 1)
							  .AddEdgeFromSourceToRequirement(2, 1)
							  .AddEdgeFromSourceToLimit(2, 0)
							  .AddEdgeFromSourceToLimit(1, 1)
							  .AddEdgeFromLimitToLimit(0, 1);
	Test_Assert_Eq(n.GetNodeCount(), 9);
	Test_Assert_Eq(n.GetEdgeCount(), 13);

	NetworkSolver ns;
	Test_Assert(ns.Solve(n, { DefaultFixed("3"), DefaultFixed("3"), DefaultFixed("3") }, { DefaultFixed("3"), DefaultFixed("6") }));
	Test_Assert(!ns.Solve(n, { DefaultFixed("3"), DefaultFixed("0"), DefaultFixed("6") }, { DefaultFixed("6"), DefaultFixed("6") } ));
	Test_Assert(ns.Solve(n, { DefaultFixed("3"), DefaultFixed("6"), DefaultFixed("0") }, { DefaultFixed("0"), DefaultFixed("6") } ));
}

Case(MiniProgram) {
	// Example program
	// Max 6* in 100-level
	// 3* in 100-level
	// CMPUT 101
	// 6* in CMPUT
	// 6* in anything

	// Student takes:
	// CMPUT 101, 102
	// ENGL 100, 101
	// CMPUT 201, 204
	// MATH 200, 201
	// PHIL 100

	// First: compute disjoint ranges:
	// 200+ \ CMPUT
	// 100 \ CMPUT, ENGL
	// CMPUT 100 \ 101
	// CMPUT 101
	// ENGL 100-level
	// CMPUT 200+
	Network n = make_network().AddRequirement(DefaultFixed("3"))
							  .AddRequirement(DefaultFixed("3"))
							  .AddRequirement(DefaultFixed("6"))
							  .AddRequirement(DefaultFixed("6"))
							  .AddLimit(DefaultFixed("6"))
							  .AddSources(6)
							  .AddEdgeFromSourceToRequirement(0, 3)
							  .AddEdgeFromSourceToRequirement(1, 3)
							  .AddEdgeFromSourceToRequirement(2, 2)
							  .AddEdgeFromSourceToRequirement(2, 3)
							  .AddEdgeFromSourceToRequirement(3, 1)
							  .AddEdgeFromSourceToRequirement(3, 2)
							  .AddEdgeFromSourceToRequirement(3, 3)
							  .AddEdgeFromSourceToRequirement(4, 0)
							  .AddEdgeFromSourceToRequirement(4, 3)
							  .AddEdgeFromSourceToRequirement(5, 2)
							  .AddEdgeFromSourceToRequirement(5, 3)
							  .AddEdgeFromSourceToLimit(1, 0)
							  .AddEdgeFromSourceToLimit(2, 0)
							  .AddEdgeFromSourceToLimit(3, 0)
							  .AddEdgeFromSourceToLimit(4, 0);
	Test_Assert_Eq(n.GetNodeCount(), 13);
	Test_Assert_Eq(n.GetEdgeCount(), 26);

	NetworkSolver ns;
	Test_Assert(ns.Solve(n, { DefaultFixed("6"), DefaultFixed("3"), DefaultFixed("3"), DefaultFixed("3"), DefaultFixed("6"), DefaultFixed("6") }, { DefaultFixed("15") }));
}

EndSuite(NetworkTest);
