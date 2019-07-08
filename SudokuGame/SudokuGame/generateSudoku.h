

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
		int row ; // ª½ 
		int column ; // ¾î 
		int block ; // °Ï¶ô 

};

void generateSudoku( Number*, int );

