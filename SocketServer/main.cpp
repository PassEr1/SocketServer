#include <iostream>
#include "SocketServer.h"

int main()
{
    CSocketServer myServer("4531");
    myServer.runForever();
    std::cout << "Done!\n";
}