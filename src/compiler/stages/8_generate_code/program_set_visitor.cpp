#include "program_set_visitor.h"
#include "program_set_expression_visitor.h"
#include <limits>

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::ProgramSetVisitor;

ProgramSetVisitor::ProgramSetVisitor(Runtime::Code::RecordTypeTable& recordTypeTable, const std::vector<int>& programArguments, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::ProgramSetVisitor", report), recordTypeTable(recordTypeTable), programArguments(programArguments), result(0), result_record_type(0) {
	return;
}

void ProgramSetVisitor::VisitTypedSetExpression(TypedSetExpression& e) {
	auto type_info = recordTypeTable.GetRecordType(e.ElementType->UniversalUniqueName);
	result_record_type = type_info.type_id;
	result = Runtime::Math::Set(type_info.width, 0, Runtime::Math::Relation::GreaterEqual, std::numeric_limits<Runtime::Math::DefaultFixed>::lowest());
}

void ProgramSetVisitor::VisitConstrainedSetExpression(ConstrainedSetExpression& e) {
	auto type_info = recordTypeTable.GetRecordType(e.ElementType->UniversalUniqueName);
	result_record_type = type_info.type_id;

	ProgramSetExpressionVisitor psev(type_info.width, programArguments, Report);
	e.Filter->Accept(psev);

	result = psev.result;
}
