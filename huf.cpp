#include <iostream>
#include <map>
#include <fstream>
#include <list>
#include <vector>

using namespace std;


class Node {
public:
	int fq;//частота
	char sym;//символ
	Node* left, * right;
	Node(){
		left = NULL;
		right = NULL;
	}
	Node(Node* L, Node* R) {
		left = L;
		right = R;
		sym = L->sym + R->sym;
	}
};

struct Compare {
	bool operator() (const Node* l, const Node* r) const {
		return l->fq < r->fq;
	}
};
vector<bool> bool_code;
map<char, vector<bool> > table;

void AssocTable(Node* root) {
	if (root->left != NULL) {//пока есть левые потомки пишем 0
		bool_code.push_back(0);
		AssocTable(root->left);
	}
	if (root->right != NULL) {//вправо 1
		bool_code.push_back(1);
		AssocTable(root->right);
	}
	if (root->left == NULL && root->right == NULL)//для листьев приписываем код
		table[root->sym] = bool_code;

	bool_code.pop_back();//подняться на уровень

}


int main() {

	//подсчет частот 
	ifstream file("text.txt", ios:: out | ios::binary);
	map <char, int> letters; //ассоциативный массив символ - частота
	char s;
	while (!file.eof()) {
		s = file.get();
		letters[s]++;
	}
	map<char, int>::iterator it; 
	////////вывод частот
	cout<<"\nСимвол-частота\n";
	 for (it = letters.begin(); it != letters.end(); it++) {
		cout << it->first << " : " << it->second << endl;}
	cout<<endl;
	
	//записываем вершины в лист
	list <Node*> t;
	map <char, int>::iterator itr;
	for (itr = letters.begin(); itr != letters.end(); ++itr) {
		Node* p = new Node;
		p->sym = itr->first;//char
		p->fq = itr->second;//int
		t.push_back(p);
	}

	while (t.size() != 1){
		t.sort(Compare());//сортировать по частоте
		
		Node* Left = t.front();//в начале самый редкий
		t.pop_front();//выкинуть
		Node* Right = t.front();//второй редкий
		t.pop_front();
		Node* Mother = new Node(Left, Right);//новый узел с суммой частот
		t.push_back(Mother);
	}
	Node* root = t.front();//в конце останется только узел 

	AssocTable(root);//соответсвия символов и нового кода
	char q;
//переместить указатель в файле в начало
	file.clear();
	file.seekg(0);
//вывести строку битов
/*	cout<<"Закодированная строка битов\n";
	while (!file.eof()){
		char c;
		c = file.get();
		vector <bool> x = table[c];
		for (int n = 0; n<x.size(); n++)
			cout<<x[n];
		}*/
	
	
	
	//открыть новый файл для закодированого сообщения	
	ofstream g("output.txt", ios::out | ios::binary);
	int pos = 0; 
	char buf = 0;
	//записать частоты в начало
	
	for (it = letters.begin(); it != letters.end(); it++){
		g<<it->first << it->second;}

	//идти по файлу 1 и записывать коды каждого символа в файл 2
	while (!file.eof()){
		char c = file.get();//берем символ
		vector <bool> x = table[c];//берем его код
		for (int n = 0; n<x.size(); n++){//записываем код, пока байт не заполнится, потом сливаем его в файл
			buf = buf | x[n]<<(7-pos);
			pos++;
			if(pos==8){pos = 0; g<<buf; buf = 0;}
			}
			
		}
	//outpup содержит в первой части символ-частоту, после - закодированный текст
	file.close();
	g.close();
	//обратное преобразование
	//считываем частоты
	ifstream G("output.txt", ios::out | ios::binary);
	int numb = G.get();//первый байт - количество 
	map <char, int> lettersG; //ассоциативный массив символ - частота
	//char s;
	//int num_of_sym = 0;
	//while (!file.eof()) {
	//	s = file.get();
	//	letters[s]++;}
	for (int k = 0; k < numb; k++){
	//сначала символ потом код, записать в мап
		}
	
	//составление дерева 





	
	return 0;
}

