#include "stdafx.h"

HNODE* head = NULL, *current = NULL, *tail = NULL;
unsigned char setBits(char*);
char* readBits(unsigned char);
void deleteLinkList(HNODE* de);

void Huffman_Tree::reverseLinkList(){
	HNODE* prev = NULL;
	HNODE* current = head;
	HNODE* next = NULL;
	while (current != NULL) {
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	} // while

	head = prev;
	nodes = head;
}

bool Huffman_Tree::smallToBig(){
	nodes = head;
	head = NULL;
	for (HNODE* i = nodes; i != NULL; i = i->next) {
		if (i->times != 0) {
			HNODE* temp = new HNODE;
			temp->value = i->value;
			temp->times = i->times;
			temp->encode = i->encode;
			temp->beenCheck = i->beenCheck;
			temp->left = i->left;
			temp->right = i->right;
			addToTree(temp);
			//cout << "smallToBig--" << temp->value << " " << temp->times << endl;
		} // if
		else delete i;
	} // for

	nodes = head;
	return true;
} //

bool Huffman_Tree::addToTree(HNODE* node){

	if (head == NULL) {
		head = node;
		tail = head;
		return true;
	} // if


	if (node->times < head->times) { // smaller than head
		node->next = head;
		head = node;
		return true;
	} // if

	for (HNODE* i = head; i->next != NULL; i = i->next){

		if (node->times < i->next->times) {
			node->next = i->next;
			i->next = node;
			return true;
		}
	} // for

	tail->next = node;
	tail = node;
	//length++;
	return true;
}

bool Huffman_Tree::addNewNode(HNODE* left, HNODE* right){

	HNODE* parent = new HNODE;
	parent->left = left;
	parent->right = right;
	parent->times = left->times + right->times;
	addToTree(parent);

	return true;
}

bool Huffman_Tree::createTree(){

	bool findLeft = false;
	HNODE* left = NULL, *right = NULL;
	HNODE* i = head;

	while (i != NULL){
		for (i = head; i != NULL && i->times != 0; i = i->next) {

			if (!i->beenCheck) {

				if (!findLeft) {
					left = i;
					findLeft = true;
					i->beenCheck = true;
				}
				else  {
					right = i;
					i->beenCheck = true;
					addNewNode(left, right);
					break;
				} // else
			} // if
		} // for

		findLeft = false;
		//Sleep( 1000 );
	} // while


	return true;
}

bool Huffman_Tree::saveEncode(){



	for (HNODE* i = nodes; i != NULL; i = i->next) {

		if (i->left != NULL) {

			i->left->encode = i->encode + "0";

		} // if
		if (i->right != NULL) {
			i->right->encode = i->encode + "1";

		} // if
	} // for

	HNODE* newList = NULL, *newListTail = NULL;
	for (HNODE* i = nodes; i != NULL; i = i->next)
		if (i->value != '\0') {
			if (newList == NULL) {
				newList = i;
				newListTail = i;
			} // if
			else {
				newListTail->next = i;
				newListTail = i;
			} // else
		} // if

	nodes = newList;
	return true;
}

string Huffman_Tree::headerToBits(string fileName ){

	FILE* out;
	string headerFileName = fileName;
	headerFileName = headerFileName.erase(headerFileName.find("."), headerFileName.length() - headerFileName.find(".")) + ".header";
	fopen_s(&out, headerFileName.c_str(), "wb");

	int n = 0;
	char space = ' ';
	for (HNODE* i = nodes; i != NULL; i = i->next){
		
		fwrite((char*)&i->value, sizeof(unsigned char), 1, out);
		fwrite((char*)i->encode.c_str(), i->encode.length(), 1, out);
		if (i->next != NULL) fwrite((char*)&space, sizeof(unsigned char), 1, out);
		n++;
	}

	fclose( out );
	return "";
}

bool Huffman_Tree::saveToFile(string fileName, string data){
	
	headerToBits(fileName);
	unsigned char ch;
	char chBits[9] = { 0 };
	string afData = "";
	string allData = data;

	while (allData.length() % 8 != 0) allData += "0";
	FILE* fout;
	cout << fileName.c_str() << endl;
	fopen_s(&fout, fileName.c_str(), "wb");
	if (fout == NULL) return false;

	afData = "";
	for (int i = 0; i < allData.length(); i += 8) {
		for (int j = 0; j < 8; j++) {
			chBits[j] = allData[i + j];
		} // for

		chBits[8] = '\0';
		ch = setBits(chBits);
		fwrite(&ch, sizeof(unsigned char), 1, fout);
		afData += ch;
	} // while

	allData = allData;
	fclose(fout);
	return true;
} // 

string readDataToString(string fileName){
	FILE* fin;
	fopen_s(&fin, fileName.c_str(), "rb");

	unsigned char ch = '\0';
	string returnData = "";

	while (fscanf_s(fin, "%c", &ch) == 1){
		returnData += ch;
	} // while

	fclose(fin);
	return returnData;
}

string Huffman_Tree::convertToStringBits(string data){
	string returnData("");

	HNODE* tempNodes = new HNODE[256];
	HNODE* i;
	for (i = nodes; i != NULL; i = i->next) 
	    tempNodes[(int)i->value].encode = i->encode;

	int len = data.length();
	for (int n = 0; n < len; n++) {
	    if ((int)((unsigned)data[n]) > 255 || (int)((unsigned)data[n]) < 0) cout << "中文或其他字符" << (int)((unsigned)data[n]) << endl;
	    else returnData += tempNodes[(int)((unsigned)data[n])].encode;

	} // for

	delete[] tempNodes;
	return returnData;
}

void deleteList(HNODE* node){
	if (node->next != NULL)
		deleteList(node->next);
	delete node;
}


void deleteLinkList(HNODE* de){
	if (de->next != NULL)
		deleteLinkList(de->next);
	delete de;
}

unsigned char setBits(char* chs){
	unsigned char byte = 0, temp;
	if (chs[0] == '1') byte = 1;
	for (int i = 1; i < 8; i++) {
		if (chs[i] == '0') byte = byte << 1;
		else {
			byte = byte << 1;
			byte = byte | 1;
		} // else
	} // for

	return byte;
}

char* readBits(unsigned char ch){
	char* aa = new char[9];
	char temp = 1;
	for (int i = 7; i >= 0; i--) {
		if (ch & 1) aa[i] = '1';
		else aa[i] = '0';
		if (i > 0) ch >>= 1;
	}

	aa[8] = '\0';
	return aa;
}

bool compressFile(string);
bool decompressFile(string);
/****************************************** Begin ***************************************************************/
bool huffmanCompression( Huffman_Order order)
{

	if (strcmp(order.order.c_str(), "compress") == 0) {

		return compressFile(order.srcFileName);
	}
	else {
		return decompressFile( order.srcFileName );
	}

	return true;
}

bool compressFile(string fileName){

	Huffman_Tree huffmanTree;
	huffmanTree.nodes = new HNODE[256];
	ifstream fin(fileName);
	if (!fin) return false;
	fin.close();

	FILE* file;
	string data = "";
	fopen_s(&file, fileName.c_str(), "rb");
	unsigned char ch;
	while (fscanf_s(file, "%c", &ch) == 1){
		if ((int)ch != 13){
			huffmanTree.nodes[(int)ch].value = ch;
			huffmanTree.nodes[(int)ch].times++;
			data += ch;
		}// if
	}// while

	fclose( file );
	for (int i = 0; i < 256; i++) 
	    if (huffmanTree.nodes[i].times != 0) huffmanTree.addToTree(&huffmanTree.nodes[i]);

	huffmanTree.smallToBig();
	huffmanTree.createTree();
	huffmanTree.reverseLinkList();
	huffmanTree.saveEncode();

	string dataBits = huffmanTree.convertToStringBits(data);
	huffmanTree.saveToFile(fileName, dataBits);
	
	return true;
} //

bool addToDecompressThree(HNODE*, char);
bool decompressFile(string fileName){

	HNODE* nodes = new HNODE, *nodesHead = NULL, *n;
	unsigned char ch, value;
	string encode = "";
	string headerFileName = fileName;
	headerFileName = headerFileName.erase(headerFileName.find("."), headerFileName.length() - headerFileName.find(".")) + ".header";
	ifstream fExist(headerFileName);
	if (!fExist) return false;
	fExist.close();
	FILE* fin;
	fopen_s(&fin, headerFileName.c_str(),"rb");
	fread( (char*)&ch, sizeof(unsigned char), 1, fin );
	// ch = fin.get();
	int num;

	if (nodesHead == NULL) nodesHead = nodes;
	while (!feof(fin)) {
		
		value = ch;
		while (fread((char*)&ch, sizeof(unsigned char), 1, fin) == 1 && (ch == '1' || ch == '0')) {
			encode += ch;
			if (ch == '0') {// left
				if (nodes->left == NULL) 
					nodes->left = new HNODE;
				nodes = nodes->left;
			} // if
			else if (ch == '1') {// right
				if (nodes->right == NULL)
					nodes->right = new HNODE;
				nodes = nodes->right;
			} // else if
		} // while

		nodes->encode += encode;
		nodes->value = value;
		nodes = nodesHead;
		encode = "";
		fread((char*)&ch, sizeof(unsigned char), 1, fin);
	}

	fclose(fin);
	FILE* unc;
	fopen_s(&unc, fileName.c_str(), "rb");
	string bitsData = "", allBitsData = "", fuckt = "";
	char* newCh = NULL;

	int isEnd = 0;
	while (fscanf_s(unc, "%c", &ch) == 1){
		delete newCh;
		newCh = readBits(ch);
		bitsData += newCh;
	}// while

	fclose(unc);
	while (bitsData.length() % 8 != 0) bitsData.erase(bitsData.length() - 1, 1);

	nodes = nodesHead;
	string newFile = fileName;
	ofstream fout(newFile.c_str());
	string Data = "", temp = "";
	char chBits[8];

	for (int i = 0; i < bitsData.length(); i++) {
		if (temp.length() <= 0) temp = bitsData[i];
		else temp += bitsData[i];
		if (bitsData[i] == '0') nodes = nodes->left;
		else if (bitsData[i] == '1') nodes = nodes->right;

		if (nodes->encode.length() != 0 ) {
			Data += nodes->value;
			temp = "";
			nodes = nodesHead;
		} // if

	} // for

	fout << Data.c_str() << endl;
	fout.close();

	if (head != NULL) deleteList(head);
	head = NULL;
	return true;
}