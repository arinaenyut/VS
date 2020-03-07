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
	char s;
	while (!file.eof()){
		s = file.get();
		letters[s]++;}
		
cout<<"hello   \n";	
    


return 0;}