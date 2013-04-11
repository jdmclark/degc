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
	Test_Assert(prog->Solve(recordIndex, networkSolver, { static_cast<int>(Subject::CMPUT) }));
	Test_Assert(prog->Solve(recordIndex, networkSolver, { static_cast<int>(Subject::ENGL) }));
}

Case(Args2) {
	ParseFiles({ "base.deg", "args.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(!prog->Solve(recordIndex, networkSolver, { static_cast<int>(Subject::ENGL) }));
}

Case(BaseArgs1) {
	ParseFiles({ "base.deg", "base_args.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "3");
	AddCourse(Faculty::AR, Subject::ENGL, "101", "6");
	AddCourse(Faculty::AR, Subject::PHIL, "101", "3");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, networkSolver, { static_cast<int>(Subject::ENGL), static_cast<int>(Subject::PHIL) }));
}

Case(BaseArgs2) {
	ParseFiles({ "base.deg", "base_args.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::CMPUT, "101", "12");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, networkSolver, { static_cast<int>(Subject::CMPUT), static_cast<int>(Subject::CMPUT) }));
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
	Test_Assert(!prog->Solve(recordIndex, networkSolver));
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
	Test_Assert(prog->Solve(recordIndex, networkSolver));
}

Case(EitherOr1) {
	ParseFiles({ "base.deg", "either_or.deg" });
	AssertResult(0, 0);

	// Branch 1
	AddCourse(Faculty::SC, Subject::CMPUT, "101", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, networkSolver));
}

Case(EitherOr2) {
	ParseFiles({ "base.deg", "either_or.deg" });
	AssertResult(0, 0);

	// Branch 2
	AddCourse(Faculty::SC, Subject::CMPUT, "101", "3");
	AddCourse(Faculty::SC, Subject::MATH, "101", "6");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, networkSolver));
}

Case(EitherOr3) {
	ParseFiles({ "base.deg", "either_or.deg" });
	AssertResult(0, 0);

	// Branch 1
	AddCourse(Faculty::AR, Subject::ENGL, "101", "9");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, networkSolver));
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
	Test_Assert(prog->Solve(recordIndex, networkSolver));
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
	Test_Assert(!prog->Solve(recordIndex, networkSolver));
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

	Test_Assert(prog->Solve(recordIndex, networkSolver));
}

Case(SubsetLimitPass2) {
	ParseFiles({ "base.deg", "subset_limit.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SC, Subject::MATH, "101", "3");
	AddCourse(Faculty::SC, Subject::MATH, "102", "3");
	AddCourse(Faculty::SC, Subject::MATH, "103", "3");
	AddCourse(Faculty::AR, Subject::PHIL, "101", "3");

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, networkSolver));
}

EndSuite(ProgramCodegenTest);
