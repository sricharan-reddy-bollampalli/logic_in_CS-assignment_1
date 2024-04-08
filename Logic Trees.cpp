//LOGIC ASSIGNMENT-1 19/10/22
//NIKHIL DHANARAJ,PRITAM REDDY,SRICHARAN REDDY

#include <iostream>
#define SIZE 100

using namespace std;

string Reverse(string input,int n);
string PrefixFromInfix(string input);
bool CheckParantheses(char c);
char SwitchParantheses(char c);
bool CheckOperator(char c);
bool CheckOperand(char c);
bool CheckRightAssociative(char c);
bool CheckNegation(char c);
bool CheckHigherPreferance(char op1,char op2);
int OperatorWorth(char c);

class Stack{												///creating our own stack function
private:
	int A[SIZE];
	int Top;
public:
	Stack(){
		Top = -1;
	}
	
	void push(int x){										///adds a member to top of stack
		if(Top == SIZE -1){
			cout<<"ERROR: Stack Overflow\n";
			return;
		}
		A[++Top] = x;
	}
	
	void pop(){												///removes the topmost member of the stack					
		if(Top == -1){
			return;
		}
		Top--;
	}
	
	char top(){												///returns the topmost member of the stack
		return A[Top];
	}
	
	bool empty(){											///we check if the stack is empty
		if(Top == -1)
			return true;
		return false;
	}
	
};

struct tree													///the information of the parse tree is contained in this structure
{
	char data;
	tree *left;
	tree *right;
};

tree *root;													///the root of the tree is initialised here

char operandarray[26];										///the operands in the expression are stored in this array	
bool trutharray[26];										///the truth values of the operands in the expression are stored here		
int arrlength = 0;											///the number of operands in the expression is stored in this variable

string PrefixFromInfix(string input){						///this function converts an infix expression to prefix
	string result = "";										
	
	int n {input.length()};									///n is the length of the expression
	
	Stack S;
	
	input = Reverse(input,n);								///we reverse the input expression to first compute the postfix expression

	for(int i = 0;i<n;i++){
		if(input[i] == ' '){								///if the current character of the input is a space we continue
			continue;
		}
		else if(CheckOperator(input[i])){														///we check if the function is operator
			while(!S.empty() && S.top() != '(' && CheckHigherPreferance(S.top(),input[i])){
				result += S.top();
				S.pop();
			}
			S.push(input[i]);
		}
		else if(CheckOperand(input[i])){				///we check if the function is an operand
			result += input[i];
		}
		else if(input[i] =='('){						///we check if the input is an open parantheses	
			S.push(input[i]);
		}
		else if(input[i]==')'){							///we check if the input is an close parantheses
			while(!S.empty() && S.top() != '('){
				result += S.top();
				S.pop();
			}
			S.pop();
		}
	}
	
	while(!S.empty()){									///we check if the stack is empty									
		result += S.top();
		S.pop();
	}
	
	result = Reverse(result,result.length());			///we now reverse the expression again from the postfix expression to get the prefix statement
		
	return result;				
}

string Reverse(string input,int n){						///this function is used to reverse the string input by pushing all members into one stack then adding them to the input expression again by first in last out logic which reverses the expression
	Stack S;
	for(int i{0};i<n;i++){
		S.push(input[i]);
	}
	
	for(int i{0};i<n;i++){
		if(!(CheckParantheses(S.top()))){
			input[i] = S.top();
			S.pop();
		}
		else{
			input[i] = SwitchParantheses(S.top());
			S.pop();
		}
	}
	return input;
}

char SwitchParantheses(char c){						///we implement a function to switch parantheses while reversing the expression	
	if(c=='{')
		return '}';
	else if(c=='(')
		return ')';
	else if(c==')')
		return '(';
	else if(c=='}')
		return '{';
	else if(c=='[')
		return ']';
	else if(c==']')
		return '[';
}

int OperatorWorth(char c){							///we rank all operators based on their order of precedence (for higher precedence higher rank is given)
	int worth {0};
	
	switch(c){
		case '>':
			worth = 1;
			break;
		case '+':
			worth = 2;
		case '*':
			worth = 3;
		case '~':
			worth = 4;
	}	
	return worth;
}

bool CheckRightAssociative(char c){				///we check the right associativity of operators
	if(c=='~'||c=='>')
		return true;
	return false;
}

bool CheckHigherPreferance(char op1,char op2){		///we check which operator has higher precedence in the expression when there are two consecutive operators in stack
	int op1pref = OperatorWorth(op1);
	int op2pref = OperatorWorth(op2);
	
	if(op1pref == op2pref){
		if(CheckRightAssociative(op1))
			return true;
		return false;
	}
	return op1pref>op2pref?true:false;
}

bool CheckOperator(char c){										///we check if current character is an operator
	if (c == '~' || c == '+' || c == '*' || c == '>')
		return true;

	return false;
}

bool CheckOperand(char c){										///we check if current character is an operand
	if (c >= 'a' && c <= 'z')
		return true;
	if (c >= 'A' && c <= 'Z')
		return true;
	return false;
}

bool CheckParantheses(char c){																	///we check if current character is a parantheses
	if (c == ']' || c == '[' || c == '}' || c == '{' || c == ')' || c == '(')
		return true;
	else
		return false;
}

bool CheckNegation(char c){									///we check if current character is anegation symbol
	if (c == '~')
		return true;
	return false;
}

int i = 1;

char *addToTree(tree **roothead, char *input){					///this function creates the parse tree			
	
	if (*input == '\0')											///this is the base condition for recursion to end	
		return "\0";

	while (1)
	{
		char *q = "null";
		if (*roothead == NULL)									///if *roothead is null this if condition is executed
		{
			tree *temp = new tree();
			temp->data = *input;
			temp->left = NULL;
			temp->right = NULL;
			*roothead = temp;
		}
		else
		{
			
			if (CheckOperand(*input))							///this checks if the character is an operand
			{
				return input;
			}
			
			if (!CheckNegation((*roothead)->data))
			{
				q = addToTree(&(*roothead)->left, input + 1);	///this builds the left sub-tree
				
				q = addToTree(&(*roothead)->right, q + 1);		///this builds the right sub-tree
			}
			else
			{
				q = addToTree(&(*roothead)->right, input + 1);	///this builds the right sub-tree
			}

			return q;
		}
	}
}

void traverse(tree *p){ 							///this function traverses the parse tree and prints the infix expression of the parse tree

	if (p == NULL)										///if p points to null then we return, this is base condition for recursion to end
	{
		return;
	}
	else
	{
		traverse(p->left);								///traversing through the left sub tree
		cout<<p->data;									///printing current atom
		traverse(p->right);								///traversing through the right subtree
	}
}

int computeHeight(tree *p){								///this function returns the height of the tree
	int leftheight, rightheight;
	if (p == NULL)										///this is the base condition for recursion to end	
	{	
		return 0;										
	}

	leftheight = computeHeight(p->left);				///computes height of left subtree
	rightheight = computeHeight(p->right);				///computes height of right subtree
	if (leftheight >= rightheight)
	{
		return leftheight + 1;							///returns height of left subtree
	}
	else
	{
		return rightheight + 1;							///returns height of right subtree		
	}
}

bool truthvalue(bool opd1, char opt, bool opd2){		///this function computes the truth value of the operand and its right and left atoms

	bool result;

	if (opt == '*')										///this function computes the logarithmic AND of the two boolean values
	{
		result = opd1 && opd2;
	}
	else if (opt == '>')
	{
		result = (!opd1) || opd2;						///this function computes the logarithmic IMPLICATION of the two boolean values
	}
	else if (opt == '+')
	{
		result = opd1 || opd2;							///this function computes the logarithmic OR of the two boolean values			
	}

	if (result == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool checkvalue(char c){								///this function checks the inputted truth value of the operand passed into the functiom
	int j = 0;
	while (c != operandarray[j])
	{
		j++;
	}
	return trutharray[j];
}

bool CheckInArray(char c){									///this function checks if the characters passed are already passed in the array
	for(int l = 0;l<arrlength;l++){
		if(c == operandarray[l]){
			return true;
		}
	}
	return false;
}

void settruth(char *a){									///this function inputs the truth values to be designated for the operands
	int j = 0;
	int k = 0;
	while (a[j] != '\0')
	{
		if (CheckOperand(a[j]) && !CheckInArray(a[j]))							///adds operand into operandarray	
		{
			operandarray[k] = a[j];
			arrlength++;
			k++;
		}
		j++;
	}

	j = 0;
	for (; j < arrlength; j++)											///adds truth value into trutharray in the same positions as the operand, the truth value is inputted for, in the operandarray
	{
		cout << "Enter truth value for " << operandarray[j] << ": ";
		cin >> trutharray[j];
	}
}

bool ComputeTruth(tree *p){												///this function computes the truth value of the parse tree expression

	bool lefttruth;														///the truth value of the left subtree	
	bool righttruth;													///the truth value of the right subtree

	if (!CheckNegation((p->data)))										///check if the current data is not a negation symbol
	{
		if(CheckOperand(p->data)){
			return checkvalue(p->data);									///returns the truth value of current operand
		}
		
		lefttruth = ComputeTruth(p->left);								///recurses through left subtree
		righttruth = ComputeTruth(p->right);							///recurses through right subtree		

		return truthvalue(lefttruth,p->data,righttruth);				///returns truth value of the subtree		

	}
	else{
		return !(checkvalue((p->right)->data));							///returns negation of the truth value of right subtree as negation is current data
	}
}

int main()																///main function
{
	string infix;														
	tree *s = NULL;
	bool truth = false;
	
	cout<<"Enter an Infix Expression: ";
	getline(cin,infix);													///inputs infix expression
	
	string prefix = PrefixFromInfix(infix);								///string prefix is the prefix expression of the infix expression that was previously inputted
	
	char a[prefix.length()];
	for(int l= 0;l<prefix.length();l++){
		a[l]=prefix[l];
	}
	
	settruth(a);														///sets the truth values for input operands	
	cout<<endl<<endl;													
	
	//TASK 1
	cout<<"The Prefix expression is: "<<prefix<<endl; 						///outputs the prefix expression
	cout<<endl<<endl;

	//TASK 2
	addToTree(&s, a);														///calling function to creates the parse tree
	
	//TASK 3
	cout<<"The Infix expression is: ";
	traverse(s);															///calling function to traverses through the parse tree
	cout<<endl<<endl;
	
	//TASK 4
	cout << "\nThe height of the parse tree is " << computeHeight(s);		///calling function to compute height of the parse tree
	cout<<endl<<endl;
	
	//TASK 5
	truth = ComputeTruth(s);												///calling function to compute truth value of expression
	cout << "\nThe truth value of the table is " << truth;					
	cout<<endl<<endl;
	
}