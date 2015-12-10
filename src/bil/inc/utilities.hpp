#ifndef _INCL_UTILITIES_HPP
#define _INCL_UTILITIES_HPP

//----------userinput0----------
/* This struct is used for holding user input.*/
struct UserInput {
	UserInput(char fwd = 0, char rev = 0, char turn = 0, char stop = 0){
		this->forward = fwd;
		this->reverse = rev;
		this->turn = turn;
		this->stop = stop;
	}
    char forward;
    char reverse;
    char turn;
    char stop;
};
//----------userinput1----------

#endif