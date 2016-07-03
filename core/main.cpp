#include "opvalue.h"
#include <iostream>

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    cout << "opvalue: " << sizeof(opvalue) << endl;
    cout << "op_slot3: " << sizeof(op_slot3) << endl;

    opvalue op;
    op.slot3.imm = 5;

    cout << "op reg3 test: " << (int)op.slot3.reg << endl;

    return 0;
}
