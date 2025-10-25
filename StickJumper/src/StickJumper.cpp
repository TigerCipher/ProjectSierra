#include <iostream>

#include "Core/App.h"

using namespace stick;

int main(int argc, char* argv[])
{
    try
    {
        App app;

        app.CreateWindow("Stick Jumper", 1920, 1080);

        app.Run();
    } catch (const StickException& ex)
    {
        std::cout << ex.what() << '\n';
        return EXIT_FAILURE;
    } catch (const std::exception& ex)
    {
        std::cout << "Unhandled standard exception: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
