#include <nullunit/nullunit.h>
#include "compilertest/codegen_test_fixture.h"
#include "runtime/solver/linearrecordtable.h"
#include "boost/format.hpp"

using Deg::Runtime::Math::DefaultFixed;
using Deg::Runtime::Solver::Record;

class FunctionCodegenTestFixture : public CodegenTestFixture {
public:
	FunctionCodegenTestFixture()
		: CodegenTestFixture("src/compilertest/language/codegen/function") {
		return;
	}
};

BeginSuiteFixture(FunctionCodegenTest, FunctionCodegenTestFixture);

Case(ArgPass) {
	ParseFile("arg_pass.deg");
	AssertResult(0, 0);

	size_t fn = functionTable.GetFunction("ca.nullptr.ArgPass");

	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, DefaultFixed("984.5")), DefaultFixed("984.5"));
}

Case(Arithmetic) {
	ParseFile("arithmetic.deg");
	AssertResult(0, 0);

	Test_Assert_Eq(vm.Call<DefaultFixed>(functionTable.GetFunction("ca.nullptr.Add"),
		DefaultFixed("1"), DefaultFixed("3")), DefaultFixed("4"));

	Test_Assert_Eq(vm.Call<DefaultFixed>(functionTable.GetFunction("ca.nullptr.Sub"),
		DefaultFixed("12"), DefaultFixed("5")), DefaultFixed("7"));

	Test_Assert_Eq(vm.Call<DefaultFixed>(functionTable.GetFunction("ca.nullptr.Mul"),
		DefaultFixed("3"), DefaultFixed("4")), DefaultFixed("12"));

	Test_Assert_Eq(vm.Call<DefaultFixed>(functionTable.GetFunction("ca.nullptr.Div"),
		DefaultFixed("7"), DefaultFixed("2")), DefaultFixed("3.5"));
}

Case(EnumArg) {
	ParseFile("enum_arg.deg");
	AssertResult(0, 0);

	Test_Assert(vm.Call<bool>(functionTable.GetFunction("ca.nullptr.Test1")));
	Test_Assert(vm.Call<bool>(functionTable.GetFunction("ca.nullptr.Test2")));
}

Case(Exists) {
	ParseFile("exists.deg");
	AssertResult(0, 0);

	Deg::Runtime::Code::RecordTypeInfo rti = recordTypeTable.GetRecordType("ca.nullptr.TestRecord");

	std::unique_ptr<Deg::Runtime::Solver::RecordTable> rt(new Deg::Runtime::Solver::LinearRecordTable(rti.width, 0));
	for(int i = 1; i < 20; ++i) {
		rt->AddRecord({ DefaultFixed(boost::str(boost::format("%s") % i)), DefaultFixed("5"), DefaultFixed("10") });
	}

	recordIndex.AddRecordTable(rti.type_id, rt);

	size_t fn = functionTable.GetFunction("ca.nullptr.ExistsRange");

	Test_Assert(vm.Call<bool>(fn, DefaultFixed("0.5"), DefaultFixed("1.5")));
	Test_Assert(!vm.Call<bool>(fn, DefaultFixed("0.0"), DefaultFixed("0.9")));
	Test_Assert(vm.Call<bool>(fn, DefaultFixed("18.5"), DefaultFixed("19")));
	Test_Assert(vm.Call<bool>(fn, DefaultFixed("1"), DefaultFixed("20")));
}

Case(Factorial) {
	ParseFile("factorial.deg");
	AssertResult(0, 0);

	size_t fn = functionTable.GetFunction("ca.nullptr.Factorial");

	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, DefaultFixed("1")), DefaultFixed("1"));
	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, DefaultFixed("2")), DefaultFixed("2"));
	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, DefaultFixed("3")), DefaultFixed("6"));
	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, DefaultFixed("4")), DefaultFixed("24"));
	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, DefaultFixed("0")), DefaultFixed("1"));
	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, DefaultFixed("-1")), DefaultFixed("1"));
	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, DefaultFixed("10")), DefaultFixed("3628800"));
}

Case(FunctionHigherOrder) {
	ParseFile("function_higherorder.deg");
	AssertResult(0, 0);

	size_t fn = functionTable.GetFunction("ca.nullptr.map_sum");
	size_t fn_sq = functionTable.GetFunction("ca.nullptr.square");
	size_t fn_dbl = functionTable.GetFunction("ca.nullptr.double");

	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, fn_sq, DefaultFixed("1"), DefaultFixed("3"), DefaultFixed("2")), DefaultFixed("14"));
	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, fn_dbl, DefaultFixed("1"), DefaultFixed("3"), DefaultFixed("2")), DefaultFixed("12"));
}

Case(FunctionIfElse) {
	ParseFile("function_ifelse.deg");
	AssertResult(0, 0);

	size_t fn = functionTable.GetFunction("ca.nullptr.Branch");

	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, DefaultFixed("0")), DefaultFixed("7"));
	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, DefaultFixed("1")), DefaultFixed("5"));
	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, DefaultFixed("2")), DefaultFixed("6"));
	Test_Assert_Eq(vm.Call<DefaultFixed>(fn, DefaultFixed("3")), DefaultFixed("7"));
}

Case(Logical) {
	ParseFile("logical.deg");
	AssertResult(0, 0);

	size_t fn_not = functionTable.GetFunction("ca.nullptr.Not");
	size_t fn_and = functionTable.GetFunction("ca.nullptr.And");
	size_t fn_or = functionTable.GetFunction("ca.nullptr.Or");

	Test_Assert_Eq(vm.Call<bool>(fn_not, false), true);
	Test_Assert_Eq(vm.Call<bool>(fn_not, true), false);

	Test_Assert_Eq(vm.Call<bool>(fn_and, true, true), true);
	Test_Assert_Eq(vm.Call<bool>(fn_and, true, false), false);
	Test_Assert_Eq(vm.Call<bool>(fn_and, false, true), false);
	Test_Assert_Eq(vm.Call<bool>(fn_and, false, false), false);

	Test_Assert_Eq(vm.Call<bool>(fn_or, true, true), true);
	Test_Assert_Eq(vm.Call<bool>(fn_or, true, false), true);
	Test_Assert_Eq(vm.Call<bool>(fn_or, false, true), true);
	Test_Assert_Eq(vm.Call<bool>(fn_or, false, false), false);
}

Case(Panic) {
	ParseFile("panic.deg");
	AssertResult(0, 0);

	try {
		vm.Call<bool>(functionTable.GetFunction("ca.nullptr.TestFunction"));
	}
	catch(...) {
		return;
	}

	Test_Assert_Always("panic failed to throw exception");
}

Case(RecordArg) {
	ParseFile("record_arg.deg");
	AssertResult(0, 0);

	size_t fn = functionTable.GetFunction("ca.nullptr.MaxGlobSumBC");

	Test_Assert_Eq(vm.Call<DefaultFixed>(fn,
			Record({ DefaultFixed("1"), DefaultFixed("2"), DefaultFixed("3") }),
			Record({ DefaultFixed("9"), DefaultFixed("5"), DefaultFixed("10") }),
			Record({ DefaultFixed("2"), DefaultFixed("8"), DefaultFixed("9") })),
			DefaultFixed("15"));
}

Case(SimpleExists) {
	ParseFile("simple_exists.deg");
	AssertResult(0, 0);

	Deg::Runtime::Code::RecordTypeInfo rti = recordTypeTable.GetRecordType("ca.nullptr.TestRecord");

	std::unique_ptr<Deg::Runtime::Solver::RecordTable> rt(new Deg::Runtime::Solver::LinearRecordTable(rti.width, 0));
	for(int i = 1; i < 20; ++i) {
		rt->AddRecord({ DefaultFixed(boost::str(boost::format("%s") % i)), DefaultFixed("5"), DefaultFixed("10") });
	}

	recordIndex.AddRecordTable(rti.type_id, rt);

	Test_Assert(vm.Call<bool>(functionTable.GetFunction("ca.nullptr.ExistsSet")));
}

Case(Simple) {
	ParseFile("simple.deg");
	AssertResult(0, 0);

	size_t fn = functionTable.GetFunction("ca.nullptr.Simple");

	Test_Assert_Eq(vm.Call<DefaultFixed>(fn), DefaultFixed("5"));
}

EndSuite(FunctionCodegenTest);
