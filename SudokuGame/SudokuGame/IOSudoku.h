
using namespace std;
class dataSudoku{
public:
	int temp;
	int no;
	void addData();
	bool readData( ifstream & );
	bool checkAnswer();
	void printSudoku();
	Number* Sudoku(){ return sudoku; };
	void convertToNoAnswer();
	int Speed(){ return speed; };
	int Size(){ return size; };
	bool clone( dataSudoku );
	bool deleteData();
	dataSudoku();
	dataSudoku(int, int, int, Number*);
private:
	int speed;
	int size;
	Number* sudoku;
};

class DataList{
public:
	int size;
	dataSudoku* data;
	DataList(int);
	DataList(){ size = 0; data = NULL; };
	bool readMyDataToList();
	void rewriteData(int);
	bool sortDataSmallToBig();
	bool add(dataSudoku);
	void deleteData(int,int);
	bool isRepeat(dataSudoku);
	int getNumOfType(int);
	int DataList::convertSizeNoToIndex(int type, int no);
};

class PlayerRank{
public:
	string Name;
	int type;
	int correctNum;
	string Date;
	PlayerRank(){ correctNum = 0; Name = "", Date = ""; };

	
	void SaveRank();
	void ClearRank();
};

class RankList{
public:
	int length(){ return len; };
	PlayerRank* list;
	RankList(){ list = NULL; };
	bool add(PlayerRank);
    bool ReadRankList();
	bool SortRankList();
	void SaveRankList();
private:
	int len;
};

class orderSudoku
{
public:
	int order;
	int size;
	int no;
	int noDes;
	int sumOfAdd; // 新增題目時用到
	int numOfQuestion[10];
	orderSudoku(){ order = 0, size = 2, no = 0, noDes = 1, sumOfAdd = 0; for (int i = 0; i < 10; i++) numOfQuestion[i] = 0; };
	dataSudoku data;
    PlayerRank thePlayer;
};

int sudokuIO( orderSudoku* );