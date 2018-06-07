#include "AgendaUI.hpp"
#include <iostream>

AgendaUI aui;
void Stop(int signo) 
{
	aui.test.quitAgenda();
	std::cout << std::endl;
	exit(0);
}
int main () {
	
	aui.OperationLoop();
	return 0;
}

