
#include "GUI_calculator.h"

//-------------------------------------------------------------
const char number = '8';				// a floating-point number
const char quit = 'q';					// an exit command
const char print = ';';					// a print command
const string prompt = "> ";
const string result = "= ";				// indicate that a result follows
const char name = 'a';					// name token
const char let = 'L';					// declaration token
const string declkey = "let";			// declaration keyword
const char factorial='!';				//declaration factorial
const char TheDeltaOfCharToInt='0';		//because the ASC of zero is '0'
const char IndexCalculate='^';			//used for index calculating
//---------------------------------------
//have already be declared in the head file
Point win_begin (740,150);
const int win_x = 300;
const int win_y= 400;
const int interval=5;
const int button_x=(win_x-8*interval)/5;			//5 buttons in a row
const int button_y=(win_y-10*interval)*2/17;		//equal to 8.5 buttons in a column
const int scr_x=win_x-4*interval;					//the x of the out_box 
const int scr_y=button_y;							//the y of the out_box
Point The_initial_point(2*interval,button_y/2);
const int button_in_row = 7;
const int button_in_column = 6;
//to make the array of the button points, in order to make the button easily
Point array_of_button_points(int x,int y)
{
	Point array_of_button_points[button_in_row][button_in_column];
	for (int i=1;i<button_in_row;i++)		//the number of the rows
		for (int j=1;j<button_in_column;j++)	//the number of the columns
			array_of_button_points[i][j] = Point (The_initial_point.x+(j-1)*(button_x+interval),The_initial_point.y+2*scr_y+4*interval+(i-1)*(button_y+interval));
	return array_of_button_points[x][y];
}
//to new the istringstream to put the string s in the expression
Token_stream::Token_stream(string s) 
	:full(false),buffer(0) 
{
	os = new istringstream (s);
};

// have already be declared in pro1
void Token_stream::putback (Token t)
{
	if (full) error("putbake() into a full buffer");
	buffer = t;
	full = true;
}

Token Token_stream::get() 
{
	if (full){
		full=false;
		return buffer;
	}

	char ch;
	(*os)>>ch;

	switch (ch) {
	case ';':   //printf
	case 'q':	//quit
	case '(':
	case ')': 
	case '+': 
	case '-':
	case '*': 
	case '/': 
	case '%':
	case '=':
	case factorial:
	case IndexCalculate:
		return Token ( ch );  // the token stand for itself
	case '.':				// a floating-point literal can start with a dot
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':    // numeric literal
		{
			(*os).putback(ch);			//first put it back to the ostream
			double val1=0;				//to make the val to store the part before the pointer
			while(true){
				char t3=(*os).get();	
				if(isdigit(t3)){							//if is digital, run the block
					double t8=t3- TheDeltaOfCharToInt;		//change the char to int
					val1=10*val1+t8;						//to connect the int
					continue;
				} 
				else {
					(*os).putback(t3);
					break;
				}
			}
			//the part is similar to the above one
			double  val4=0;
			char t4=(*os).get();
			int times = 1;		//we should now the number location after the point
			if(t4=='.'){
				while (true){
					char t5=(*os).get();
					if(isdigit(t5)){
						double t7=t5- TheDeltaOfCharToInt;
						val4=val4+t7/pow(10.0,times);
						times++;
						continue;
					}
					else {
						(*os).putback(t5);
						break;
					}
				}
			} else (*os).putback(t4);
			double  val=val1+val4;		//make the combination of the part before the point and after the point
			char t2=(*os).get();
			if (t2=='.')  error("too many points in one number");									//to solve the problem like 1.1.1
			if (isspace(t2)) error("sorry, don't put the space or enter between the expression, \ncheck if you have entered ';'");	//we don't allow the space between the expression
			(*os).unget();
			return Token(number,val);  
		}
	default:
		if (isalpha(ch))	 //it must be the order to make the val
		{
			string s;	
			s += ch;		//to make the string
			while ((*os).get(ch) && (isalpha(ch) || isdigit(ch))) s+=ch;	
			(*os).putback(ch);	
			if (s == declkey) return Token(let); 
			return Token(name,s);	
		}
		error ("Bad token");	
	}
}
//pro1
void Token_stream::ignore(char c)
	// skip characters until we find a c; also discard that c
{
	// first look in buffer:
	if (full && c==buffer.kind) {  // && means and
		full = false;
		return;
	}
	full = false;  // discard the contents of buffer
	// now search input:
	char ch = 0;
	while ((*os)>>ch)		//we have to use the istringstream to put the expression to the GUI
		if (ch==c) return;
}

//the factorial calculate
double calculate_factorial(double x,int y){
	int zero_factorial=1;
	if (int(x)!=x||x<0)
		error ("Sorry not a positive integer, Please check it");
	else if (x>100) error("Oops,it's a too big number,try again");
	else if (x==0)  return zero_factorial;
	else {
		for (int i=x-y;i>0;i-=y)
			x*=i;
	}
	return x;
}
//the constructor
Calculator::Calculator(Token_stream& tss)
	:ts(tss) {}
//can see the detail declaration in the pro1
double Calculator::primary()
{ 
	Token t=ts.get();	
	double result=0;
	switch(t.kind){			
	case name:
		result = get_value (t.name);
		break;
	case'(':				
		result = expression();
		t=ts.get();
		if(t.kind!=')')	error("')' expected");
		break;
	case number:		
		result = t.value;
		break;
	case'-':			
		result = 1-primary()-1;
		break;
	case'+':
		result = primary();		
		break;
	default:				
		ts.putback(t);
		error("primary expected");
	}
	Token t3 = ts.get();
	if ( t3.kind == factorial)
	{
		int factorial_time=1;
		Token t2 = ts.get();
		while(t2.kind == factorial){
			factorial_time++;
			t2 = ts.get();
		}
		ts.putback(t2);
		return calculate_factorial (result,factorial_time);
	}
	else {
		ts.putback(t3);
		return result;
	}
}

double Calculator::middle()
{
	double left=primary();
	while (true){
		Token t=ts.get();
		switch (t.kind){
		case IndexCalculate:
			{
				double val2=primary();
				//to prevent the user to put a too large number in the expression 
				if (left<-1000||left>1000||val2>=1000) error("it's a too large number ");
				left= pow(left,val2);
				return left;
			}

		default:
			ts.putback(t);
			break;
		}
		break;
	}
	return left;
}



double Calculator::term()  // deal * and /
{
	double left = middle();  
	Token t = ts.get();  // to get the next token in the stream
	while (true) {		
		switch (t.kind) {
		case '*':
			left*=middle();
			t=ts.get();
			break;
		case '/':
			{
				double d=middle();
				if(d==0) error("divide by zero");
				left/=d;
				t=ts.get();
				break;
			}
		case '%':
			{
				double d = middle();
				int i1 = int(left);
				if (i1 != left) error ("left-hand operand of % not int");
				int i2 = int(d);
				if (i2 != d) error ("right-hand operand of % not int");
				if (i2 == 0) error("%: divide by zero");
				left = i1%i2;
				t = ts.get();
				break;
			}
		case name:
			return left*=get_value(t.name);
		default:
			ts.putback(t);  // to put the unused token back to the stream
			return left;
		}
	}
}

double Calculator::expression()
{
	// deal+ and-
	double left = term(); 	
	while (true) {
		Token t = ts.get();  // to get the next token in the stream
		switch (t.kind) {
		case '+':    left += term(); break;
		case '-':    left -= term(); break;
		default:    ts.putback(t);  
			return left;
		}
	}
}
// handle: name = expression
// declare a variable called "name" with the initial value "expression"
double Calculator::declaration()
{
	Token t = ts.get();
	string var_name = t.name;
	if (t.name=="Pi"||t.name=="e"||t.name=="Ans") error ("name expected in declaration");
	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of ", var_name);
	double d = expression();
	define_name(var_name,d);
	preserved = this->var_table;
	return d;
}

double Calculator::statement()
{
	Token t = ts.get();
	switch (t.kind) {
	case let:
		return declaration();
	default:
		ts.putback(t);
		return expression();
	}
}

void Calculator::clean_up_mess()
{
	ts.ignore(print);	
}

double Calculator::calculate() 
{
	double result = statement();
	return result;
}

double Calculator::get_value(string s)		// return the value of the Variable names s
{
	for (int i = 0; i<var_table.size(); ++i)	
		if (var_table[i].name == s) return var_table[i].value;
	error("undefined variation ",s);
}

double Calculator::define_name(string var, double val)	
{
	for (int i = 0; i<var_table.size(); ++i)	
		if (var_table[i].name == var) {var_table[i].value==val;return val;}
		var_table.push_back(Variable(var,val));	
		return val;
}
//to make the GUI calculator constructor
CalculatorGUI::CalculatorGUI(Point begin,int width,int height,const string& title)
	:Window(begin,width,height,title),
	input_win(The_initial_point,scr_x,scr_y,""),
	output_win	(Point(The_initial_point.x,The_initial_point.y+button_y+interval),scr_x,scr_y,""),
	e			(array_of_button_points(1,1),button_x,button_y,"e",cb_number_e),
	pi			(array_of_button_points(1,2),button_x,button_y,"Pi",cb_number_pi),
	left_bracket(array_of_button_points(1,3),button_x,button_y,"(",cb_number_left_bracket),
	right_bracket(array_of_button_points(1,4),button_x,button_y,")",cb_number_right_bracket),
	mode		(array_of_button_points(1,5),button_x,button_y,"Mode",cb_mode),
	off			(Point(win_x-button_x,1)	,button_x,button_y/2,"OFF",cb_exit),
	X			(array_of_button_points(2,1),button_x,button_y,"X",cb_number_X),
	Y			(array_of_button_points(2,2),button_x,button_y,"Y",cb_number_Y),
	dim			(array_of_button_points(2,3),button_x,button_y,"<--",cb_operator_dim),
	backspace	(array_of_button_points(2,4),button_x,button_y,"Del",cb_bs),
	all_clear	(array_of_button_points(2,5),button_x,button_y,"AC",cb_all_clear),
	one			(array_of_button_points(5,1),button_x,button_y,"1",cb_number_1),
	two			(array_of_button_points(5,2),button_x,button_y,"2",cb_number_2),
	three		(array_of_button_points(5,3),button_x,button_y,"3",cb_number_3),
	four		(array_of_button_points(4,1),button_x,button_y,"4",cb_number_4),
	five		(array_of_button_points(4,2),button_x,button_y,"5",cb_number_5),
	six			(array_of_button_points(4,3),button_x,button_y,"6",cb_number_6),
	seven		(array_of_button_points(3,1),button_x,button_y,"7",cb_number_7),
	eight		(array_of_button_points(3,2),button_x,button_y,"8",cb_number_8),
	nine		(array_of_button_points(3,3),button_x,button_y,"9",cb_number_9),
	zero		(array_of_button_points(6,1),button_x,button_y,"0",cb_number_0),
	dot			(array_of_button_points(6,2),button_x,button_y,".",cb_number_dot),
	divide		(array_of_button_points(4,5),button_x,button_y,"/",cb_operator_div),
	remainder	(array_of_button_points(3,4),button_x,button_y,"%",cb_operator_rem),
	factorial	(array_of_button_points(3,5),button_x,button_y,"!",cb_operator_fac),
	multi		(array_of_button_points(4,4),button_x,button_y,"*",cb_operator_mlt),
	ans			(array_of_button_points(6,3),button_x,button_y,"Ans",cb_ans),
	index		(array_of_button_points(6,4),button_x,button_y,"^",cb_operator_ind),
	minus		(array_of_button_points(5,5),button_x,button_y,"-",cb_operator_min),
	equal		(array_of_button_points(6,5),button_x,button_y,"=",cb_equal),
	plus		(array_of_button_points(5,4),button_x,button_y,"+",cb_operator_plu)
{
	//all kinds attach of the button
	attach(zero);
	attach(one);
	attach(two);
	attach(three);
	attach(four);
	attach(five);
	attach(six);
	attach(seven);
	attach(eight);
	attach(nine);
	attach(dot);
	attach(left_bracket);
	attach(right_bracket);
	attach(X);
	attach(Y);
	attach(e);
	attach(pi);


	attach(plus);
	attach(minus);
	attach(multi);
	attach(divide);
	attach(index);
	attach(remainder);
	attach(factorial);

	attach(equal);
	attach(all_clear);
	attach(backspace);
	attach(off);
	attach(ans);
	attach(mode);
	attach(dim);
	attach(input_win);
	attach(output_win);

}
//------------------------------------------------------------
//the callback functions
void CalculatorGUI::cb_number_0(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('0');
}
void CalculatorGUI::cb_number_1(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('1');
}
void CalculatorGUI::cb_number_2(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('2');
}

void CalculatorGUI::cb_number_3(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('3');
}

void CalculatorGUI::cb_number_4(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('4');
}

void CalculatorGUI::cb_number_5(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('5');
}

void CalculatorGUI::cb_number_6(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('6');
}

void CalculatorGUI::cb_number_7(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('7');
}

void CalculatorGUI::cb_number_8(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('8');
}

void CalculatorGUI::cb_number_9(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('9');
}
void CalculatorGUI::cb_number_dot(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('.');
}

void CalculatorGUI::cb_number_left_bracket(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('(');
}

void CalculatorGUI::cb_number_right_bracket(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed(')');
}
void CalculatorGUI::cb_number_X(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('X');
}

void CalculatorGUI::cb_number_Y(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('Y');
}

void CalculatorGUI::cb_number_e(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('e');
}

void CalculatorGUI::cb_number_pi(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('P');
	reference_to<CalculatorGUI>(pw).number_pressed('i');
}

void CalculatorGUI::cb_operator_fac(Address pb,Address pw)
{
	if (mod){
		reference_to<CalculatorGUI>(pw).number_pressed('!');
	}
	else {
		reference_to<CalculatorGUI>(pw).compute();
		reference_to<CalculatorGUI>(pw).number_pressed('!');
	}
}

void CalculatorGUI::cb_ans(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('A');
	reference_to<CalculatorGUI>(pw).number_pressed('n');
	reference_to<CalculatorGUI>(pw).number_pressed('s');
}

void CalculatorGUI::cb_operator_dim(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).number_pressed('<');
}

void CalculatorGUI::cb_operator_plu(Address pb,Address pw)
{ 
	if(mod)
	{reference_to<CalculatorGUI>(pw).number_pressed('+');}
	else
	{
		reference_to<CalculatorGUI>(pw).compute();
		reference_to<CalculatorGUI>(pw).number_pressed('+');
	}
}
//the functions below had to change when the mode is changed
void CalculatorGUI::cb_operator_min(Address pb,Address pw)
{
	if(mod)
	{reference_to<CalculatorGUI>(pw).number_pressed('-');}
	else
	{
		reference_to<CalculatorGUI>(pw).compute();
		reference_to<CalculatorGUI>(pw).number_pressed('-');
	}
}
void CalculatorGUI::cb_operator_mlt(Address pb,Address pw)
{
	if(mod)
	{reference_to<CalculatorGUI>(pw).number_pressed('*');}
	else
	{
		reference_to<CalculatorGUI>(pw).compute();
		reference_to<CalculatorGUI>(pw).number_pressed('*');
	}

}
void CalculatorGUI::cb_operator_div(Address pb,Address pw)
{
	if(mod)
	{reference_to<CalculatorGUI>(pw).number_pressed('/');}
	else
	{
		reference_to<CalculatorGUI>(pw).compute();
		reference_to<CalculatorGUI>(pw).number_pressed('/');
	}

}
void CalculatorGUI::cb_operator_ind(Address pb,Address pw)
{
	if(mod)
	{reference_to<CalculatorGUI>(pw).number_pressed('^');}
	else
	{
		reference_to<CalculatorGUI>(pw).compute();
		reference_to<CalculatorGUI>(pw).number_pressed('^');
	}

}
void CalculatorGUI::cb_operator_rem(Address pb,Address pw)
{
	if(mod)
	{reference_to<CalculatorGUI>(pw).number_pressed('%');}
	else
	{
		reference_to<CalculatorGUI>(pw).compute();
		reference_to<CalculatorGUI>(pw).number_pressed('%');
	}

}
//------------------------------------------------------------------------------
void CalculatorGUI::cb_equal(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).compute();
}
void CalculatorGUI::cb_all_clear(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).all_clear_pressed();
}
void CalculatorGUI::cb_bs(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).bs_pressed();
}
void CalculatorGUI::cb_exit(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).exit();
}

void CalculatorGUI::cb_mode(Address pb,Address pw)
{
	reference_to<CalculatorGUI>(pw).change_mode();
}
//------------------------------------------------------------------------------
//the detail implement of the callback functions
void CalculatorGUI::number_pressed(char c)
{
	exp += c;						//to build the expression of the char you enter in
	input_win.put(exp);				//put the expression in the in_box
	redraw();						
}
void CalculatorGUI::all_clear_pressed()
{
	exp = "";								//clear the expression
	input_win.put(exp);						//put the cleared expression in the in_box (clear the in_box)
	output_win.put(exp);					//clear the output_win 
	redraw();
}
void CalculatorGUI::bs_pressed()
{
	exp=exp.substr(0,exp.length()-1);		//when the backspace is pressed, erase the end char
	input_win.put(exp);						//put the expression in In_box to substitute the old one
	redraw(); 
}

void CalculatorGUI::exit()
{
	hide();									//hide the window
}
void CalculatorGUI::memory()
{
	exp=current_result;						//to record the current result of the calculating
}
void CalculatorGUI::change_mode()
{
	if(mod==true)							//judge the mode
	{
		mod=false;							//change the mode
		//in the mode with instant calculate, the brackets,the answer,the val_define is not provided 
		//so we need to hide some buttons in the window
		left_bracket.hide();				
		right_bracket.hide();
		X.hide();
		Y.hide();
		dim.hide();
		ans.hide();
		redraw();
		return;
	}
	if(mod==false)
	{
		//in the mode of the normal calculating, we need to attach the buttons
		//to support the brackets and the val_define and the answer_memory 
		mod=true;
		attach(left_bracket);
		attach(right_bracket);
		attach(X);
		attach(Y);
		attach(dim);
		attach(ans);
		redraw();
		return;
	}
}

//------------------------------------------------------------------------------

void CalculatorGUI::compute()
{
	try{
		bool decl=false;					//if the val is defined
		string name;						//the name of the val
		string final = exp;					//the result of the calculating
		//the main point is that to use the main to rebuild the expression in a way accepted in the project1
		for (int i=1;i<final.size();i++)		
			if (final[i]=='<')				//judge if we need to define the val
			{
				decl=true;
				string use="let ";			
				for (int j=0;j<i;j++)
				{
					use += final[j];
					name += final[j];
				}
				use+="=";
				for (int j=i+1;j<final.size();j++)
				{
					if (final[j]=='<')
					{
						use+=final[j];
						break;
					}
					use+=final[j];
				}
				final=use;
				break;
			}
			//to calculate the result
			final+=print;								//to build the right expression
			Token_stream ts(final);						//put the final in the Token_stream
			cal=new Calculator(ts);						//new a calculator
			cal->var_table=preserved;					//set the val like pi,e in the vector of the val s
			double come=cal->calculate();				//to get the result of the expression

			//to put the result out in the out_box
			ostringstream cr;							//used the ostringstream to put the result out 
			cr.precision(8);							//set the precision of the ostream
			cr<<come;									//print the result
			current_result=cr.str();					

			stringstream out;
			out<<"="<<come;
			string answer=out.str();					//set the answer after calculating
			if(decl==true)								//if it's the val definition
				output_win.put("now "+name+answer);
			else 
				output_win.put(answer);
			redraw();
			for(int i=0;i<preserved.size();i++)		//every time to change the ans in the vector
				if(preserved[i].name=="Ans")preserved[i].value=come;	//'cause we use ans as a val_defined too
			decl=false;					//reset the expression in the next calculating
			if(mod)						//the mode1
			{
				exp="";					//don't need to store the current result
			}
			else						//the mode2
			{
				exp=current_result;		//the beginning of the next expression is the current result
			}
	}
	catch (exception& e) {
		input_win.put(e.what()); 
		cal->clean_up_mess();		//to deal with the problem   
	}
}

//------------------------------------------------------------------------
//to set the val s in the vector and it's easy for us to put the new vals (like:NA(6.02*10^23)) in
void set_the_vals(vector<Variable>& p)
{
	p.push_back(Variable("e",2.718281828));
	p.push_back(Variable("Pi",3.141592653));
	p.push_back(Variable("Ans",0));
}

//to give the user the instruction
void instrcution_for_user () 
{
	cout<<"Now you can use the GUI calculator designed by zdl"<<endl
		<<"	It supports many functions,but still has many weaknesses"<<endl;
	cout<<"Here are some Instructions for you:"<<endl
		<<"		1.We allow + - * / ! ^and mitipu-! calculating method ,"<<endl
		<<"		2.!! means like this <4!!=4*2>,!!!means like this <6!!!=6*3>"<<endl
		<<"		3.You can define the val by yourself and use it in the following"
		<<"		calculating,but it doesn't support the repeated val definition"<<endl
		<<"		4.You can even calculate the value of 2.1^3.2"<<endl
		<<"		5.It can not support the scentific input,so 2e means 2*e"<<endl
		<<"		6.Don't put the space after the number,we considered it a error"<<endl
		<<"		7.Attention :do not enter Chinese characters in the program,"<<endl
		<<"		it has no idea about that"<<endl
		<<"		8.To make it more general,we have already set pi(3.1415926) and"<<endl
		<<"		e(2.71828) for you"<<endl
		<<"		9.You need to enter a ';' behind the expression to print the "<<endl
		<<"		result"<<endl;
	cout<<"Anyway,it's a not too bad calculator,just click the button on the window~~~ "<<endl<<endl;
}

//the main function.....
//but why when I put it to a new main_cpp, it will have many problems like (multi_define in the obj?)....so wield
//how to deal with this kind of the problem
int main()
{
	try{
		instrcution_for_user();
		set_the_vals(preserved);
		CalculatorGUI cal(win_begin,win_x,win_y,"ZDL_Calculator");
		return gui_main();
	} 
	catch (exception& e){
		cerr << e.what() <<endl;
	}
	catch (...)
	{
		cerr << "something went worry"<< endl;
	}
}
