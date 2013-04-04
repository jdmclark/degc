#include <nullunit/nullunit.h>
#include "compilertest/codegen_test_fixture.h"
#include "runtime/solver/linearrecordtable.h"

using Deg::Runtime::Math::DefaultFixed;
using Deg::Runtime::Solver::Record;

enum class Subject {
	CMPUT = 0,
	MATH = 1,
	ENGL = 2,
	PHIL = 3
};

enum class Faculty {
	SCI = 0,
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

	void AddCourse(Faculty fac, Subject sub, DefaultFixed level, DefaultFixed credits) {
		Deg::Runtime::Solver::RecordTable* rt = GetTakenCourseRecordTable();
		rt->AddRecord({ credits, level, DefaultFixed(static_cast<int>(sub)), DefaultFixed(static_cast<int>(fac)) });
	}
};

BeginSuiteFixture(ProgramCodegenTest, ProgramCodegenTestFixture);

Case(Example) {
	ParseFiles({ "base.deg", "example.deg" });
	AssertResult(0, 0);

	AddCourse(Faculty::SCI, Subject::CMPUT, DefaultFixed("101"), DefaultFixed("3"));
	AddCourse(Faculty::SCI, Subject::CMPUT, DefaultFixed("102"), DefaultFixed("3"));
	AddCourse(Faculty::AR, Subject::ENGL, DefaultFixed("100"), DefaultFixed("3"));
	AddCourse(Faculty::AR, Subject::ENGL, DefaultFixed("101"), DefaultFixed("3"));
	AddCourse(Faculty::SCI, Subject::CMPUT, DefaultFixed("201"), DefaultFixed("3"));
	AddCourse(Faculty::SCI, Subject::CMPUT, DefaultFixed("204"), DefaultFixed("3"));
	AddCourse(Faculty::SCI, Subject::MATH, DefaultFixed("200"), DefaultFixed("3"));
	AddCourse(Faculty::SCI, Subject::MATH, DefaultFixed("201"), DefaultFixed("3"));
	AddCourse(Faculty::AR, Subject::PHIL, DefaultFixed("100"), DefaultFixed("3"));

	auto prog = programTable.GetProgram("ca.nullptr.TestProgram");
	Test_Assert(prog->Solve(recordIndex, networkSolver));
}

EndSuite(ProgramCodegenTest);
