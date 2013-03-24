#include <nullunit/nullunit.h>
#include "compilertest/codegen_test_fixture.h"

using Deg::Runtime::Math::DefaultFixed;

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

Case(Simple) {
	ParseFile("simple.deg");
	AssertResult(0, 0);

	size_t fn = functionTable.GetFunction("ca.nullptr.Simple");

	Test_Assert_Eq(vm.Call<DefaultFixed>(fn), DefaultFixed("5"));
}

EndSuite(FunctionCodegenTest);