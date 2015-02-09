#include "GUI_calculator.h"


int main()
{
	save.push_back(Variable("e",2.718281828));
	save.push_back(Variable("Pi",3.141592653));
	save.push_back(Variable("Ans",0));
	CalculatorGUI cal(win_begin,win_x,win_y,"Calculator");
	return gui_main();
}