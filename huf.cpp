#include <iostream>
#include <map>
#include <fstream>

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


int main (){
	ifstream file ("text.txt", ios::binary);
	map <char,int> letters;
	map <char,int>::iterator it;
	char s;
	while (!file.eof()){
		s = file.get();
		letters[s]++;}

//	for (it=letters.begin(); it!=letters.end(); it++)// вывод символ-частота
//	cout << it->first << ' ' << it->second << '\n';
//new
    		
    


return 0;}