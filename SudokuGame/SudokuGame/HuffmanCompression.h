class HNODE{
public:
	unsigned char value;
	int times;
	string encode;
	HNODE* left;
	HNODE* right;
	HNODE* next;
	bool beenCheck;
	void clone(HNODE*);
	HNODE(){ value = '\0'; times = 0; encode = ""; beenCheck = false;  left = NULL; right = NULL; next = NULL; };
};

class Huffman_Tree{
public:
	HNODE* nodes;
	char theUnique;
	char* theUniqueEncode;
	bool addToTree(char);
	bool addToTree(HNODE*);
	bool addNewNode(HNODE*, HNODE*);
	bool createTree();
	bool saveEncode();
	bool smallToBig();
	void reverseLinkList();
	bool saveToFile(string, string);
	bool findUniqueEncode();
	string convertToStringBits(string);
	string headerToBits(string fileName);
	void deleteUniqueInHeader();
	Huffman_Tree(){ nodes = NULL; theUnique = '\0'; };
};

struct Huffman_Order{

	string order;
	string srcFileName;
	string desFileName;

};

bool huffmanCompression( Huffman_Order order);