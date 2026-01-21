/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendActions.h"
#include "gtest/gtest.h"
#include "GlobalArgs.h"

namespace TG::Reflection
{
    using namespace clang;
    using namespace clang::ast_matchers;
    using namespace clang::tooling;

    class LoopPrinter final : public MatchFinder::MatchCallback
    {
    public :
        void run(const MatchFinder::MatchResult &result) override
        {
            if (const auto *fs = result.Nodes.getNodeAs<ForStmt>("forLoop"))
                fs->dump();
        }
    };

    class AnnotatePrinter : public MatchFinder::MatchCallback
    {
    public :
        void run(const MatchFinder::MatchResult &result) override
        {
            if (const auto *declNode = result.Nodes.getNodeAs<CXXRecordDecl>("id"))
                FoundRecord(declNode);
            if (const auto *fieldNode = result.Nodes.getNodeAs<FieldDecl>("id"))
                FoundField(fieldNode);
            if (const auto *functionNode = result.Nodes.getNodeAs<FunctionDecl>("id"))
                FoundFunction(functionNode);
        }

    private:
        void FoundRecord(CXXRecordDecl const *record)
        {
            record->dump();
            //        llvm::outs() << record->getName() << "\n";
        }

        void FoundField(FieldDecl const *field)
        {
            field->dump();
            //        llvm::outs() << field->getName() << "\n";
        }

        void FoundFunction(FunctionDecl const* function)
        {
            function->dump();
            //        llvm::outs() << function->getName() << "\n";
        }
    };

    struct DumpAstAction : public ASTFrontendAction
    {
        std::unique_ptr<ASTConsumer>
        CreateASTConsumer(CompilerInstance &ci, StringRef inFile) override
        {
            return CreateASTDumper(nullptr, "", true, true, false, true, ASTDumpOutputFormat::ADOF_Default);
        }
    };

    class CustomClassVisitor : public RecursiveASTVisitor<CustomClassVisitor>
    {
    public:
        explicit CustomClassVisitor(ASTContext& context) : m_context(context) {}

        bool VisitCXXRecordDecl(CXXRecordDecl *declaration)
        {
            declaration->dump();
            if (declaration->getQualifiedNameAsString() == "n::m::C")
            {
                FullSourceLoc fullLocation = m_context.getFullLoc(declaration->getBeginLoc());
                if (fullLocation.isValid())
                {
                    llvm::outs() << "Found declaration at "
                                 << fullLocation.getSpellingLineNumber() << ";"
                                 << fullLocation.getSpellingColumnNumber() << "\n";
                }
            }
            return true;
        }

        bool VisitEnumDecl(EnumDecl *declaration)
        {
            declaration->dump();
            for (auto it = declaration->enumerator_begin(); it != declaration->enumerator_end(); ++it)
                llvm::outs() << it->getNameAsString() << '\n';
            return true;
        }

    private:
        ASTContext& m_context;
    };

    class CustomConsumer : public ASTConsumer
    {
    public:
        explicit CustomConsumer(ASTContext& context) : m_visitor(context) {}

        void HandleTranslationUnit(ASTContext &context) override
        {
            m_visitor.TraverseDecl(context.getTranslationUnitDecl());
        }

    private:
        CustomClassVisitor m_visitor;
    };

    struct CustomAction : public ASTFrontendAction
    {
        std::unique_ptr<ASTConsumer>
        CreateASTConsumer(CompilerInstance &ci, StringRef inFile) override
        {
            return std::make_unique<CustomConsumer>(ci.getASTContext());
        }

    };

    TEST(LibTooling, SyntaxChecker)
    {
        // 创建选项分类，将工具的选项放在该类别下，方便查看
        static llvm::cl::OptionCategory myToolCategory("my-tool options");
        // 添加CommonOptionsParser的提示信息
        static llvm::cl::extrahelp commonHelp(CommonOptionsParser::HelpMessage);
        // 添加帮助信息
        static llvm::cl::extrahelp moreHelp("\nMore help text...\n");

        // int argc = 2;
        // const char* argv[] {
        //     "E:\\lch\\TGGraphicLab\\cmake-build-debug\\Tests\\Reflection\\Reflection.exe",
        //     "--help",
        //     nullptr
        // };

        int argc = static_cast<int>(gArgv.size() - 1);
        const char** argv = gArgv.data();
        auto expectedParser = CommonOptionsParser::create(argc, argv, myToolCategory);
        if (!expectedParser)
        {
            // 对于不支持的选项，打印错误信息
            llvm::errs() << expectedParser.takeError();
            return;
        }
        CommonOptionsParser& optionsParser = expectedParser.get();

        ClangTool tool(optionsParser.getCompilations(),
                       optionsParser.getSourcePathList());

        // tool.run(newFrontendActionFactory<SyntaxOnlyAction>().get());
        tool.run(newFrontendActionFactory<DumpAstAction>().get());

        // StatementMatcher loopMatcher =
        // forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl(
        // hasInitializer(integerLiteral(equals(0)))))))).bind("forLoop");

        // DeclarationMatcher classMatcher = cxxRecordDecl(hasAttr(attr::Annotate)).bind("id");
        // DeclarationMatcher fieldMatcher = fieldDecl(hasAttr(attr::Annotate)).bind("id");
        // DeclarationMatcher functionMatcher = functionDecl(hasAttr(attr::Annotate)).bind("id");

        // LoopPrinter printer;
        // AnnotatePrinter printer2;
        // MatchFinder finder;
        // finder.addMatcher(loopMatcher, &printer);
        // finder.addMatcher(classMatcher, &printer2);
        // finder.addMatcher(fieldMatcher, &printer2);
        // finder.addMatcher(functionMatcher, &printer2);

        // tool.run(newFrontendActionFactory(&finder).get());
        // tool.run(newFrontendActionFactory<CustomAction>().get());
        // runToolOnCode(std::make_unique<FindNamedClassAction>(), argv[1]);
    }
}
