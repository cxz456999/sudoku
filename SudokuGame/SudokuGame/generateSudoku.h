

class Number{
	public:
		int num ;
		bool hide;
		void setPosition( int );
		void setValue( int );
		int value(void);
		bool isRepeat( Number*, int, int );
		Number(); 
	private:
		int row ; // �� 
		int column ; // �� 
		int block ; // �϶� 

};

void generateSudoku( Number*, int );

