#include "stdafx.h"

int format = 4;
void Number::setPosition(int i) {
	row = i % format + 1;
	column = i / format + 1;

	int sqrtFormat = (int)sqrt(format);
	for (int i = 1; i <= sqrtFormat; i++) { // column
		for (int j = 1; j <= sqrtFormat; j++) { // row 
			if (column <= i * sqrtFormat && row <= j * sqrtFormat) {
				block = (i - 1) * sqrtFormat + j;
				i = format;
				break;
			} // if
		} // for	    
	} // for

}

int Number::value() {
	return num;
}

Number::Number() {
	num = row = column = block = -1;
	hide = false;
}

void Number::setValue(int value) {
	num = value;
}

bool Number::isRepeat(Number* Num, int length, int value) {

	for (int i = 0; i < length; i++) {

		if (!(Num[i].row == row && Num[i].column == column) && ((Num[i].row == row || Num[i].column == column || Num[i].block == block) && Num[i].num == value))
			return true;
	} // for

	return false;
} // 

void hideAnswer(Number*);
void swapNumber(Number*, int, int);
int getValue(Number*, int, Number);
void output(int*);

void generateSudoku(Number* Num, int size) {

	format = size;
	//Number Num[ format * format ];
	int* rowValue = new int[format];
	int value = -1;


	for (int i = 0; i < format * format; i++) {
		Num[i].setPosition(i);
		value = getValue(Num, i, Num[i]);
		Num[i].setValue(value);
	} // for

	// 随机变换题型
	//srand(time(NULL));
	int times = 1 + rand() % format;
	for (int i = 0; i < times; i++) {
		//srand(time(NULL));
		int s1 = 1 + rand() % format;
		int s2 = 1 + rand() % format;
		swapNumber(Num, s1, s2);
	} // for

	// 随机隐藏 
	hideAnswer(Num);


}

void hideAnswer(Number* Num) {
	int sqrtFormat = (int)sqrt(format);
	int totalHole = (int)(format * format * 2 / 3) - 2, numHole = 1 + rand() % (format); // 最多可挖几个洞 && 目前要挖哪个 

	while (totalHole != 0) {
		//srand(time(NULL));
		numHole = rand() % (format * format);
		if (!Num[numHole].hide) {
			Num[numHole].hide = true;
			totalHole--;
		}
	} // while

} // hideAnswer()

void swapNumber(Number* Num, int s1, int s2) {

	int n1 = 0, n2 = 0;
	//srand(time(NULL));
	while (s2 == s1) s2 = 1 + rand() % format;

	int* s1Position = new int[format];
	int* s2Position = new int[format];

	for (int i = 0; i < format * format; i++) {
		if (Num[i].value() == s1)	s1Position[n1++] = i;
		else if (Num[i].value() == s2) Num[i].num = s1;
	} // for

	for (int i = 0; i < n1; i++) Num[s1Position[i]].num = s2;
} // swapNumber()

int getValue(Number* Num, int length, Number thisNum) {

	int i = length - format, value = -99;
	if (i < 0) value = 1;
	else value = Num[i].value() + 1;
	if (value > format) value = 1;

	int countToFormat = 0;
	for (i = value; i <= format; i++) {

		if (!thisNum.isRepeat(Num, length, i)) {
			return i;
		} // if

		if (i == format && ++countToFormat < format) i = 0;
	} // for

	return -100;
} // 


