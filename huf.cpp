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
	int ch;
	cout<<"Выберите действие:\n1)Закодировать файл\n2)Декодировать файл\n";
	cin>>ch;
	if (ch ==1){
	//подсчет частот 
	ifstream file("1text.txt", ios:: out | ios::binary);
	map <char, int> letters; //ассоциативный массив символ - частота
	char s;
	int num_of_sym = 0;
	while (!file.eof()) {
		s = file.get();
		letters[s]++;
		
	}
	map<char, int>::iterator it; 
	////////вывод частот
	cout<<"\nСимвол-частота\n";
	 for (it = letters.begin(); it != letters.end(); it++) {
		//cout << it->first << "!!!(код)" <<(int)it->first<<" : "<<  it->second << endl;}
		cout << it->first<< " : "<<  it->second << endl;}
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
	//	cout<<"Закодированная строка битов\n";
	//	while (!file.eof()){
	//		char c;
	//		c = file.get();
	//		//cout<<c<<" ---- ";
	//		vector <bool> x = table[c];
	//		for (int n = 0; n<x.size(); n++)
	//			cout<<x[n];
	//		//	cout<<endl;
	//		}
	
	
	
	//открыть новый файл для закодированого сообщения	
	ofstream g("output.huff", ios::out | ios::binary);
	int pos = 0; 
	char buf = 0;
	//записать частоты в начало и их количество
	cout<<"ДЛИНА АЛФАВИТА!"<<" - "<<letters.size();
	g<<letters.size();//сначала записываем длину алфавита
	//потом переписываем символ(1байт)-частота(4байта)
	for (it = letters.begin(); it != letters.end(); it++){
		g<<it->first << it->second;}

	//идти по файлу 1 и записывать коды каждого символа в файл 2
	while (!file.eof()){
		char c = file.get();//берем символ
		vector <bool> x = table[c]; //берем его код
		for (int n = 0; n<x.size(); n++){//записываем код, пока байт не заполнится, потом сливаем его в файл
			buf = buf | x[n]<<(7-pos);
			pos++;
			if(pos==8){pos = 0; g<<buf; buf = 0;}
			}
			
		}
	
	file.close();
	g.close();
	}
	//обратное преобразование
	//считываем частоты
	if (ch == 2){
	ifstream G("output.huff", ios::out );//| ios::binary
	int numb, ii = 0, nn; char ss;
	G >> numb;//первый байт - количество cим-в в алф-те
	cout<<" СЧИТАТЬ "<<numb<<endl;
	map <char, int> lettersG; //ассоциативный массив символ - частота
	while (ii != numb){//считать из файла символ-частота 
		ss = G.get();//символ
		G >> nn;//частота
		lettersG[ss] = nn;
		ii++;
		}	
		
	//проверка массива
	cout<<"\nСимвол-частота2!!!!!\n";
	map<char, int>::iterator iq; 
	 for (iq = lettersG.begin(); iq != lettersG.end(); iq++) {
		cout << iq->first << " : " << iq->second << endl;}
	cout<<endl;	
		
	


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

