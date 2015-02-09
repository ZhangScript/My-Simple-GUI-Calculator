//The head files
#include "std_lib_facilities.h"
#include "Simple_window.h"
#include "Graph.h"
#include "Window.h"
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
using namespace Graph_lib;
using namespace std;

//similar to the default mode
static bool mod=true;
//------------------------------------------------------------------
//here's all kinds of the token
extern const char number;				// a floating-point number
extern const char quit;					// an exit command
extern const char print;				// a print command
extern const string prompt;
extern const string result;				// indicate that a result follows
extern const char name;					// name token
extern const char let;					// declaration token
extern const string declkey;			// declaration keyword
extern const char factorial;			//declaration factorial
extern const char TheDeltaOfCharToInt;	//because the ASC of zero is '0'
extern const char IndexCalculate;		//used for index calculating

//---------------------------------------------------------------
//here's all kinds of the setting for the window
extern Point win_begin;				//The begin of the window
extern const int win_x;				//window's x
extern const int win_y;				//window's y
extern const int interval;		
extern const int button_x;			//size of the button
extern const int button_y;
extern const int scr_x;				// the screen's x
extern const int scr_y;				//the screen's y
extern const int button_in_row;		//the number of the button in a row
extern const int button_in_column;	//the number of the button in a column
extern Point The_initial_point;

//------------------------------------------------------------------------------

class Token {
public:
	char kind;					// what kind of token
	double value;				// for numbers: a value 
	string name;				// for names: name itself
	Token(char ch)             : kind(ch), value(0)   {}
	Token(char ch, double val) : kind(ch), value(val) {}
	Token(char ch, string n)   : kind(ch), name(n)    {}
};

class Token_stream {
public:
	Token get();					// get a Token
	void putback(Token t);			// put back a Token
	Token_stream(string);			// make a Token_stream that reads from cin
	void ignore(char c);			// discard tokens up to and including a c
	istringstream* os;
private:
	bool full;						// is there a Token in the buffer?
	Token buffer;					// here is where we keep a Token put back using putback()
};

//new class of Variable,already be declared in pro1
class Variable {
public:
	string name;											//the name of the val
	double value;											//the value of the val
	Variable (string n, double v) :name(n), value(v) { }	//the constructor
};

//new class for the calculate
class Calculator{
public:
	vector<Variable> var_table;		//to store the val
	Calculator (Token_stream&);		//the constructor
	double calculate ();			//to calculate the result
	void clean_up_mess ();			//to deal with the error solution	

private:
	Token_stream ts;				//the Token_stream to store the expression
	Token get_token();				//to get the token in the expression

	//here's the rules of the expression in the pro1
	double primary		();				
	double middle		();
	double term			();
	double expression	();
	double declaration	();
	double statement	();
	double get_value	(string s);						//to get the value of the val
	double define_name	(string var, double val);		//to define the name of the val
};
//------------------------------------------------------------------------
//the new class to make the calculator to the GUI kind
class CalculatorGUI:Window
{	
public:
	CalculatorGUI(Point begin,int width,int height,const string& title);

private:
	Calculator* cal;				//to make a pointer whose class is the Calculator

	string exp;						//means the expression
	string current_result;			//to store the current result
		
	Button zero;				//0
	Button one;					//1
	Button two;					//2
	Button three;				//3
	Button four;				//4
	Button five;				//5
	Button six;					//6
	Button seven;				//7
	Button eight;				//8
	Button nine;				//9
	Button dot;					//.
	Button left_bracket;		//'('
	Button right_bracket;		//')'
	Button plus;				//+
	Button minus;				//-
	Button multi;				//*
	Button divide;				// /
	Button remainder;			//%
	Button factorial;			//!
	Button equal;				//=
	Button index;				//^
	Button X;					//to make the val1
	Button Y;					//to make the val2
	Button e;					//to make the 'e'
	Button pi;					//to make the 'pi'
	Button all_clear;			//AC
	Button backspace;			//<-
	Button off;					// close
	Button ans;					// answer
	Button mode;				//change mode
	Button dim;					//to define a val
	Out_box input_win;			//to put the expression you sent out
	Out_box output_win;			//to put the result out
	
	//the callback functions of the value 
	static void cb_number_0(Address,Address);
	static void cb_number_1(Address,Address);
	static void cb_number_2(Address,Address);
	static void cb_number_3(Address,Address);
	static void cb_number_4(Address,Address);
	static void cb_number_5(Address,Address);
	static void cb_number_6(Address,Address);
	static void cb_number_7(Address,Address);
	static void cb_number_8(Address,Address);
	static void cb_number_9(Address,Address);
	static void cb_number_dot			(Address,Address);
	static void cb_number_left_bracket	(Address,Address);
	static void cb_number_right_bracket	(Address,Address);
	static void cb_number_X	(Address,Address);
	static void cb_number_Y	(Address,Address);
	static void cb_number_e	(Address,Address);
	static void cb_number_pi(Address,Address);
	static void cb_ans		(Address,Address);

	//the callback function of the operator function
	static void cb_operator_plu(Address,Address);
	static void cb_operator_min(Address,Address);
	static void cb_operator_mlt(Address,Address);
	static void cb_operator_div(Address,Address);
	static void cb_operator_ind(Address,Address);
	static void cb_operator_rem(Address,Address);
	static void cb_operator_fac(Address,Address);
	static void cb_operator_dim(Address,Address);

	//the callback function of the special function of the calculate
	static void cb_equal	(Address,Address);
	static void cb_all_clear(Address,Address);
	static void cb_bs		(Address,Address);
	static void cb_exit		(Address,Address);
	static void cb_mode		(Address,Address);

	// the detail implement of the callback functions
	void number_pressed  (char c);		//to press the number button and the basic operator button
	void all_clear_pressed ();			//to clear the expression you have already put in			
	void bs_pressed ();					//to make the backspace
	void exit ();						//to exit 
	void memory ();						//store the current result
	void change_mode ();					//change the kind of the mode
	void compute ();						//to calculate the result

};

void instrcution_for_user ();			//the easy function just to instruct the user

vector <Variable> preserved;				//the vector to store the new val
void set_the_vals (vector<Variable>& p);	//to get the basic val in the vector
