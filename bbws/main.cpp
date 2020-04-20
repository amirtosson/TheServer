#include "WebServer.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;
void main()
{
	WebServer webServer("127.0.0.1", 54000);
	cerr << "Welcome to SoFa sever \nThe server is running on port # " << 54000 << endl;
	while (true)
	{
		if (webServer.init() != 0)
			return;
		webServer.run();

	}

	system("pause");
}