#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
using namespace std;

class Node
{
public:
	int fq;//частота
	char sym;//символ
	Node *left, *right; 
	Node(){left=right=NULL;}

	Node(Node *L, Node *R){
		left =  L;
		right = R;
		fq = L->fq + R->fq;
	}
};


struct Compare
{
    bool operator()(const Node* l, const Node* r) const { return l->fq < r->fq; }
};


vector<bool> bool_code;                
map<char,vector<bool> > table;    

void AssocTable(Node *root){	
	if (root->left!=NULL){//пока есть левые потомки пишем 0
		bool_code.push_back(0);
                AssocTable(root->left);}
     
        if (root->right!=NULL){//вправо 1
                bool_code.push_back(1);
                AssocTable(root->right);}
   
        if (root->left==NULL && root->right==NULL)//для листьев приписываем код
        	table[root->sym]=bool_code;     
    
	bool_code.pop_back();//подняться на уровень
}


int main ()
{
setlocale(LC_ALL,"Russian");
int ch;
cout<<"Выберите действие:\n1)Закодировать файл\n2)Декодировать файл\n";
cin>>ch;
if (ch == 1){
	
	ifstream file("text1.txt", ios::out | ios::binary);
	
	map<char,int> letters;//ассоциативный массив символ - частота
	
	while (!file.eof()){ 
		char s = file.get(); 
	  	letters[s]++;}

	for( map<char,int>::iterator itr=letters.begin();itr!=letters.end(); ++itr){
		cout<<itr->first <<" : "<< itr->second;
		cout<<endl;}
  

	//записываем вершины в лист   	     
	list<Node*> t;
	for(map<char,int>::iterator itr=letters.begin(); itr!=letters.end(); ++itr){  
	Node *p = new Node;
	p->sym = itr->first;//char
	p->fq = itr->second;//int  
	t.push_back(p);  
	}	
	

		

	while (t.size()!=1){
		t.sort(Compare());//сортировать по частоте
    
     		Node *Left = t.front();//в начале самый редкий
     		t.pop_front();//выкинуть
    		Node *Right = t.front();//второй редкий
     		t.pop_front();
     
    		Node *Mother = new Node(Left, Right); //новый узел с суммой частот
     		t.push_back(Mother);
  		}
	
		Node *root = t.front();  //в конце останется только узел 

			

   	AssocTable(root);  //соответсвия символов и нового кода 
		


	file.clear(); file.seekg(0); 
    	
    	//открыть новый файл для закодированого сообщения
	ofstream g("output.txt", ios::out | ios::binary);
    	int SIZE = letters.size();
    	cout<<"ДЛИНА АЛФАВИТА!"<<" - "<<letters.size();
	g.write((char*)&SIZE, sizeof(int));//сначала записываем длину алфавита
    	
    	//потом переписываем символ(1байт)-частота(4байта)
	for (map<char,int>::iterator it = letters.begin(); it != letters.end(); it++){
		g.write((char*)&it->first, sizeof(char));
		g.write((char*)&it->second, sizeof(int));
		}
    	
    	int pos=0; char buf=0;
    	//идти по файлу 1 и записывать коды каждого символа в файл 2
    	while (!file.eof()){
    		char c = file.get();//берем символ
	  	vector<bool> x = table[c];//берем его код
	  	for(int n=0; n<x.size(); n++){//записываем код, пока байт не заполнится, потом сливаем его в файл
	   		buf = buf | x[n]<<(7-pos);   
	    		pos++;   
	    		if (pos==8) { pos=0;   g<<buf; buf=0; } 
       		}
    	}

    	file.close();
	g.close(); 
}		

if (ch == 2){

	ifstream G("output.txt", ios::out );//| ios::binary
	int numb, ii = 0, nn; char ss;
	G.read((char*)&numb, sizeof(int));//первый байт - количество cим-в в алф-те
	cout<<" СЧИТАТЬ "<<numb<<endl;
	map <char, int> lettersG; //ассоциативный массив символ - частота
	while (ii != numb){//считать из файла символ-частота 
		G.read((char*)&ss, sizeof(char));//символ
		G.read((char*)&nn, sizeof(int));//частота
		lettersG[ss] = nn;
		ii++;
		}	
		
	//проверка массива
	cout<<"\nСимвол-частота2!!!!!\n";
	map<char, int>::iterator iq; 
	for (iq = lettersG.begin(); iq != lettersG.end(); iq++) {
		cout << iq->first << " : " << iq->second << endl;}
	cout<<endl;	
		
	
	///Cocтавление дерева2

	//записываем вершины в лист
	list <Node*> tG;
	map <char, int>::iterator iqr;
	for (iqr = lettersG.begin(); iqr != lettersG.end(); ++iqr) {
		Node* P = new Node;
		P->sym = iqr->first;//char
		P->fq = iqr->second;//int
		tG.push_back(P);
	}

	while (tG.size() != 1){
		tG.sort(Compare());//сортировать по частоте
		
		Node* Left = tG.front();//в начале самый редкий
		tG.pop_front();//выкинуть
		Node* Right = tG.front();//второй редкий
		tG.pop_front();
		Node* Mother = new Node(Left, Right);//новый узел с суммой частот
		tG.push_back(Mother);
	}
	Node* ROOT = tG.front();//в конце останется только узел 

	AssocTable(ROOT);//соответсвия символов и нового кода


	Node *P = ROOT;
	int pos = 0; char byte;//pos!!!
	byte = G.get();
	while (!G.eof()){
		bool b = byte & (1 << (7 - pos));
		if (b) P = P->right; //1 вправо
		else P =P->left;//0 влево
		if (P->left==NULL && P->right==NULL) {cout<<P->sym; P = ROOT;}
		pos++;
		if(pos == 8) {pos = 0; byte =G.get();}//считывает новый симол, после заполнения байта;}
		}
	G.close();
}


	return 0;
}