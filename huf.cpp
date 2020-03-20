#include <iostream>
#include <map>
#include <fstream>
#include <list>
#include <vector>

using namespace std;


class Node{
	public:
        	int fq;//частота
                char sym;//символ
     		Node *left, *right;  
    		Node(){
    			left = NULL;
    			right = NULL;
    		} 
 		Node(Node *L, Node *R){ 
 			left =  L;
        	right = R;
            sym = L->sym + R->sym;
                }
};

struct Compare {
	bool operator() (const Node* l, const Node* r){
		return l->fq < r->fq;
	}
};
vector<bool> bool_code;                
map<char,vector<bool> > table;    

void AssocTable(Node *root){
	if (root->left!=NULL){//пока есть левые потомки пишем 0
		bool_code.push_back(0);
		AssocTable(root->left);
	}
	if (root->right!=NULL){//вправо 1
		bool_code.push_back(1);
		AssocTable(root->right);
	}
	if (root->left==NULL && root->right==NULL)//для листьев приписываем код
		table[root->sym] = bool_code;

	bool_code.pop_back();//подняться на уровень

}


int main (){

	//подсчет частот 
	ifstream file ("text.txt", ios::binary);
	map <char,int> letters; //ассоциативный массив символ - частота
	map <char,int>::iterator it;
	char s;
	while (!file.eof()){
		s = file.get();
		letters[s]++;}

	//записываем вершины в лист
	list <Node*> t;
	map <char, int>:: iterator itr;
	for (itr = letters.begin(); itr!=letters.end(); itr++ ){
		Node *p = new Node;
		p->sym = itr->first;//char
		p->fq = itr->second;//int
		t.push_back(p);
	}
	
	while (t.size()!=1)
	{
		t.sort(Compare());//сортировать по частоте
		Node *Left = t.front();//в начале самый редкий
		t.pop_front();//выкинуть
		Node *Right = t.front();//второй редкий
		t.pop_front();
		Node *Mother = new Node(Left, Right);//новый узел с суммой частот
		t.push_back(Mother);
	}
	Node *root = t.front();//в конце останется только узел 

	AssocTable(root);//соответсвия символов и нового кода







    cout<<"ok\n";
return 0;}