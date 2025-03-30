#include <iostream>
#include "server.h"
#include <memory> 

using namespace std;

int main()
{
    try{
        auto server = std::make_shared<Server>("0.0.0.0", 8080);  // std:: 명시적으로 추가
        server->run();
    }
    catch(exception& e)
    {
        cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}