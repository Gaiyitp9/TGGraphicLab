// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/ASTConsumers.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

StatementMatcher LoopMatcher =
        forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl(
                hasInitializer(integerLiteral(equals(0)))))))).bind("forLoop");

class LoopPrinter : public MatchFinder::MatchCallback {
public :
    void run(const MatchFinder::MatchResult &result) override {
        if (const auto *fs = result.Nodes.getNodeAs<clang::ForStmt>("forLoop"))
            fs->dump();
    }
};

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory s_myToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterward.
static cl::extrahelp MoreHelp("\nMore help text...\n");

struct DumpASTAction : public ASTFrontendAction
{
    std::unique_ptr<ASTConsumer>
    CreateASTConsumer(CompilerInstance& ci, StringRef inFile) override
    {
        return CreateASTDumper(nullptr, "", true, true, false, true, ASTDumpOutputFormat::ADOF_Default);
    }
};

int main(int argc, const char **argv) {
    auto expectedParser = CommonOptionsParser::create(argc, argv, s_myToolCategory);
    if (!expectedParser) {
        // Fail gracefully for unsupported options.
        llvm::errs() << expectedParser.takeError();
        return 1;
    }
    CommonOptionsParser& optionsParser = expectedParser.get();
    ClangTool tool(optionsParser.getCompilations(),
                   optionsParser.getSourcePathList());

    LoopPrinter printer;
    MatchFinder finder;
    finder.addMatcher(LoopMatcher, &printer);

//    return tool.run(newFrontendActionFactory(&finder).get());
    return tool.run(newFrontendActionFactory<DumpASTAction>().get());
}