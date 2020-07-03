#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <algorithm>

int bits_to_follow;//сколько битов сбрасывать
using namespace std;

bool cmp(const pair<char, int>& A, const pair<char, int>& B){//функция сравнения
	return A.second > B.second;
}

int IndexSym(char symbol, string alphabet){//return index
	int i = 0;
	for (; i < alphabet.size(); i++)
	
		if (symbol == alphabet.at(i)) return (i + 1);
        }

//процедура переноса найденных битов - вывод
void BitsPlusFollow(int bit){
	cout<<bit;
	for (; bits_to_follow > 0; bits_to_follow--)
		cout<<(!bit);
	
}


int main(){

	string text = "abcaaaabbbcccccaaa";
	map<char, int> fq;	//символ-частота
	for (char b : text)
		fq[b]++;

	//вывод ассоц массива
	for(map<char,int>::iterator itr=fq.begin();itr!=fq.end(); ++itr){
		cout<<itr->first <<" : "<< itr->second;
		cout<<endl;}	

	//сортировка частоты
	vector< pair< char, int >> V (fq.begin(), fq.end());
	sort(V.begin(), V.end(), cmp);
	
    //массив диапазонов 
	int* b = new int[V.size() + 1];
	b[0] = 0;
	int i = 1;
	for (auto p : V){
		b[i] = b[i - 1] + p.second;
		i++;
	}
	//отсортированный алфавит
	string alphabet;
	for (auto p : V){
		char b = p.first;
		alphabet += b;
	}
	cout<<"ABC "<<alphabet<<endl;
	
	bits_to_follow = 0;
   	int len = text.length();
	unsigned short int* h = new unsigned short int[len];
	unsigned short int* l = new unsigned short int[len];
	h[0] = 65535;
	l[0] = 0;
	unsigned int delitel = b[alphabet.size()];
	unsigned short int First_qtr = (h[0] + 1) / 4;
	unsigned short int Half = First_qtr * 2;
	unsigned short int Third_qtr = First_qtr * 3;
	
	for (i = 0; i < len;){//i - symbol
		int j = IndexSym(text.at(i), alphabet);//j - index
		i++;
		l[i] = l[i-1] + b[j-1] * (h[i-1] - l[i-1] + 1) / delitel;
		h[i] = l[i-1] + b[j] * (h[i-1] - l[i-1] + 1)/ delitel - 1;
		for (;;){		  //обрабатываем варианты
			if (h[i] < Half){ //переполнения
				BitsPlusFollow(0);
			}
			else if (l[i] >= Half){
				BitsPlusFollow(1);
				l[i] -= Half;
				h[i] -= Half;
			}
			    else if ((l[i] >= First_qtr) && (h[i] < Third_qtr)){
			    	bits_to_follow++;
				    h[i] -= First_qtr;
			}
			        else  break;
			    l[i] += l[i];
			    h[i] += h[i] + 1;	
		}
		//cout<<"l "<<l[i]<<" h "<<h[i]<<endl;
	}
	bits_to_follow = 1;
	if (l[i] < First_qtr) BitsPlusFollow(1);
	else BitsPlusFollow(0);

			
	
	


return 0;
}