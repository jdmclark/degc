#include <boost/filesystem/path.hpp>

namespace Deg {
namespace Compiler {

namespace Diagnostics {
class Report;
}

namespace AST {
class Factory;
class TranslationUnit;
}

namespace Stages {

namespace GenerateAST {
AST::TranslationUnit* GenerateAST(const boost::filesystem::path& filename, AST::Factory& Factory, Diagnostics::Report& Report);
}

}
}
}
