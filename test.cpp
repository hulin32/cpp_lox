
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include <exception>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;

class Bar;

class Foo {
 public:
    Bar* bar;
    explicit Foo(Bar* bar_);
    string foo_str = "foo";
    string hey();
};

class Bar {
 public:
    Foo* foo;
    explicit Bar(Foo* foo_): foo(foo_) { }
    string bar_str = "foo";
    string hey() {
        return foo->foo_str;
    }
};

Foo::Foo(Bar* bar_):bar(bar_) { }
string Foo::hey() {
    return bar->bar_str;
}

int main(int argc, char const *argv[]) {
    return 0;
}
