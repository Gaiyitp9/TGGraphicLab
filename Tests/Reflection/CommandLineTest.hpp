#pragma once

#include "clang/Tooling/CommonOptionsParser.h"
#include <iostream>

using namespace llvm;

namespace TG
{
    // 添加帮助信息
    static cl::extrahelp gs_moreHelp("\nMore help text?...\n");

    static cl::opt<std::string> gs_outputFilename("o", cl::desc("Specify output filename"), cl::value_desc("filename"));
    static cl::list<std::string> gs_inputFilenameLists(cl::Positional, cl::desc("<input file>"), cl::OneOrMore);

    static cl::opt<bool> gs_force("f", cl::desc("Enable binary output on terminals"));
    static cl::opt<bool> gs_quiet("quiet", cl::desc("Don't print informational messages"));
    static cl::alias gs_quiet2("q", cl::desc("Alias for -quiet"), cl::aliasopt(gs_quiet));

    enum OptLevel
    {
        Debug,
        O1,
        O2,
        O3
    };

    static cl::opt<OptLevel> gs_optimizationLevel(cl::desc("Choose optimization level:"),
                                                  cl::values(
                                                          clEnumValN(Debug, "g", "No optimizations, enable debug"),
                                                          clEnumVal(O1, "Enable trivial optimizations"),
                                                          clEnumVal(O2, "Enable default optimizations"),
                                                          clEnumVal(O3, "Enable expensive optimizations")));
}

void CommandLineTest(int argc, const char **argv)
{
    cl::ParseCommandLineOptions(argc, argv);
    std::cout << TG::gs_outputFilename << std::endl;
    for (const auto & inputFilename : TG::gs_inputFilenameLists)
        std::cout << inputFilename << std::endl;
    std::cout << std::boolalpha << TG::gs_force << std::endl;
    std::cout << TG::gs_optimizationLevel << std::endl;
}