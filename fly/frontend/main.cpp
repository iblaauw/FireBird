#include <StringView.h>
#include <iostream>

int main()
{
    std::cout << "Hello World" << std::endl;
    StringView view = "Hello There"_sv;
    StringView view2 = view.To(5);
    std::cout << view2 << std::endl;
    return 0;
}
