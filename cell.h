/*
The cell.h file for Programing Finance's final project

Written by Zihong Zheng
*/

#ifndef __CELL_H__
#define __CELL_H__
#include <sstream>
#include <limits>
#include "list.h"
using namespace std;
#include <iostream>
/* define the depth for functional cell, see the description in final.cpp */
extern int function_depth;

/* define the Cell class, it is the base class of cell_double, cell_string and cell_functional */
class Cell {
	public:
	/* lots of virtual methods are defined here, most of them are needed by the child classes, their functions would be explained after */
	/* return the initiate value of a normal cell for print */
	virtual string toString(){
		return "";
	}

	virtual double get_value(){
		/* what value return here is meaningless because we would never call the get value function of a normal cell */
		return 3.1415926;
	}
	/*return the type value, 0 stands for none numeric*/
	virtual int get_cell_type(){
		return 0;
	}

	virtual void update(){}

	virtual int get_row(){
		/* what value return here is meaningless because we would never call the get row function of a normal cell */
		return 0;
	}

	virtual void row_decrease(){}

	virtual void row_increase(){}

	virtual void set_to_deleted(){}

	virtual int get_depth(){
		/* 0 is returned for calculating the max depth, notice that non functional cell's depth is always smaller than that of functional cell */
		return 0;
	};

	~Cell(){
		/* used for debuging whether we delete all things when we exit the program */
		//cout << "deleting a cell" << endl;
	}

};

/* define the Cell_string for user to set a string cell */
class Cell_string : public Cell{
	private:
	/* the value of the string cell */
	string value;
	public:
	/* constructor for Cell_string */
	Cell_string(string v){
		value = v;
	}
	/* this function is used to return string cell's value for print */
	string toString(){
		return value;
	}
	/* this function is used to return the type of the cell, 0 means neither double cell nor functional cell */
	int get_cell_type(){
		return 0;
	}
};

class Cell_double : public Cell{
	private:
	/* the value of the double cell */
	double value;
	public:
	/* constructor for Cell_double */
	Cell_double(){}
	Cell_double(double v){
		value = v;
	}
	/* this function is used to return double cell's value for print */
	string toString(){
		return to_string(value);
	}
	/* get_value is used to return the double cell's value for functional cells to calculate */
	double get_value(){
		return value;
	}
	/* this function is used to return the type of the cell, 0 means double cell */
	int get_cell_type(){
		return 1;
	}

};

class Cell_functional : public Cell_double{
	private:
	/* the value of the functional cell */
	double value;
	/* this variable records the funtional type of this functional cell, 0 for mean, 1 for max and 2 for min */
	int function_type;
	/* defined the begin and the end iterator for calculating */
	LList<Cell*>::iterator begin;
	LList<Cell*>::iterator end;
	/* the function depth of this functional cell, it is very similar to what we said in the first part of the final.cpp file */
	int depth;
	/* this bool variable is used to indicate whether the row this functional cell depends on is deleted */
	bool deleted;

	public:
	/* this int variable records which row this functional cell depends on */
	int row;
	/* constructor for Cell_functional */
	Cell_functional(LList<Cell*>::iterator cal_begin, LList<Cell*>::iterator cal_end, int type, int cal_row){
		/* set up the initial value for the functional cell */
		begin = cal_begin;
		end = cal_end;
		row = cal_row;
		function_type = type;
		deleted = false;
		depth = 1;
		/* call the update method to calculate the value first */
		update();
	}
	/* update method for the functional cell to update their value when the cell they depend on change their value */
	void update(){
		/* judge if this functional cell is still valid. If not, do not update*/
		if(!deleted){
			/* define the counter to count the amount of number in the range which this cell used to calculate */
			int num_count = 0;
			/* initiate the sum for calculate mean, max and min */
			double sum = 0;
			/* define max_depth to record the max depth encountered in the loop */
			int max_depth;
			/* loop over the range to calculate the value */
			/* if begin == end, we would use another method */
			if(begin != end){
				/* entering the loop */
				for(LList<Cell*>::iterator I = begin; I != end; I++){
					/* judge if the cell is a double cell or a functional cell */
					if((*I) -> get_cell_type() != 0){
						/* if we find a double cell or a functional cell, increase the number counter */
						num_count++;
						/* if type is mean, calculate the sum of those cells' values */
						if(function_type == 0){
							sum += (*I) -> get_value();
						}
						/* if type is max, calculate the max value of those cells' */
						else if (function_type == 1){
							/* if it is the first cell, just record its value without comparing */
							if(num_count == 1){
								sum = (*I) -> get_value();
							}
							/* if it is not the first numeric cell, compare its value to th former one, if it is larger, record its value */
							else{
								if(sum < (*I) -> get_value()){
									sum = (*I) -> get_value();
								}
							}
						}
						/* if type is min, calculate the min value of those cells' */
						else{
							/* if it is the first numeric cell, just record its value without comparing */
							if(num_count == 1){
								sum = (*I) -> get_value();
							}
							/* if it is not the first cell, compare its value to th former one, if it is smaller, record its value */
							else{
								if(sum > (*I) -> get_value()){
									sum = (*I) -> get_value();
			 					}
							}
						}
						/*if it is the first numeric cell, record its depth */
						if(num_count == 1){
							max_depth = (*I) -> get_depth();
						}
						/* if it is not the first numeric cell, compare its depth to the former and record the larger one. Notice that the depth of none functional cell is always smaller than that of functional cell*/
						else{
							if(max_depth < (*I) -> get_depth()){
								max_depth = (*I) -> get_depth();
							}
						}
					}
				}
				/* no number found, set the value to none number */
				if(num_count == 0){
					value = numeric_limits<double>::quiet_NaN();
				}
				/* numbers found, set corresponding value */
				else{
					if(function_type == 0){
						value = sum / double(num_count);
					}
					else{
						value = sum;
					}
					/* set this functional cell's depth to (1 + the max depth found in this loop) */
					depth = max_depth + 1;
					/* if the depth is larger than the global depth, update its value */
					if(depth > function_depth){
						function_depth = depth;
					}
				}
			}
			/* if begin == end, set this functional cell's value the same as the target's or none number */
			else{
				/* judge if target cell is a double cell or a functional cell */
				if((*begin) -> get_cell_type() != 0){
					value = (*begin) -> get_value();
				}
				else{
					value = numeric_limits<double>::quiet_NaN();
				}
			}

		}
	}
	/* return the type of the cell, 2 means functional cell */
	int get_cell_type(){
		return 2;
	}
	/* return the depth of the functional cell */
	int get_depth(){
		return depth;
	}
	/* return which row this functional cell depends on */
	int get_row(){
		return row;
	}
	/* decease the row by 1 */
	void row_decrease(){
		row--;
	}
	/* increase the row by 1 */
	void row_increase(){
		row++;
	}
	/* when the row this functional cell depends on is deleted, set it value to none number and modified it deleted bit */
	void set_to_deleted(){
		value = numeric_limits<double>::quiet_NaN();
		deleted = true;
	}
	/* return the value for print */
	string toString(){
		return to_string(value);
	}
	/* return the value for calculate */
	double get_value(){
		return value;
	}
};

#endif