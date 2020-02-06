#include <string>
#include <memory>
#include <iostream>

using std::string;
using std::stod;
using std::shared_ptr;
using std::cout;
using std::endl;

class Foo {
 public:
    int foo;
    int hello () { return 100; }
};

int main(int argc, char const *argv[])
{
    Foo foo;
    cout << foo.hello() << endl;
    return 0;
}