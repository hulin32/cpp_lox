
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <exception>
#include <vector>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;
using std::map;
using std::vector;

class Foo {
 public:
    int foo;
    explicit Foo(int foo_);
    string foo_str = "foo";
    string hey();
};
Foo::Foo(int foo_):foo(foo_) { }
string Foo::hey() {
    return foo_str;
}

int main(int argc, char const *argv[]) {
    vector<map<string, int>> scopes;
    map<string, int> m;
    scopes.push_back(m);
    auto res = scopes[scopes.size() - 1];
    res["hello"] = 10;
    scopes.emplace_back(res);
    auto res2 = scopes[scopes.size() - 1];
    cout << res["hello"] << endl;
    cout << res2["hello"] << endl;
    return 0;
}
