#include "Application.hpp"

int main() 
{
	Application application(1024, 768);

	application.Initialize();
	application.Render();
	application.CleanUp();

    return 0;
}