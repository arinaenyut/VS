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
	for (int i =0 ; i < alphabet.size(); i++)
		if (symbol == alphabet.at(i)) return (i + 1);
}

//процедура переноса найденных битов в строку с кодом
void BitsPlusFollow(bool bit, string& encode_text){
	encode_text.push_back(bit+48);
	for (;bits_to_follow > 0; bits_to_follow-- )
		encode_text.push_back(!bit+48);	
}

string get_message(){
	string text;
	ifstream File("text.txt", ios::in);
	while (!File.eof()){
		string temp;
		getline(File, temp);
		if (!File.eof())
			temp += '\n';
		text += temp;
	}
	File.close();
	return text;
}


string arithmet_encode(string text, int* b, string alphabet){
	int len = text.length();
	unsigned short int* h = new unsigned short int[len];
	unsigned short int* l = new unsigned short int[len];
	h[0] = 65535;
	l[0] = 0;
	unsigned int delitel = b[alphabet.size()];
	unsigned short int First_qtr = (h[0] + 1) / 4;
	unsigned short int Half = First_qtr * 2;
	unsigned short int Third_qtr = First_qtr * 3;
	bits_to_follow = 0;
	string code;
	int i = 0;
	for (i = 0; i < len;){//i - symbol
	//cout<<"символ "<<input.at(i)<<" диапазоны ";
	//cout<<"l "<<l[i]<<" h "<<h[i]<<endl;
		int j = IndexSym(text.at(i), alphabet);//j - index
		i++;
		l[i] = l[i-1] + b[j-1] * (h[i-1] - l[i-1] + 1) / delitel;
		h[i] = l[i-1] + b[j] * (h[i-1] - l[i-1] + 1) / delitel - 1;
		for (;;){		//обрабатываем варианты
			if (h[i] < Half) //переполнения
				BitsPlusFollow(0, code);
			
			else if (l[i] >= Half){
				BitsPlusFollow(1, code);
				l[i] -= Half;
				h[i] -= Half;
			}
			     else if ((l[i] >= First_qtr) && (h[i] < Third_qtr)){
				bits_to_follow++;
				l[i] -= First_qtr;
				h[i] -= First_qtr;
			}
			          else  break;


			l[i] += l[i];
			h[i] += h[i] + 1;
		}

	}
	bits_to_follow = 1;
	if (l[i] < First_qtr) BitsPlusFollow(1, code);
	else BitsPlusFollow(0, code);
	return code;
}

unsigned short int Read16Bit(string code){
	int i;
	unsigned short int value = 0;
	for (i = 0; i < 16; i++){
		if (i == code.size()){
			return value;
		}
		if ('1' == code[i]){
			value = value | (1 << (15 - i));
		}
	}
	return value;
}

unsigned short int ReadBit(char s){
	if (s == '1')
		return 1;
	return 0;
}

string arithmet_decode(string code, string alphabet, int* b){
	unsigned short int* h = new unsigned short int[code.length()];
	unsigned short int* l = new unsigned short int[code.length()];
	l[0] = 0;
	h[0] = 65535;
	unsigned int delitel = b[alphabet.size()];
	unsigned short int First_qtr = (h[0] + 1) / 4;
	unsigned short int Half = 2 * First_qtr;
	unsigned short int Third_qtr = 3 * First_qtr;
	unsigned short int value;
	int idx = 16;
	string decode_text;
	value = Read16Bit(code);
	int d = 0;
	
	for (int i = 1; i < delitel + 1; i++){
		unsigned int freq = ((value - l[i - 1] + 1) * delitel - 1) / (h[i - 1] - l[i - 1] + 1);
		int j;
		for (j = 1; b[j] <= freq; j++);//поиск символа
		l[i] = l[i - 1] + ((h[i - 1] - l[i - 1] + 1) * b[j - 1]) / delitel;
		h[i] = l[i - 1] + ((h[i - 1] - l[i - 1] + 1) * b[j]) / delitel - 1;

		for (;;){//обрабатываем варианты переполения
			if (h[i] < Half)
				;//ничего
			else if (l[i] >= Half){
				value -= Half; l[i] -= Half; h[i] -= Half;
			}
			else if ((l[i] >= First_qtr) && (h[i] < Third_qtr)){
				l[i] -= First_qtr;
				h[i] -= First_qtr;
				value -= First_qtr;
			}
			else break;
			l[i] += l[i];
			h[i] += h[i] + 1;
		
			if (idx < code.length())
				d = ReadBit(code.at(idx++));
			value += value + d;
			}
		
		decode_text.push_back(alphabet.at(j - 1));
		}	
	return decode_text;
}


string atoi(unsigned char a){
	string bitchar = "";
	while (a != 0){
		string bit = a % 2 == 0 ? "0" : "1";
		bitchar += bit;
		a /= 2;
	}
	if (bitchar.size() < 8){
		int k = 8 - bitchar.size();
		for (int i = 0; i < k; i++)	
			bitchar += "0";
	}
	reverse(bitchar.begin(), bitchar.end());
	//cout<<output;
	return bitchar;
}
string string_to_bin(string str, int d){
	string bincode = "";
	for (int i = 0; i < str.size(); i++)
		bincode += atoi(str[i]);
	
	bincode = bincode.substr(0, bincode.size() - d);
	return bincode;

}

int main(){
	int number;
	cout << "1)code\n2)decode\n";
	cin >> number;
	switch (number) {
		case(1)://кодирование
		{
			string text = get_message();
			map<char, int> fq;       // символ-частота
			for (char b : text)
				fq[b]++;
			vector<pair< char, int > > V(fq.begin(), fq.end());
			sort(V.begin(), V.end(), cmp);//сортировка частоты
			
			//вывод
			//for(map<char,int>::iterator itr=fq.begin();itr!=fq.end(); ++itr){
			//	cout<<itr->first <<" : "<< itr->second;
			//	cout<<endl;}	
				
			//массив диапазонов
			int* b = new int[V.size() + 1];
			b[0] = 0;
			int i = 1;
			for (auto p : V){
				b[i] = b[i - 1] + p.second;
				i++;
			}
			//отсортированный алфавит			
			string alphabet = "";
			for (auto p : V){
				char b = p.first;
				alphabet += b;
				}				
			//cout<<"abcd "<<alphabet;
			
			string CODE = arithmet_encode(text, b, alphabet);
			//cout << CODE << endl;	
			
			int k = (CODE.size() % 8 == 0) ? 0 : 8 - CODE.size() % 8;
			
			fstream tab("out.txt", ios::binary | ios::out);
			int SIZE = alphabet.size();
			tab.write((char*)&k, sizeof(char));
			tab.write((char*)&SIZE, sizeof(int));
			for (int i = 0, j = 1; i < SIZE, j < SIZE + 1; i++, j++){
				tab.write((char*)&alphabet[i], sizeof(char));
				tab.write((char*)&b[j], sizeof(int));
			}			
			
			int count = 0;
			char byte = 0;
			for (int i = 0; i < CODE.size(); i++){
				byte = (byte << 1) | (CODE[i] - '0');
				count++;
				if (count == 8){
					count = 0; tab << byte; byte = 0;
				}
			}
			if (count != 0){
				while (count != 8){
					byte = (byte << 1); count++;
				}
				tab << byte;
			}
	
			tab.close();
			break;
		}

		case(2)://декодирование
		{
			
			fstream File("out.txt", ios::binary | ios::in);
			int SIZE = 0;
			int k = 0;
			File.read((char*)&k, sizeof(char));
			File.read((char*)&SIZE, sizeof(int));
			int* fq = new int[SIZE + 1];
			fq[0] = 0;
			char s = 0;
			string alphabet = "";
			for (int i = 1; i < SIZE + 1; i++){
				File.read((char*)&s, sizeof(char));
				alphabet += s;
				File.read((char*)&fq[i], sizeof(int));
			}
			string code = "";
			while (!File.eof()){
				string temp;
				getline(File, temp);
				if (!File.eof())
					temp += '\n';
			code += temp;
			}
			File.close();
			
			string bincode = string_to_bin(code, k);
			string decoded = arithmet_decode(bincode, alphabet, fq);
			
			ofstream file("newfile.txt", ios::out);
			for (int i = 0; i < decoded.size(); i++){
				file << decoded[i]; cout<<decoded[i];}
					
		break;
		}
	}

	return 0;
}
