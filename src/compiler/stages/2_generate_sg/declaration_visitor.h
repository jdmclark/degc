#pragma once

#include <string>
#include "compiler/ast/visitor.h"
#include "compiler/sg/module.h"
#include "compiler/sg/error_helper.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateSG {

class DeclarationVisitor : public AST::Visitor {
private:
	const std::string& module_name;
	SG::Module& module;

public:
	DeclarationVisitor(const std::string& module_name, SG::Module& module, Diagnostics::Report& report);

	void VisitProgram(AST::Program& n);
	void VisitEnumeration(AST::Enumeration& n);
	void VisitFunction(AST::Function& n);
	void VisitRecord(AST::Record& n);
};

}
}
}
}
