#include "stdafx.h"

int getTotalSudokuNum();

unsigned long getN(int minM, unsigned long maxM);
void printNum(int, Number*);
dataSudoku generateNew(int, int);

/*************************************** class PlayerRank  *************************************************/
/*

for (int i = 0; !fin.eof(); i++ ){

if (i != 0 && i % 10 == 0) {
temp = new PlayerRank[i];
saveRankTo(i, list, temp);
delete[] list;
list = new PlayerRank[i + 10];
saveRankTo(i, temp, list);
delete[] temp;
if (i == 0) MessageBox(NULL, L"", L"", MB_OK);
i++;
} // if

fin >> list[i].Name >> buffer; // name type
list[i].type = atoi( buffer );
fin >> buffer;
list[i].correctNum = atoi( buffer );
fin >> buffer >> date_Month >> date_Day >> date_Time >> list[i].Date;
strcat_s(list[i].Date, " ");
strcat_s(list[i].Date, date_Month);
strcat_s(list[i].Date, "/");
strcat_s(list[i].Date, date_Day);
strcat_s(list[i].Date, " ");
strcat_s(list[i].Date, date_Time);



len = i;
}

*/

void saveRankTo(int length, PlayerRank* list, PlayerRank* temp) {

	for (int j = 0; j < length; j++) {
		temp[j].Name = list[j].Name;
		temp[j].type = list[j].type;
		temp[j].correctNum = list[j].correctNum;
		temp[j].Date = list[j].Date;
	} // for
}


bool RankList::add(PlayerRank thePlayer) {

	//if (len == 0) MessageBox(NULL, L"0", L"", MB_OK);
	//return false;
	PlayerRank* temp = new PlayerRank[len + 1];

	saveRankTo(len, list, temp);
	delete[] list;
	list = temp;
	list[len].Name = thePlayer.Name;
	list[len].type = thePlayer.type;
	list[len].correctNum = thePlayer.correctNum;
	list[len].Date = thePlayer.Date;
	len++;

	return true;
}

bool RankList::SortRankList() {

	if (len <= 0) return false;

	PlayerRank* temp = new PlayerRank[len + 1];
	int index = 0;
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			if (i != j) {
				if (list[i].type < list[j].type && list[i].correctNum == 0) index++; // 4  0 vs 16 0
				else if (list[i].correctNum != 0 && list[j].correctNum != 0 && list[i].type * list[i].correctNum < list[j].type * list[j].correctNum) index++; // 4 1 vs 16 1 
				else if (list[i].type == list[j].type && list[i].correctNum < list[j].correctNum) index++; // 4 1  vs 4 2 
			} // if
		} // for

		while (temp[index].Name.length() > 0 && index < len) index++; // 若都相同, 没此行会导致所有资料一直在 temp[0] -> temp[1]...没东西, 因此产生乱码
		temp[index].Name = list[i].Name;
		temp[index].type = list[i].type;
		temp[index].correctNum = list[i].correctNum;
		temp[index].Date = list[i].Date;

		index = 0;
	} // for

	//delete[] list;
	//list = temp;
   //	wsprintf(buffer, L"list %S", list[1].Name.c_str());

	//MessageBox(NULL, buffer, L"", MB_OK);
	saveRankTo(len, temp, list);
	delete[] temp;

	//wsprintf(buffer, L"list %s", list[0].Name.c_str());
	//MessageBox(NULL, buffer, L"", MB_OK);
	return true;
}

bool RankList::ReadRankList() {
	ifstream fin("sudokuRank.data");
	if (!fin) {
		ofstream create("sudokuRank.data");
		create << "0" << endl;
		create.close();
		fin.close();
		return false;
	} // if    

	len = 0;
	char buffer[80], date[40];

	fin >> buffer;
	len = atoi(buffer);
	if (len <= 0) return false;
	delete[] list;
	list = new PlayerRank[len];

	for (int i = 0; i < len; i++) {

		fin >> buffer;
		list[i].Name = buffer;

		fin >> buffer; // name type	

		list[i].type = atoi(buffer);
		fin >> buffer;
		list[i].correctNum = atoi(buffer);
		fin >> list[i].Date >> date;
		list[i].Date += " ";
		list[i].Date += date;

	} // for


	fin.close();
	return true;
}

void RankList::SaveRankList() {


	ofstream create("sudokuRank.data");
	if (len > 18) len = 18;
	create << len << endl;
	create.close();

	for (int i = 0; i < len; i++) {
		list[i].SaveRank();
	}

	//if (len == 1 ) MessageBox(NULL, L"", L"", MB_OK);
}

void PlayerRank::SaveRank() {

	ifstream open("sudokuRank.data");
	if (!open) {
		ofstream create("sudokuRank.data");
		create << "0";
		create.close();
	} // if

	open.close();
	ofstream fout("sudokuRank.data", ios::app);
	char strType[256], strNum[256];
	sprintf_s(strType, "%d", type); // IMPORTANT!!! 要换成字串存
	sprintf_s(strNum, "%d", correctNum);
	fout << Name << " " << strType << " " << strNum << " " << Date << endl;
	fout.close();
}

void PlayerRank::ClearRank() {
	ofstream create("sudokuRank.data");
	create.close();
}

/*************************************** class dataSudoku  *************************************************/

dataSudoku::dataSudoku() {
	no = speed = size = -1;
}

bool dataSudoku::deleteData() {
	if (sudoku != NULL) delete[] sudoku;
	no = speed = size = -1;
	sudoku = NULL;
	return true;
}

dataSudoku::dataSudoku(int No, int Speed, int Size, Number* Num) {
	no = No;
	speed = Speed;
	size = Size;
	sudoku = Num;
}

void dataSudoku::addData() {

	ofstream fout("dataSudoku.data", ios::out | ios::app);
	fout << "No " << no << endl;
	fout << "Speed " << speed << endl;
	fout << "Size " << size << endl;
	fout << "Hide ";
	int numOfHide = 0;
	for (int i = 0; i < size * size; i++)
		if (sudoku[i].hide) numOfHide++;
	fout << numOfHide << endl;
	for (int i = 0; i < size * size; i++)
		if (sudoku[i].hide) fout << i << " ";
	fout << endl;

	fout << "Sudoku " << endl;
	for (int i = 0; i < size * size; i++) {
		fout << sudoku[i].value() << " ";
		if ((i + 1) % (size) == 0 && i != 0) fout << endl;
	} // for


} // outputData()

bool dataSudoku::readData(ifstream& fin) {
	char buffer[80];
	char temp[80];
	streampos sp;
	if (!fin) return false;

	fin >> temp >> buffer; // No
	no = atoi(buffer);


	fin >> temp >> buffer; // Speed
	speed = atoi(buffer);


	fin >> temp >> buffer; // Size
	size = atoi(buffer);


	fin >> temp >> buffer; // Hide hide 几个  
	int* poHide = new int[atoi(buffer)];
	int n = 0;
	while (strcmp(buffer, "Sudoku") != 0) {
		fin >> buffer;
		poHide[n++] = atoi(buffer);
	} // while

	//fin >> temp; // Sudoku
	sudoku = new Number[size * size];
	for (int i = 0; i < size * size; i++) {
		fin >> buffer;
		sudoku[i].setPosition(i);
		sudoku[i].setValue(atoi(buffer));
	} // for

	for (int i = 0; i < n; i++)
		sudoku[poHide[i]].hide = true;


	return true;
}

void dataSudoku::convertToNoAnswer() {
	for (int i = 0; i < size * size; i++)
		if (sudoku[i].hide) sudoku[i].num = 0;
}

bool dataSudoku::clone(dataSudoku data) {
	sudoku = data.Sudoku();
	speed = data.Speed();
	size = data.Size();
	no = data.no;
	return true;
}

/*************************************** class DataList  *************************************************/

DataList::DataList(int n) {
	data = new dataSudoku[n];
	size = n;
}

bool DataList::readMyDataToList() {
	ifstream fin("dataSudoku.data", ios::in);
	if (!fin) return false;
	char temp[80];
	fin >> temp;

	for (int i = 0; i < size; i++) {
		data[i].readData(fin);
	}

	fin.close();
	return true;
}

void DataList::deleteData(int index, int num) {

	TCHAR str[256];
	wsprintf(str, TEXT("%d and %d"), index, num);
	//MessageBox(NULL, str, L"", MB_OK);
	//if ( index + num > size) num = size - No;
	for (int i = index + num; i < size; i++) {
		//wsprintf(str, TEXT("delete From %d No: %d and size %d \n"), i-1, data[i-1].no, data[i-1].Size());
		//MessageBox(NULL, str, L"", MB_OK);
		data[i - num].clone(data[i]);
		data[i - num].no = i;
	} // for

	size -= num;
	rewriteData(size);
}

bool DataList::add(dataSudoku newData) {
	dataSudoku* newList = new dataSudoku[size + 1];
	for (int i = 0; i < size; i++) {
		newList[i].clone(data[i]);
	} // for

	delete[] data;
	newList[size].clone(newData);
	size++;
	data = newList;
	return true;
}

bool DataList::sortDataSmallToBig() {
	dataSudoku* newList = new dataSudoku[size];
	int index = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++)
			if (i != j && data[i].Size() > data[j].Size()) index++; // 不能 = 
		while (newList[index].no > 0) index++; // 此位置有东西时后挪一个
		newList[index].clone(data[i]);
		newList[index].no = index + 1;
		index = 0;
	} // for

	if (data != NULL) delete[] data;
	data = newList;
	return true;
}

void DataList::rewriteData(int aftNo) {
	TCHAR str[256];
	ofstream fout("dataSudoku.data");
	fout << aftNo << endl;
	fout.close();
	for (int i = 0; i < size; i++) {
		data[i].addData();

	} // for
} // rewriteData()

bool DataList::isRepeat(dataSudoku ds) {

	if (size == 0) return false;

	for (int i = 0; i < size; i++) {
		if (ds.Size() == data[i].Size()) {
			for (int j = 0; j < ds.Size() * ds.Size(); j++) {
				if (ds.Sudoku()[j].num != data[i].Sudoku()[j].num) return false;
				if (ds.Sudoku()[j].hide != data[i].Sudoku()[j].hide) return false;
			} // for
		} // if
		else return false;
	} // for

	return true;
}

int DataList::getNumOfType(int Type) {
	int num = 0;
	for (int i = 0; i < size; i++) {
		if (data[i].Size() == Type * Type) num++;
	} // for

	return num;
} // getNumBySize

int DataList::convertSizeNoToIndex(int Type, int no) {
	int i = 0, indexOfSize = 0;
	for (i = 0; i < size; i++) {
		if (data[i].Size() == Type * Type) {
			indexOfSize++;
			if (indexOfSize == no) return i;
		} // if
	} // for

	return -1;

} // getNumBySize

/*************************************** Begin  *************************************************/

int sudokuIO(orderSudoku* order) {

	int timesRepeat = 0, index = 0;
	int n = getTotalSudokuNum();
	if (n < 0) return 0;


	dataSudoku* newData = new dataSudoku[order->sumOfAdd];
	DataList dataList(n);
	dataList.readMyDataToList();

	if (order->order == 0) {
		for (int i = 0; i < 9; i++) {
			order->numOfQuestion[i] = dataList.getNumOfType(i + 2);
		} // for
	} // if

	else if (order->order == 1) {
		//cout << "目前 " << n << "题" << endl; 
		for (int i = n + 1; i <= n + order->sumOfAdd; i++) {
			newData[i - n - 1] = generateNew(i, order->size);

			while (dataList.isRepeat(newData[i - n - 1]) && timesRepeat++ < 100) newData[i - n - 1] = generateNew(i, order->size); // 如果题型重复就重新产生 
			if (timesRepeat >= 100) {
				return -1;
			} // if

			//if ( i == n+1 ) dataList.rewriteData(n+order->sumOfAdd); // 已有的题目只需更新一次题号
			//newData[i-n-1].addData();
			dataList.add(newData[i - n - 1]);
		} // for

		dataList.sortDataSmallToBig();
		dataList.rewriteData(dataList.size);
		delete[] newData;
	}

	else if (order->order == 2) {
		index = dataList.convertSizeNoToIndex(order->size, order->no);
		order->data = dataList.data[index];
		order->numOfQuestion[order->size] = dataList.getNumOfType(order->size);
	}
	else if (order->order == 3) { // 照大小顺序存题目
		TCHAR str[256];
		index = dataList.convertSizeNoToIndex(order->size, order->no);
		dataList.deleteData(index, order->noDes);
	}
	else if (order->order == 4) { // 储存排名

	}

	return 0;
}

dataSudoku generateNew(int n, int size) {
	int  num = 0;
	//cout << "输入leve( 2~10 )( 2 -> 4*4 数独 3 -> 9*9 数独)" << endl; 
	size *= size;
	Number* Num = new Number[size * size];
	generateSudoku(Num, size);
	//printSudoku( size, Num );
	//modifyTotalNo( n+1 );
	dataSudoku dS(n, 0, size, Num);
	return dS;
	//dS.addData();
}

int getTotalSudokuNum() {
	ifstream file("dataSudoku.data", ios::in);
	char charNum[256];

	if (!file) {
		//cout << "建立题库" << endl;
		ofstream create("dataSudoku.data");
		create << "0" << endl;
		create.close();
		//file.open( "dataSudoku.data", ios::out | ios::app );
	}
	else {
		file >> charNum;
	} // else

	file.close();

	return atoi(charNum);

}


/*
unsigned long getN(int minM, unsigned long maxM)                                        // 函式定义：取得一个正整数
{   unsigned long   M;                                                                  // 函式参数：最小值、最大值，传回落在范围内的值
	string          inputS;
	bool            isNum;

	do {
		cout << endl << "Input a number: ";
		cin >> inputS;
		isNum = true;
		for (int i = 0; i < (int)inputS.length(); i++)                                       // 禁止输入非数字的字串
			if ((inputS[i] > '9') || (inputS[i] < '0'))
				isNum = false;
		if (!isNum)
			continue;
		M = strtoul(inputS.c_str(), NULL, 10);                                          // 将字串转为以十为底的长整数
		if ((M >= minM) && (M <= maxM))                                                 // 禁止输入未落在[最小值, 最大值]范围内的数值
			break;
		else
			cout << endl << "### The number must be in [" << minM << "," << maxM << "] ###" << endl;
	} while (true);   // end do-while
	return M;                                                                                   //成功输入才可继续
}   // end getM

*/


