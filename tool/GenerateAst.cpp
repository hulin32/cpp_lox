// Copyright 2020 <Copyright hulin>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "./tool/GenerateAst.h"

using std::string;
using std::vector;
using std::ofstream;
using std::endl;

string trim(string str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}
vector<string> split(string source, string delimiter) {
    vector<string> result;
    size_t pos = 0;
    string token;
    while ((pos = source.find(delimiter)) != string::npos) {
        token = source.substr(0, pos);
        result.push_back(trim(token));
        source.erase(0, pos + delimiter.length());
    }
    return result;
}

void GenerateAst::execute(string outputDir) {
    vector<string> ast = {
        "Binary   : Expr left, Token operator, Expr right",
        "Grouping : Expr expression",
        "Literal  : Object value",
        "Unary    : Token operator, Expr right"
    };
    defineAst(outputDir, "Expr", ast);
}

void GenerateAst::defineAst(
    string outputDir,
    string baseName,
    vector<string> types) {
    string path = outputDir + "/" + baseName + ".cpp";
    std::ofstream outfile(path);
    outfile << "// my text here!" << std::endl;
    outfile << "class " + baseName + " {" << endl;

    // The AST classes.
    for (string type : types) {
      string className = split(type, ":")[0];
      string fields = split(type, ":")[1];
      defineType(&outfile, baseName, className, fields);
    }

    outfile << "}" << endl;
    outfile.close();
}

void GenerateAst::defineType(
    ofstream* outfile,
    string baseName,
    string className,
    string fields) {
    *outfile << "  static class " + className + " : public " +
    baseName + " {";

}
