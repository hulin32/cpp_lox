#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

struct Object {
    typedef enum { Object_str, Object_num } Object_type;
    string str;
    double num;
    Object_type type;
};

void test() {
    Object obj;
    obj.type = Object::Object_str;
    obj.str = "hello";
    cout << obj.str << endl;
}

int main(int argc, char const *argv[])
{
    test();
    return 0;
}