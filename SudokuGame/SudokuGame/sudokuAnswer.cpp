#include "stdafx.h"

int sudokuAnswer::convertToUserType(dataSudoku ds){

	sudoku = ds.Sudoku();
	for (int i = 0; i < ds.Size()*ds.Size(); i++){

		if (sudoku[i].hide) sudoku[i].num = 0;
		
	}

	return 0;
}