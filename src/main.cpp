#include <iostream>
#include "server.h"

using namespace std;

int main()
{
    try{
        Server server("0.0.0.0",8080);
        server.run();
    }
    catch(exception& e)
    {
        cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}