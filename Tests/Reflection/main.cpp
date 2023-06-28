#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/ASTConsumers.h"
#include <iostream>

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

class LoopPrinter : public MatchFinder::MatchCallback
{
public :
    void run(const MatchFinder::MatchResult &result) override
    {
        if (const auto *fs = result.Nodes.getNodeAs<clang::ForStmt>("forLoop"))
            fs->dump();
    }
};

// 创建选项分类，将工具的选项放在该类别下，方便查看
static llvm::cl::OptionCategory gs_myToolCategory("my-tool options");

struct DumpAstAction : public ASTFrontendAction
{
    std::unique_ptr<ASTConsumer>
    CreateASTConsumer(CompilerInstance &ci, StringRef inFile) override
    {
        return CreateASTDumper(nullptr, "", true, true, false, true, ASTDumpOutputFormat::ADOF_Default);
    }
};

int main(int argc, const char **argv)
{
    StatementMatcher loopMatcher =
            forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl(
                    hasInitializer(integerLiteral(equals(0)))))))).bind("forLoop");

    // 添加CommonOptionsParser的提示信息
    static llvm::cl::extrahelp commonHelp(CommonOptionsParser::HelpMessage);
    // 添加帮助信息
    static llvm::cl::extrahelp moreHelp("\nMore help text...\n");

    auto expectedParser = CommonOptionsParser::create(argc, argv, gs_myToolCategory);
    if (!expectedParser)
    {
        // 对于不支持的选项，打印错误信息
        llvm::errs() << expectedParser.takeError();
        return 1;
    }
    CommonOptionsParser& optionsParser = expectedParser.get();

    ClangTool tool(optionsParser.getCompilations(),
                   optionsParser.getSourcePathList());

    LoopPrinter printer;
    MatchFinder finder;
    finder.addMatcher(loopMatcher, &printer);

//    return tool.run(newFrontendActionFactory(&finder).get());
    return tool.run(newFrontendActionFactory<DumpAstAction>().get());
}