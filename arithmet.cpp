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

int IndexSym(char symbol, string alphabet){
	int i = 0;
	for (; i < alphabet.size(); i++)
		if (symbol == alphabet.at(i)) return (i + 1);
        }

//процедура переноса найденных битов в строку с кодом
void BitsPlusFollow(bool bit, string& encode_text){
	encode_text.push_back(bit+48);
	for (;bits_to_follow > 0; bits_to_follow-- )
		encode_text.push_back(!bit+48);		
}


int main(){
	int ch;
	cout<<"1)code\n2)decode\n";
	cin>>ch;
if (ch == 1){
	string text = "kob.ko";
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
	
	//изменение границ
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
	
	string encode_text;
	for (i = 0; i < len;){//i - symbol
	cout<<"\nl "<<l[i]<<" h "<<h[i]<<endl;
		int j = IndexSym(text.at(i), alphabet);//j - index
		i++;
		l[i] = l[i-1] + b[j-1] * (h[i-1] - l[i-1] + 1) / delitel;
		h[i] = l[i-1] + b[j] * (h[i-1] - l[i-1] + 1)/ delitel - 1;
		for (;;){		  //обрабатываем варианты
			if (h[i] < Half){ //переполнения
				BitsPlusFollow(0, encode_text);
			}
			else if (l[i] >= Half){
				BitsPlusFollow(1, encode_text);
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
		
	}
	bits_to_follow = 1;
	if (l[i] < First_qtr) BitsPlusFollow(1, encode_text);
	else BitsPlusFollow(0, encode_text);
	//cout<<encode_text;
	fstream G("output.txt", ios::binary | ios::out);
	string result = "";
	int pos = 0;
	unsigned char byte = 0;
	for (int i = 0; i < encode_text.size(); i++){
		byte = (byte << 1) | (encode_text[i] - '0');
		pos++;
		if (pos == 8){
			pos = 0;
			result += byte;
			byte = 0;
		}
	}
	if (pos != 0){
		while (pos != 8){
			byte = (byte << 1);
			pos++;
		}
		result += byte;
	}		
	int size = alphabet.size();
	G.write((char*)&size, sizeof(int));
	for(int i = 0, j = 1; i < size, j < size + 1; i++, j++){
		G.write((char*)&alphabet[i], sizeof(char));
		G.write((char*)&fq[j], sizeof(int));
	}
	for (int i = 0; i < result.size(); i++)
		G << result[i];	
	}
	
if(ch == 2){
	
	
	
	}

			
	
	


return 0;
}