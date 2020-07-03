#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <algorithm>
using namespace std;

bool cmp(const pair<char, int>& A, const pair<char, int>& B){//функция сравнения
	return A.second > B.second;
}

int main(){

	string text = "abcaaaabbbcccccaa";
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
	
			
	
	


return 0;
}