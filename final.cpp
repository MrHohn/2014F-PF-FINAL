/*
The final.cpp file for Programing Finance's final project

Written by Zihong Zheng
*/

#include <iostream>
#include <sstream>
#include <string>
#include "list.h"
#include "cell.h"
using namespace std;

/* 
initiating the global variable for the depth of functional cell's maximum depth(if functional cell A use functional cell B's value, and functionnal cell B use functional cell C's value, the depth is 3)
*/
int function_depth = 1; 

/*this function is used to update the functional cell's value*/

void reupdate(LList<LList<Cell*>>& sheet){
	/*use function_depth to judge how many time the spreadsheet should loop in order to keep all the functional cell's value up to date*/
	for(int i = 0; i < function_depth; i++){
		for(LList<LList<Cell*>>::iterator I = sheet.begin(); I != sheet.end(); ++I){
			for(LList<Cell*>::iterator J = (*I).begin(); J !=(*I).end(); ++J) {
				/*if the cell is a funtional cell, call its update method*/
				if((*J) -> get_cell_type() == 2){
					(*J) -> update();
				}
			}
		}
	}
}

int main(int argc, char** argv){

	/*Judge if the user gives correct number of input*/
	if(argc != 3){
		cout << "Please input the value of width and height!" << endl;
		return 1;
	}

	/* Initialize the width and height of the spreadsheet */
	int column;
	int row;
	try{
	/* Attempt to set width and height from the user provided arguments */
		column = stoi(string(argv[1]));
		row = stoi(string(argv[2]));
		/*Judge if the user gives positive for width and height*/
		if(column <= 0 || row <= 0){
			cout << "Please input positive values for width and height!" << endl;
			return 1;
		}
	}
	/*if the user doesn't input number, prompt*/
	catch(std::exception e){
	/* Handle failure of the stoi function */
		cout << " Got bad values for width and height !\n" ;
		return 1;
	}

	/* Initiating an empty spreasheet */

	/* Define the Linked list to store rows */
	LList<LList<Cell*>> spreadsheet;
	for(int i = 0; i < row; i++){
		LList<Cell*> list_new;
		spreadsheet.push_front(list_new);
		for(int j = 0; j < column; j++){
			(*(spreadsheet.begin())).push_back(new Cell);
		}
	}

/* Read in the entire input line */
	string line;
	while(getline(cin, line)){
		istringstream ss(line);
		string command;
		ss >> command;
		//Process the command
		if(command == "print"){
			/*define position_count for recognizing the end of a row*/
			int position_count = 0;
			/*loop over the spreadsheet and call every cell's toString method to print*/
			for(LList<LList<Cell*>>::iterator I = spreadsheet.begin(); I != spreadsheet.end(); ++I){
				for(LList<Cell*>::iterator J = (*I).begin(); J !=(*I).end(); ++J) {
					position_count++;
					if(position_count == column){
						cout << (*J) -> toString() << endl;
						position_count = 0;
					}
					else{
						cout << (*J) -> toString() << ", ";
					} 
				}
			}
		}
		/*process the set command*/
		else if(command == "set"){
			/* read the position that the user want to put the input */
			ss >> command;
			int position_column = stoi(command);
			ss >> command;
			int position_row = stoi(command);
			ss >> command;
			string cell_type = command;

			/* judge if the position is valid */
			if((0 <= position_column && column > position_column) && (0 <= position_row && row > position_row)){

				/* processing different type of set */

				/* deal with set number command */
				if(cell_type == "number"){
					double value;
					/* judge if the uset input number for set number */
					if(!(ss >> value)){
						cout << "Error: Bad input for set number" << endl;
					}
					else{
						/*calculate the target iterator*/
						LList<LList<Cell*>>::iterator I = spreadsheet.begin();
						for(int i = 0; i < position_row; i++){
							I++;
						}
						LList<Cell*>::iterator J = (*I).begin();
						for(int i = 0; i < position_column; i++){
							J++;
						}
						/* use the set_value funtion of list to set respectively value */
						(*I).set_value(J, new Cell_double(value));
						/* call the update funtion to update the value of functional cell within the spreadsheet at the end */
						reupdate(spreadsheet);
					}							
				}
				/* deal with set string command */
				else if(cell_type == "string"){
					string value = "";
					/* define a bool variable to judge if it is the first part of a string (to prevent inserting a space before the string) */
					bool initial = true;
					while(ss >> command){
						/* combine all the parts if there are spaces within the string */
						if(initial){
							value += command;
							initial = false;
						}
						else{
							value += " " + command;
						}
					}
					/* calculate the target iterator */
					LList<LList<Cell*>>::iterator I = spreadsheet.begin();
					for(int i = 0; i < position_row; i++){
						I++;
					}
					LList<Cell*>::iterator J = (*I).begin();
					for(int i = 0; i < position_column; i++){
						J++;
					}
					(*I).set_value(J, new Cell_string(value));
					/* call the update function */
					reupdate(spreadsheet);

				}
				/* deal with set mean|max|min command */
				else if(cell_type == "mean" || cell_type == "max" || cell_type == "min"){
					/* define function_type to record the type(0 for mean, 1 for max and 2 for min) */
					int function_type;
					if(cell_type == "mean")function_type = 0;
					else if(cell_type == "max")function_type = 1;
					else function_type = 2;

					ss >> command;
					int cal_row = stoi(command);
					ss >> command;
					int cal_begin = stoi(command);
					ss >> command;
					int cal_end = stoi(command);

					/* judge if the user input a valid postion and range */
					if((0 <= cal_begin && column > cal_end && cal_end >= cal_begin) && (0 <= cal_row && row > cal_row)){
						/* calculate the begin iterator of the cells that this functional cell asks for */
						LList<LList<Cell*>>::iterator I = spreadsheet.begin();
						for(int i = 0; i < cal_row; i++){
							I++;
						}
						LList<Cell*>::iterator J = (*I).begin();
						for(int i = 0; i < cal_begin; i++){
							J++;
						}
						LList<Cell*>::iterator column_begin = J;
						LList<Cell*>::iterator column_end;
						/* calculate the end iterator of the cells */
						if(cal_end != cal_begin){
							for(int i = 0; i < cal_end - cal_begin + 1; i++){
								J++;
							}
							column_end = J;
						}
						else{
							column_end = column_begin;
						}
						
						/* calculate the target iterator */
						I = spreadsheet.begin();
						for(int i = 0; i < position_row; i++){
							I++;
						}
						J = (*I).begin();
						for(int i = 0; i < position_column; i++){
							J++;
						}
						/* use the set_value function to set the value of the functional cell */
						(*I).set_value(J, new Cell_functional(column_begin, column_end, function_type, cal_row));
						/* call the update function */
						reupdate(spreadsheet);

					}
					/* if the user input an invalid postion or range for functional cell to calculate, prompt */
					else{
						cout << "Error: funtion incorrect" << endl;
					}
					/* if what user input does't match any of the commands, prompt */
				}
				else{
					cout << "Error: unknown command" << endl;
				}
			}
			/* if user input an invalid position for functional cell, prompt */
			else{
				cout << "Error: cell out of range" << endl;
			}
		}
		/* deal with the addrow command */
		else if(command == "addrow"){
			ss >> command;
			/* use try in case the user does not input number for row*/
			try{
				int position_row = stoi(command);
				/* judge if the row is valid */
				if(position_row < row && position_row >= 0){
					/* increase the row counter */
					row++;
					LList<LList<Cell*>>::iterator I = spreadsheet.begin();
					/* if the user what to insert before the first row, use the first method */
					if(position_row == 0){
						LList<Cell*> list_new;
						spreadsheet.push_front(list_new);
						for(int j = 0; j < column; j++){
							(*(spreadsheet.begin())).push_back(new Cell);
						}	
					}
					else{
						/* calculate the target row's iterator */
						for(int i = 0; i < position_row; i++){ 
							I++;
						}
						/* insert a new row before this position */
						I = spreadsheet.insert(I, *(new LList<Cell*>));
						for(int j = 0; j < column; j++){
							(*I).push_back(new Cell);
						}	
					}
					/* loop over the spreadsheet to modified the row member stored in the functional cells --- the row member inside them is used to store the row which they depend on, cause we are adding a row now, some of them should be changed correspondingly */
					for(LList<LList<Cell*>>::iterator I = spreadsheet.begin(); I != spreadsheet.end(); ++I){
						for(LList<Cell*>::iterator J = (*I).begin(); J !=(*I).end(); ++J) {
							/* judge if the cell is a functional cell */
							if((*J) -> get_cell_type() == 2){
								/* if the row it store is larger than what we insert, increase its row */
								if((*J) -> get_row() >= position_row){
									(*J) -> row_increase();
								}
							}
						}
					}		
				}
				/* prompt if the row is invalid */
				else{
					cout << "Error: row out of range" << endl;
				}
			}
			/* if the user does not input number, prompt */
			catch(exception e){
				cout << "Error: row out of range" << endl;
			}
		}
		/* deal with the removerow command */
		else if(command == "removerow"){
			ss >> command;
			/* use try in case the user does not input number for row*/
			try{
				int position_row = stoi(command);	
				/* judge if the row is valid */
				if(position_row < row && position_row >= 0){
					/* decrease the row counter */
					row--;
					/* calculate the target row's iterator */
					LList<LList<Cell*>>::iterator I = spreadsheet.begin();
					for(int i = 0; i < position_row; i++){ 
						I++;
					}
					/* loop over the row to delete all cells inside it */
					for(LList<Cell*>::iterator J = (*I).begin(); J !=(*I).end(); ++J) {
						delete *J;
					}
					/* delete the row */
					/* if it is the first row, use special method because we need to modified the head of the linked list */
					if(position_row == 0){
						spreadsheet.erase_first(I);
					}
					else{
						spreadsheet.erase(I);
					}
					/* loop over the spreadsheet to set specific functional cell's value to none valid if the row they depend on is removed. We can achieve it by simply matching the row number. Also, we need to decrease the row member of part of the functional cell as what is discussed about above */
					for(LList<LList<Cell*>>::iterator I = spreadsheet.begin(); I != spreadsheet.end(); ++I){
						for(LList<Cell*>::iterator J = (*I).begin(); J !=(*I).end(); ++J) {
							if((*J) -> get_cell_type() == 2){
								if((*J) -> get_row() == position_row){
									(*J) -> set_to_deleted();
								}
								else if((*J) -> get_row() > position_row){
									(*J) -> row_decrease();
								}
							}
						}
					}
					/* call the reupdate function to update the function cell that has a depth larger than 1 */
					reupdate(spreadsheet);
				}
				/* prompt if the row is invalid */
				else{
					cout << "Error: row out of range" << endl;
				}
			}
			/* if the user does not input number, prompt */
			catch(exception e){
				cout << "Error: row out of range" << endl;
			}
			
		}
		/* processing the quit command */
		else if(command == "quit"){
			/* loop over the spreadsheet to delete all cell we use new to create */
			for(LList<LList<Cell*>>::iterator I = spreadsheet.begin(); I != spreadsheet.end(); ++I){
				for(LList<Cell*>::iterator J = (*I).begin(); J !=(*I).end(); ++J) {
					delete *J;
				}
			}
			return 0;
		}
		/* if the command can not match any type we defined, prompt */
		else{
			cout << "Error: unknown command" << endl;
		}
	}

	return 0;
}