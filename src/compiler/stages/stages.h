#include <boost/filesystem/path.hpp>

namespace Deg {

namespace Runtime {
namespace Code {
class CodeBuffer;
class FunctionTable;
class RecordTypeTable;
}
}

namespace Compiler {

namespace Diagnostics {
class Report;
}

namespace AST {
class Factory;
class TranslationUnit;
}

namespace SG {
class SymbolTable;
}

namespace IR {
class Printer;
}

namespace Stages {

namespace GenerateAST {
AST::TranslationUnit* GenerateAST(const boost::filesystem::path& filename, AST::Factory& factory, Diagnostics::Report& report);
}

namespace GenerateSG {
void GenerateSG(const std::vector<AST::TranslationUnit*>& units, SG::SymbolTable& symbolTable, Diagnostics::Report& report);
}

namespace ResolveImports {
void ResolveImports(const std::vector<AST::TranslationUnit*>& units, SG::SymbolTable& symbolTable, Diagnostics::Report& report);
}

namespace GenerateMembers {
void GenerateMembers(SG::SymbolTable& symbolTable, Runtime::Code::RecordTypeTable& recordTypeTable, Diagnostics::Report& report);
}

namespace GenerateExpressions {
void GenerateExpressions(SG::SymbolTable& symbolTable, Diagnostics::Report& report);
}

namespace ConstantFolding {
void ConstantFolding(SG::SymbolTable& symbolTable, Diagnostics::Report& report);
}

namespace GenerateSetExpressions {
void GenerateSetExpressions(SG::SymbolTable& symbolTable, Diagnostics::Report& report);
}

namespace GenerateCode {
void GenerateCode(SG::SymbolTable& symbolTable, IR::Printer& code, Runtime::Code::RecordTypeTable& recordTypeTable, Diagnostics::Report& report);
}

}
}
}
