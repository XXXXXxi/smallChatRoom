#include <iostream>
#include "Utils/Message.h"

using namespace std;
int main ()
{
    Message message(1,"king","host","hhhhh");
    std::string str = message.serialization();
    Message message1 = message.deserialization(str);
    message1.printMessage();
    return 0;
}