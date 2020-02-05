#include <string>
#include <memory>
#include <iostream>

using std::string;
using std::stod;
using std::shared_ptr;
using std::cout;
using std::endl;

int main(int argc, char const *argv[])
{
    string t = "1";
    cout << stod(t) << endl;
    return 0;
}