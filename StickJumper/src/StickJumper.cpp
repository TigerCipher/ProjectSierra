#include <iostream>

#include "Core/App.h"

using namespace stick;

int main(int argc, char* argv[])
{
    App app;

    app.CreateWindow("Stick Jumper", 1920, 1080);

    app.Run();
    
    return 0;
}
