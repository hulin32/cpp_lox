// Copyright 2020 <Copyright hulin>

#ifndef TOOL_GENERATEAST_H_
#define TOOL_GENERATEAST_H_

#include <string>
#include <vector>
#include <iostream>

using std::vector;
using std::string;
using std::ofstream;

class GenerateAst {
 private:
    static void defineAst(
       string outputDir,
       string baseName,
       vector<string> types);
    static void defineType(
       ofstream* outfile,
       string baseName,
       string className,
       string fields);
 public:
    void execute(string outputDir);
};

#endif  // TOOL_GENERATEAST_H_
