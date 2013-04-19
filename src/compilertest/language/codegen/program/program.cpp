#include <nullunit/nullunit.h>
#include "compilertest/codegen_test_fixture.h"
#include "runtime/solver/linearrecordtable.h"
#include <iostream>

using Deg::Runtime::Math::DefaultFixed;
using Deg::Runtime::Solver::Record;

enum class Subject {
	CMPUT = 0,
	MATH = 1,
	ENGL = 2,
	PHIL = 3
};

enum class Faculty {
	SC = 0,
	AR = 1
};

class ProgramCodegenTestFixture : public CodegenTestFixture {
private:
	Deg::Runtime::Solver::RecordTable* GetTakenCourseRecordTable() {
		auto record_type_info = recordTypeTable.GetRecordType("ca.nullptr.TakenCourse");
		Deg::Runtime::Solver::RecordTable* rt = recordIndex.GetRecordTable(record_type_info.type_id);
		if(!rt) {
			rt = new Deg::Runtime::Solver::LinearRecordTable(record_type_info.width, record_type_info.quantity);
			std::unique_ptr<Deg::Runtime::Solver::RecordTable> mg_rt(rt);
			recordIndex.AddRecordTable(record_type_info.type_id, mg_rt);
		}

		return rt;
	}

public:
	Deg::Runtime::Solver::NetworkSolver networkSolver;

	ProgramCodegenTestFixture()
		: CodegenTestFixture("src/compilertest/language/codegen/program") {
		return;
	}

	void AddCourse(Faculty fac, Subject sub, const std::string& level, const std::string& credits) {
		Deg::Runtime::Solver::RecordTable* rt = GetTakenCourseRecordTable();
		rt->AddRecord({ DefaultFixed(credits), DefaultFixed(level), DefaultFixed(static_cast<int>(sub)), DefaultFixed(static_cast<int>(fac)) });
	}
};

BeginSuiteFixture(ProgramCodegenTest, ProgramCodegenTestFixture);

Case(Args1) {
	ParseFiles({ "base.deg", "args.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "6");
	AddCourse(Faculty::AR, Subject::ENGL, "101", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver, { static_cast<int>(Subject::CMPUT) }));
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver, { static_cast<int>(Subject::ENGL) }));
}

Case(Args2) {
	ParseFiles({ "base.deg", "args.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(!prog->Solve(recordIndex, vm, networkSolver, { static_cast<int>(Subject::ENGL) }));
}

Case(AssertAlways) {
	ParseFiles({ "base.deg", "assert_always.deg" });
	AssertResult(0, 0);

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(AssertNever) {
	ParseFiles({ "base.deg", "assert_never.deg" });
	AssertResult(0, 0);

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(!prog->Solve(recordIndex, vm, networkSolver));
}

Case(BaseArgs1) {
	ParseFiles({ "base.deg", "base_args.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "3");
	AddCourse(Faculty::AR, Subject::ENGL, "101", "6");
	AddCourse(Faculty::AR, Subject::PHIL, "101", "3");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver, { static_cast<int>(Subject::ENGL), static_cast<int>(Subject::PHIL) }));
}

Case(BaseArgs2) {
	ParseFiles({ "base.deg", "base_args.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "12");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver, { static_cast<int>(Subject::CMPUT), static_cast<int>(Subject::CMPUT) }));
}

Case(DeepNestedLimit) {
	ParseFiles({ "base.deg", "deep_nested_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "6");
	AddCourse(Faculty::SC, Subject::MATH, "101", "6");
	AddCourse(Faculty::AR, Subject::ENGL, "101", "6");
	AddCourse(Faculty::SC, Subject::PHIL, "101", "1500");
	AddCourse(Faculty::AR, Subject::PHIL, "102", "1500");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(!prog->Solve(recordIndex, vm, networkSolver));
}

Case(DisjointLimit) {
	ParseFiles({ "base.deg", "disjoint_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "3");
	AddCourse(Faculty::SC, Subject::CMPUT, "102", "3");
	AddCourse(Faculty::AR, Subject::ENGL, "101", "3");
	AddCourse(Faculty::AR, Subject::ENGL, "102", "3");
	AddCourse(Faculty::AR, Subject::PHIL, "200", "3");
	AddCourse(Faculty::SC, Subject::MATH, "101", "3");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(EitherOr1) {
	ParseFiles({ "base.deg", "either_or.deg" });
	AssertResult(0, 0);

	// Branch 1
	AddCourse(Faculty::SC, Subject::CMPUT, "101", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(EitherOr2) {
	ParseFiles({ "base.deg", "either_or.deg" });
	AssertResult(0, 0);

	// Branch 2
	AddCourse(Faculty::SC, Subject::CMPUT, "101", "3");
	AddCourse(Faculty::SC, Subject::MATH, "101", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(EitherOrAlways) {
	ParseFiles({ "base.deg", "either_or_always.deg" });
	AssertResult(0, 0);

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(EitherOrCorrectBranch1) {
	ParseFiles({ "base.deg", "either_or_correct_branch.deg" });
	AssertResult(0, 0);

	// Having a single Philosophy course triggers branch 2
	AddCourse(Faculty::AR, Subject::PHIL, "101", "3");
	AddCourse(Faculty::AR, Subject::ENGL, "101", "3");
	AddCourse(Faculty::SC, Subject::MATH, "101", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(EitherOrCorrectBranch2) {
	ParseFiles({ "base.deg", "either_or_correct_branch.deg" });
	AssertResult(0, 0);

	// Having no Philosophy courses triggers branch 1
	AddCourse(Faculty::SC, Subject::CMPUT, "101", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(EitherOrLimit1) {
	ParseFiles({ "base.deg", "either_or_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "201", "12");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(EitherOrLimit2) {
	ParseFiles({ "base.deg", "either_or_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "100", "6");
	AddCourse(Faculty::SC, Subject::MATH, "100", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(EitherOrNever) {
	ParseFiles({ "base.deg", "either_or_never.deg" });
	AssertResult(0, 0);

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(!prog->Solve(recordIndex, vm, networkSolver));
}

Case(EitherOr3) {
	ParseFiles({ "base.deg", "either_or.deg" });
	AssertResult(0, 0);

	// Branch 1
	AddCourse(Faculty::AR, Subject::ENGL, "101", "9");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(EmbedParameterized) {
	ParseFiles({ "base.deg", "embed_parameterized.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "6");
	AddCourse(Faculty::AR, Subject::ENGL, "101", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver, { static_cast<int>(Subject::CMPUT), static_cast<int>(Subject::ENGL) }));
}

Case(EmbedParametric) {
	ParseFiles({ "base.deg", "embed_parametric.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "6");
	AddCourse(Faculty::AR, Subject::ENGL, "101", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver, { static_cast<int>(Subject::CMPUT) }));
	Test_Assert(!prog->Solve(recordIndex, vm, networkSolver, { static_cast<int>(Subject::ENGL) }));
}

Case(EmbedStatic) {
	ParseFiles({ "base.deg", "embed_static.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "6");
	AddCourse(Faculty::AR, Subject::ENGL, "101", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(Example) {
	ParseFiles({ "base.deg", "example.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "3");
	AddCourse(Faculty::SC, Subject::CMPUT, "102", "3");
	AddCourse(Faculty::AR, Subject::ENGL, "100", "3");
	AddCourse(Faculty::AR, Subject::ENGL, "101", "3");
	AddCourse(Faculty::SC, Subject::CMPUT, "201", "3");
	AddCourse(Faculty::SC, Subject::CMPUT, "204", "3");
	AddCourse(Faculty::SC, Subject::MATH, "200", "3");
	AddCourse(Faculty::SC, Subject::MATH, "201", "3");
	AddCourse(Faculty::AR, Subject::PHIL, "100", "3");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(IfAlways) {
	ParseFiles({ "base.deg", "if_always.deg" });
	AssertResult(0, 0);

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(IfElseAlways) {
	ParseFiles({ "base.deg", "if_else_always.deg" });
	AssertResult(0, 0);

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(IfElseNever) {
	ParseFiles({ "base.deg", "if_else_never.deg" });
	AssertResult(0, 0);

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(IfNever) {
	ParseFiles({ "base.deg", "if_never.deg" });
	AssertResult(0, 0);

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(IfTakeLimit1) {
	ParseFiles({ "base.deg", "if_take_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::AR, Subject::PHIL, "101", "12");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(!prog->Solve(recordIndex, vm, networkSolver));
}

Case(IfTakeLimit2) {
	ParseFiles({ "base.deg", "if_take_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::MATH, "0", "0");
	AddCourse(Faculty::AR, Subject::PHIL, "201", "12");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(IfTakeLimit3) {
	ParseFiles({ "base.deg", "if_take_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::MATH, "100", "12");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(!prog->Solve(recordIndex, vm, networkSolver));
}

Case(IfTakeLimit4) {
	ParseFiles({ "base.deg", "if_take_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::MATH, "100", "12");
	AddCourse(Faculty::AR, Subject::ENGL, "100", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(IfTakeLimit5) {
	ParseFiles({ "base.deg", "if_take_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::MATH, "200", "12");
	AddCourse(Faculty::SC, Subject::CMPUT, "100", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(!prog->Solve(recordIndex, vm, networkSolver));
}

Case(IfTakeLimit6) {
	ParseFiles({ "base.deg", "if_take_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::MATH, "200", "12");
	AddCourse(Faculty::SC, Subject::CMPUT, "100", "6");
	AddCourse(Faculty::SC, Subject::CMPUT, "495", "3");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(OverlapLimit) {
	ParseFiles({ "base.deg", "overlap_limit.deg" });
	AssertResult(1, 0);
}

Case(SubsetLimit) {
	ParseFiles({ "base.deg", "subset_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "3");
	AddCourse(Faculty::SC, Subject::CMPUT, "102", "3");
	AddCourse(Faculty::SC, Subject::MATH, "101", "3");
	AddCourse(Faculty::SC, Subject::MATH, "102", "3");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(!prog->Solve(recordIndex, vm, networkSolver));
}

Case(SubsetLimitPass) {
	ParseFiles({ "base.deg", "subset_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "3");
	AddCourse(Faculty::SC, Subject::CMPUT, "102", "3");
	AddCourse(Faculty::SC, Subject::MATH, "101", "3");
	AddCourse(Faculty::SC, Subject::MATH, "102", "3");
	AddCourse(Faculty::AR, Subject::PHIL, "101", "3");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");

	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

Case(SubsetLimitPass2) {
	ParseFiles({ "base.deg", "subset_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::MATH, "101", "3");
	AddCourse(Faculty::SC, Subject::MATH, "102", "3");
	AddCourse(Faculty::SC, Subject::MATH, "103", "3");
	AddCourse(Faculty::AR, Subject::PHIL, "101", "3");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, vm, networkSolver));
}

EndSuite(ProgramCodegenTest);
