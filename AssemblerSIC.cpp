// Assembler  SIC
// 10827245 桑怡蓁 

#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <bitset>
#include <iomanip>
#include <conio.h>
#include <complex>
#include <math.h>
#include <algorithm>


using namespace std;

typedef struct cT {
	vector<string> tokens;
	vector<string> changecase;
	vector<string> tokensloc;
	string rawR;
	string mcode;
}tokenType;

typedef struct opt {
	string instruction;
	string format;
	string opcode;
	string notes;
	string m;
	string n;
	string r1;
	string r2;
}optType;

typedef struct tab {
	string name;
	int address;
	int firstappr;
}tableType;


class CutToken {
	public :
		void GetToken( string middlename, char* file, vector<tokenType> &list );  
		void RemoveSpace( tokenType &content, vector<string> &table5, vector<string> &table6, vector<string> &table7  );
		void CheckToken( tokenType &content, vector<string> &table5, vector<string> &table6, vector<string> &table7  );
		bool FindTable1( int now, tokenType &content );  // 1-4
		void PutInToTable( int now, int tablenum, tokenType &content, vector<string> &table5, vector<string> &table6, vector<string> &table7 );
		void PutInToTable5( int now, tokenType &content, vector<string> &table5 );
		void PutInToTable6( int now, tokenType &content, vector<string> &table6 );
		void PutInToTable7( int now, tokenType &content, vector<string> &table7 );
		void ChangeCase( int now, tokenType &content );
		void ToUpperCase( int now, tokenType &content );
		void ToLowerCase( int now , tokenType &content );
		void SpecialToken( int now, tokenType &content, vector<string> &tokens );
		void PrintResult( string middlename, vector<tokenType> list, vector<string> &table5, vector<string> &table6, vector<string> &table7  );
};

class Assembler {
	public :
		string StrToHex( string temp );
		string IntToHex( string temp, int mode );
		void assembler( vector<tokenType> list, string middlename );
		void TranslateMachineCode( vector<tokenType> &list, vector<optType> optab, vector<tableType> &symtab );
		void GetOptab( vector<optType> &optab, optType &content, string line );
		string ToUpperCase( string temp ) ;
		int CheckOptab( int now, string temp, vector<optType> optab );
		void Format1( vector<tokenType> &list, int listnum, int now, int oploc, int &count );
		void Format2( vector<tokenType> &list, int listnum, int now, int oploc, int &count );
		void Format34( vector<tokenType> &list, int listnum, int now, int oploc, int &count );
		string HexToBin( string temp );
		string NIXBPE( vector<tokenType> &list, int listnum, int oploc );
		string BinToHex( string temp );
		void DecToHex( vector<int> locctr10, vector<string> &locctr );
		void PutInSymTab( vector<tableType> &symtab, vector<tokenType> &list, int listnum, int now );
		void Pesudo( vector<tokenType> &list, int listnum, int now, int &count );
		int StrToInt( string temp );
		void PrintResult( vector<tokenType> list, vector<int> line, string middlename );
		void CheckError( vector<tokenType> list, int listnum, int now, string type );
		bool CheckReg( vector<tokenType> list, int listnum, int now );
		bool CheckLabel( vector<tokenType> list, int listnum, int now );
		bool CheckNum( vector<tokenType> list, int listnum, int now );
		bool CheckString( vector<tokenType> list, int listnum, int now );
		bool CheckM( vector<tokenType> list, int listnum, int now );
		bool CheckImm( vector<tokenType> list, int listnum, int now );
		bool CheckDirect( vector<tokenType> list, int listnum, int now );
		bool CheckIndirect( vector<tokenType> list, int listnum, int now );
		bool CheckIndex( vector<tokenType> list, int listnum, int now );
		bool CheckLiteral( vector<tokenType> list, int listnum, int now );
		bool CheckFormat34( vector<tokenType> list, int listnum, int now, int oploc );
		vector<optType> optab;
		vector<tableType> symtab;    // 變數、符號的位址 
		vector<int> locctr10;
		vector<int> line;
		vector<string> locctr;    // 正在處理指令的位址 
		int base = 0;
};

CutToken TList;
Assembler AList;

void CutToken :: RemoveSpace( tokenType &content, vector<string> &table5, vector<string> &table6, vector<string> &table7  ) {
	stringstream ss;
	string temptoken;
	string temptoken2;
	string temptokenloc;
	
	ss << content.rawR;
	while ( !ss.eof() ) {
		ss >> temptoken;
		
		if ( temptoken != temptoken2 ) {
			content.tokens.push_back( temptoken );
			content.changecase.push_back( temptoken );
			content.tokensloc.push_back( temptokenloc );
		} // if

		temptoken2 = temptoken;
	} // while

	CheckToken( content, table5, table6, table7 );
} // RemoveSpace()

void CutToken :: ChangeCase( int now, tokenType &content ) {
	string temptoken;
	if ( content.changecase[now][0] >= 'A' && content.changecase[now][0] <= 'Z' ) {
		ToUpperCase( now, content );
	} // if
	else if ( content.changecase[now][0] >= 'a' && content.changecase[now][0] <= 'z' ) {
		ToLowerCase( now, content ); 
	} // else if
} // ChangeCase()

void CutToken :: ToUpperCase( int now, tokenType &content ) {
	for ( int i = 0 ; i < content.changecase[now].size() ; i++ ) {
		content.changecase[now][i] = tolower( content.changecase[now][i] );
	} // for
} // ToTupperCase()

void CutToken :: ToLowerCase( int now, tokenType &content ) {
	for ( int i = 0 ; i < content.changecase[now].size() ; i++ ) {
		content.changecase[now][i] = toupper( content.changecase[now][i] );
	} // for 
} // ToLowerCase()

void CutToken :: SpecialToken( int now, tokenType &content, vector<string> &tokens ) {
	string temptoken;
	string temp;
	stringstream ss;
	ifstream infile;
	string line, loc;
	vector<string> table4;
	string input = "Table4.table";
	char* filename = (char*)input.c_str();
	infile.open(filename);
	bool found = false;
	bool end = false;
	
	while ( getline(infile, line) ) {
		table4.push_back(line);
	} // while
	infile.close();
	
	for ( int i = 0 ; i < content.tokens[now].size() ; i++ ) {
		for( int j = 0 ; j < table4.size() ; j++ ) {
			temptoken = content.tokens[now][i];
			if ( temptoken == table4[j] ) {  // ex. @RETADR   A,  A,B
				if ( i==0 ) {
					tokens.push_back(table4[j]);
					temp = "";
					found = true;
				} // if
				else if ( temp != "" && (i!=0) ) {
					tokens.push_back(temp);
					tokens.push_back(table4[j]);
					temp = "";
					found = true;
				} // else if
			} // if
		} // for
		
		if ( found == false ) {
			temp = temp + content.tokens[now][i];
		} // if

		found = false;
	} // for
	
	if ( temp != "" ) {
		tokens.push_back(temp);
	} // if
	
	temp = "";
	
	table4.clear();
} // SpecialToken()

void CutToken :: CheckToken( tokenType &content, vector<string> &table5, vector<string> &table6, vector<string> &table7  ) {
	string temptoken;
	string temptokenloc;
	bool check = false;
	bool found1 = false;
	bool found2 = false;
	bool end = false;
	
	for ( int i = 0 ; i < content.tokens.size() && end == false; i++ ) {
		for ( int j = 0 ; j < content.tokens[i].size() && check == false ; j++ ) {
			if ( content.tokens[i][j] == 'C' && content.tokens[i][j+1] == '\'' ) {  // 7 string
				j = j + 2;
				int count = 0;
				bool breakout = false;
				bool space = false;
				int tempnow, temp;
				for ( int t = 0 ; t < content.rawR.size() && breakout == false ; t++ ) {
					if ( content.rawR[t] == '\'' ) {
						tempnow = t+1;
						breakout = true;
					} // if
				} // for
				breakout = false;

				for ( int t = tempnow ; t < content.rawR.size() && breakout == false ; t++ ) {
					if ( content.rawR[t] != '\'' ) {
						temptoken = temptoken + content.rawR[t];
					} // if
					else {
						temp = t;
						breakout = true;
					} // else
					
					if ( content.rawR[t] == ' ' || content.rawR[t] == '\t' ) {
						space = true;
					} // if
				} // for
				
				if ( temp == content.rawR.size()-1 ) {
					if ( space == true ) {
						content.tokens.erase(content.tokens.begin()+(i+1),content.tokens.begin()+(i+2));
						content.changecase.erase( content.changecase.begin()+(i+1),content.changecase.begin()+(i+2) );
					} // if
					
					content.tokens[i] = "\'"; 
					content.changecase[i] = "\'";
					content.tokens.insert( content.tokens.begin() + (i+1), temptoken );
					content.tokens.insert( content.tokens.begin() + (i+2), "\'" );
					content.changecase.insert( content.changecase.begin() + (i+1), temptoken );
					content.changecase.insert( content.changecase.begin() + (i+2), "\'" );
					content.tokensloc.insert( content.tokensloc.begin() + (i+1), "" );  // token
					content.tokensloc.insert( content.tokensloc.begin() + (i+2), "" );  // '
				
					FindTable1( i, content );
					PutInToTable( i+1, 7, content, table5, table6, table7 );
					FindTable1( i+2, content );
					end = true;
					check = true;
				} // if
				else {
					string temptoken2;
					for ( int t = temp+1 ; t < content.rawR.size() ; t++ ) {
						if ( content.rawR[t] != ' ' && content.rawR[t] != '\t' ) {
							temptoken2 = temptoken2 + content.rawR[t];
						} // if
					} // for
					
					if ( space == true ) {
						content.tokens.erase(content.tokens.begin()+(i+1),content.tokens.begin()+(i+2));
						content.changecase.erase( content.changecase.begin()+(i+1),content.changecase.begin()+(i+2) );
					} // if
					
					content.tokens[i] = "\'";  // temp == content.rawR.size()-1
					content.changecase[i] = "\'";
					content.tokens.insert( content.tokens.begin() + (i+1), temptoken );
					content.tokens.insert( content.tokens.begin() + (i+2), "\'" );
					content.changecase.insert( content.changecase.begin() + (i+1), temptoken );
					content.changecase.insert( content.changecase.begin() + (i+2), "\'" );
					content.tokensloc.insert( content.tokensloc.begin() + (i+1), "" );  // token
					content.tokensloc.insert( content.tokensloc.begin() + (i+2), "" );  // '
					FindTable1( i, content );
					PutInToTable( i+1, 7, content, table5, table6, table7 );
					FindTable1( i+2, content );
					
					content.tokens.insert( content.tokens.begin() + (i+3), temptoken2 );  // token
					content.changecase.insert( content.changecase.begin()+(i+3),temptoken2 );
					content.tokensloc.insert( content.tokensloc.begin() + (i+3), "" );  // '
					i = i + 2;
					check = true;
				} // else
			} // if
			else if ( content.tokens[i][j] == 'X' && content.tokens[i][j+1] == '\'' ) {  // 6  integer/real number
				j = j + 2;
				int count = 0;
				bool breakout = false;
				bool space = false;
				int tempnow, temp;
				for ( int t = 0 ; t < content.rawR.size() && breakout == false ; t++ ) {
					if ( content.rawR[t] == '\'' ) {
						tempnow = t+1;
						breakout = true;
					} // if
				} // for
				breakout = false;

				for ( int t = tempnow ; t < content.rawR.size() && breakout == false ; t++ ) {
					if ( content.rawR[t] != '\'' ) {
						temptoken = temptoken + content.rawR[t];
					} // if
					else {
						temp = t;
						breakout = true;
					} // else
					
					if ( content.rawR[t] == ' ' || content.rawR[t] == '\t' ) {
						space = true;
					} // if
				} // for
				
				if ( temp == content.rawR.size()-1 ) {
					if ( space == true ) {
						content.tokens.erase(content.tokens.begin()+(i+1),content.tokens.begin()+(i+2));
					} // if
					
					content.tokens[i] = "\'"; 
					content.changecase[i] = "\'";
					content.tokens.insert( content.tokens.begin() + (i+1), temptoken );
					content.tokens.insert( content.tokens.begin() + (i+2), "\'" );
					content.changecase.insert( content.changecase.begin() + (i+1), temptoken );
					content.changecase.insert( content.changecase.begin() + (i+2), "\'" );
					content.tokensloc.insert( content.tokensloc.begin() + (i+1), "" );  // token
					content.tokensloc.insert( content.tokensloc.begin() + (i+2), "" );  // '
				
					FindTable1( i, content );
					PutInToTable( i+1, 6, content, table5, table6, table7 );
					FindTable1( i+2, content );
					end = true;
					check = true;
				} // if
				else {
					string temptoken2;
					for ( int t = temp+1 ; t < content.rawR.size() ; t++ ) {
						if ( content.rawR[t] != ' ' && content.rawR[t] != '\t' ) {
							temptoken2 = temptoken2 + content.rawR[t];
						} // if
					} // for
					
					if ( space == true ) {
						content.tokens.erase(content.tokens.begin()+(i+1),content.tokens.begin()+(i+2));
					} // if
					
					content.tokens[i] = "\'";  // temp == content.rawR.size()-1
					content.changecase[i] = "\'";
					content.tokens.insert( content.tokens.begin() + (i+1), temptoken );
					content.tokens.insert( content.tokens.begin() + (i+2), "\'" );
					content.changecase.insert( content.changecase.begin() + (i+1), temptoken );
					content.changecase.insert( content.changecase.begin() + (i+2), "\'" );
					content.tokensloc.insert( content.tokensloc.begin() + (i+1), "" );  // token
					content.tokensloc.insert( content.tokensloc.begin() + (i+2), "" );  // '
					FindTable1( i, content );
					PutInToTable( i+1, 6, content, table5, table6, table7 );
					FindTable1( i+2, content );
					
					content.tokens.insert( content.tokens.begin() + (i+3), temptoken2 );  // token
					content.changecase.insert( content.changecase.begin()+(i+3), temptoken2 );
					content.tokensloc.insert( content.tokensloc.begin() + (i+3), "" );  // '
					i = i + 2;
					check = true;
				} // else
			} // else if
			else if ( content.tokens[i][j] >= '0' && content.tokens[i][j] <= '9' ) {  // integer
				PutInToTable( i, 6, content, table5, table6, table7 );
				check = true;
			} // else if
			else if ( content.tokens[i][j] == '.' ) {   // 7  註解 (後面直接讀掉)
				j++;
				int lefttoken = content.tokens.size() - i - 1;
				if ( lefttoken != 0 ) {  // 後面剩的token 
					if ( content.tokens[i][j] == '\0' ) {   // 註解後有空白 
						FindTable1( i, content );
					} // if
					else {                                  // 註解後沒有空白 
						while ( content.tokens[i][j] != '\0' ) {
							temptoken = temptoken + content.tokens[i][j];
							j++;
						} // while
				
						content.tokens[i] = ".";
						content.tokens.insert( content.tokens.begin() + (i+1), temptoken );
						content.changecase.insert( content.changecase.begin()+(i+1), temptoken );
						content.tokensloc.insert( content.tokensloc.begin() + (i+1), "" );
						FindTable1( i, content );
						i++;
					} // else
				} // if
				else {
					while ( content.tokens[i][j] != '\0' ) {
						temptoken = temptoken + content.tokens[i][j];
						j++;
					} // while
				
					content.tokens[i] = ".";
					content.tokens.insert( content.tokens.begin() + (i+1), temptoken );
					content.changecase.insert( content.changecase.begin()+(i+1), temptoken );
					content.tokensloc.insert( content.tokensloc.begin() + (i+1), "" );
					FindTable1( i, content );
					i++;
				} // else
				
				check = true;
				end = true;
			} // else if
			else { 
				vector<string> tokens;
				SpecialToken( i, content, tokens );

				if ( tokens.size() == 1 ) {
					found1 = FindTable1( i, content );
					
					if ( (content.tokens[i][0] == 'A' || content.tokens[i][0] == 'a') && content.tokens[i].size() == 1 ) {
						content.tokens[i] = "A ";
						content.tokensloc[i] = "(3,1)";
						found1 = true;
					} // if
					else if ( content.tokens[i][0] == '-' && content.tokens[i].size() == 1 ) {
						content.tokens[i] = "- ";
						content.tokensloc[i] = "(4,3)";
						found1 = true;
					} // else if
					
					if ( found1 == false ) {
						ChangeCase( i, content );
						found1 = FindTable1( i, content );
						ChangeCase( i, content );
						found2 = FindTable1( i, content );
						
						if ( found1 == false && found2  == false ) {
							PutInToTable( i, 5, content, table5, table6, table7 );
						} // if
					} // if
				} // if
				else {
					int insert = i+1;
					content.tokens[i] = tokens[0] ;
					content.changecase[i] = tokens[0];
					if ( (content.tokens[i][0] == 'A' || content.tokens[i][0] == 'a') && content.tokens[i].size() == 1 ) {
						content.tokens[i] = "A ";
						content.tokensloc[i] = "(3,1)";
						found1 = true;
					} // if
					else if ( content.tokens[i][0] == '-' && content.tokens[i].size() == 1 ) {
						content.tokens[i] = "- ";
						content.tokensloc[i] = "(4,3)";
						found1 = true;
					} // else if
					
					for ( int t = 1 ; t < tokens.size() ; t++ ) {
						content.tokens.insert( content.tokens.begin() + insert, tokens[t] );
						content.changecase.insert( content.changecase.begin() + insert, tokens[t] );
						content.tokensloc.insert( content.tokensloc.begin() + insert, "" );
						insert++;
					} // for
					
					for ( int now = i; now < content.tokens.size() ; now++ ) {
						found1 = FindTable1( now, content );
						if ( found1 == false ) {
							ChangeCase( now, content );
							found1 = FindTable1( now, content );
							ChangeCase( now, content );
							found2 = FindTable1( now, content );
						
							if ( found1 == false && found2 == false ) {
								PutInToTable( now, 5, content, table5, table6, table7 );
							} // if
						} // if
					} // for
				} // else
				
				tokens.clear();
				check = true;
			} // else 
		} // for
		check = false;
	} // for
} // CheckToken()

void CutToken :: GetToken( string middlename, char* file, vector<tokenType> &list ) {   // 切token 
	string line;
	ifstream infile ; // 讀檔案
	infile.open( file ) ;
	vector<string> table5;
	vector<string> table6;
	vector<string> table7;
	
	for ( int i = 0 ; i < 100 ; i++ ) {
		string space = " ";
		table5.push_back(space);
		table6.push_back(space);
		table7.push_back(space);
	} // for
	
	while ( getline( infile , line ) ) { 
		tokenType content;
		content.rawR = line ;
		RemoveSpace(content, table5, table6, table7);
		list.push_back( content ) ;  // put the new item at the end of the heap
	} // while()
	
	PrintResult( middlename, list, table5, table6, table7 );
	infile.close();
	//list.clear();
} // GetToken()

bool CutToken :: FindTable1( int now, tokenType &content ) {
	stringstream ss;
	stringstream loc;
	bool found = false;
	ifstream infile;
	string line;
	vector<string> table1;
	vector<string> table2;
	vector<string> table3;
	vector<string> table4;
	
	for ( int i = 1 ; i <= 4 ; i++ ) {
		ss << i;
		string num = ss.str();
		string input = "Table" + num + ".table";
		char* filename = (char*)input.c_str();
		infile.open(filename);
		ss.str("");
		
		while ( getline( infile, line ) ) {
			if ( i == 1 ) {
				table1.push_back(line);
			} // if
			else if ( i == 2 ) {
				table2.push_back(line);
			} // else if 
			else if ( i == 3 ) {
				table3.push_back(line);
			} // else if
			else {
				table4.push_back(line);
			} // else
		} // while
		
		infile.close();
	} // for
	
	for ( int j = 0 ; j < table1.size() ; j++ ) {
		if ( content.tokens[now] == table1[j] || content.changecase[now] == table1[j] ) {
			loc << j+1;
			string num2 = loc.str();
			content.tokensloc[now] = "(1," + num2 + ")";
			found = true;
			loc.str("");
		} // if
	} // for
	
	for ( int j = 0 ; j < table2.size() ; j++ ) {
		if ( content.tokens[now] == table2[j] || content.changecase[now] == table2[j] ) {
			loc << j+1;
			string num2 = loc.str();
			content.tokensloc[now] = "(2," + num2 + ")";
			found = true;
			loc.str("");
		} // if
	} // for
	
	for ( int j = 0 ; j < table3.size() ; j++ ) {
		if ( content.tokens[now] == table3[j] || content.changecase[now] == table3[j] ) {
			loc << j+1;
			string num2 = loc.str();
			content.tokensloc[now] = "(3," + num2 + ")";
			found = true;
			loc.str("");
		} // if
	} // for
	
	for ( int j = 0 ; j < table4.size() ; j++ ) {
		if ( content.tokens[now] == table4[j] || content.changecase[now] == table4[j] ) {
			loc << j+1;
			string num2 = loc.str();
			content.tokensloc[now] = "(4," + num2 + ")";
			found = true;
			loc.str("");
		} // if
	} // for
	
	
	if ( found == false ) {
		return false;
	} // if
	else {
		return true;
	} // else
	
	table1.clear();
	table2.clear();
	table3.clear();
	table4.clear();

} // FindTable1()

void CutToken :: PutInToTable5( int now, tokenType &content, vector<string> &table5 ) {
	stringstream ss;
	stringstream location;
	string line;
	bool found = false;
	bool same = false;
	int ascii = 0;
	int place = 0;
	
	for ( int i = 0 ; i < content.tokens[now].size() ; i++ ) {
		ascii = (int)content.tokens[now][i] + ascii;
	} // for

	ascii = ascii % 100; 
	
	for ( int i = ascii ; i< 100 && found == false ; i++ ) {
		
		
		if ( table5[i] != " " && table5[i] != content.tokens[now] ) {
			for ( int j = i ; table5[j] != " " && j < 100 && same == false ; j++ ) {
				if ( j == 99 ) {
					j = 0;
				} // if
				else if ( table5[j] == content.tokens[now] ) { 
					location << j;
					string num2 = location.str();
					content.tokensloc[now] = "(5," + num2 + ")";
					same = true;
					location.str("");
				} // else if
				
				place = j;
			} // for
				
			if ( same == false ) { 
				location << (place+1);
				string num2 = location.str();
				content.tokensloc[now] = "(5," + num2 + ")";
				found = true;
				location.str("");
				table5[place+1] = content.tokens[now];
			} // if
		} // if
		else if ( table5[i] == " " ) { 
			table5[i] = content.tokens[now];
			location << i;
			string num2 = location.str();
			content.tokensloc[now] = "(5," + num2 + ")";
			found = true; 
			location.str("");
		} // else if
		else if ( table5[i] == content.tokens[now] ) {    // 萬一有相同的字串 
			location << i;
			string num2 = location.str();
			content.tokensloc[now] = "(5," + num2 + ")";
			found = true; 
			location.str("");
		} // else if
		else if ( i == 99 && found == false && table5[i] != " " ) {  // 到底了但還沒找到 
			i = 0;
		} // else if
	} // for
} // PutInToTable()

void CutToken :: PutInToTable6( int now, tokenType &content, vector<string> &table6 ) {
	stringstream ss;
	stringstream location;
	string line;
	bool found = false;
	bool same = false;
	int ascii = 0;
	int place = 0;
	
	for ( int i = 0 ; i < content.tokens[now].size() ; i++ ) {
		ascii = (int)content.tokens[now][i] + ascii;
	} // for

	ascii = ascii % 100; 
	
	for ( int i = ascii ; i< 100 && found == false ; i++ ) {
		if ( table6[i] != " " && table6[i] != content.tokens[now] ) {
			for ( int j = i ; table6[j] != " " && j < 100 && same == false ; j++ ) {
				if ( j == 99 ) {
					j = 0;
				} // if
				else if ( table6[j] == content.tokens[now] ) {
					location << j;
					string num2 = location.str();
					content.tokensloc[now] = "(6," + num2 + ")";
					same = true;
					location.str("");
				} // else if
				
				place = j;
			} // for
				
			if ( same == false ) {
				location << (place+1);
				string num2 = location.str();
				content.tokensloc[now] = "(6," + num2 + ")";
				found = true;
				location.str("");
				table6[place+1] = content.tokens[now];
			} // if
		} // if
		else if ( table6[i] == " " ) { 
			table6[i] = content.tokens[now];
			location << i;
			string num2 = location.str();
			content.tokensloc[now] = "(6," + num2 + ")";
			found = true; 
			location.str("");
		} // else if
		else if ( table6[i] == content.tokens[now] ) {    // 萬一有相同的字串 
			location << i;
			string num2 = location.str();
			content.tokensloc[now] = "(6," + num2 + ")";
			found = true; 
			location.str("");
		} // else if
		else if ( i == 99 && found == false && table6[i] != " " ) {  // 到底了但還沒找到 
			i = 0;
		} // else if
	} // for
} // PutInToTable()

void CutToken :: PutInToTable7( int now, tokenType &content, vector<string> &table7 ) {

	stringstream ss;
	stringstream location;
	string line;
	bool found = false;
	bool same = false;
	int ascii = 0;
	int place = 0;
	
	for ( int i = 0 ; i < content.tokens[now].size() ; i++ ) {
		ascii = (int)content.tokens[now][i] + ascii;
	} // for

	ascii = ascii % 100; 
	
	for ( int i = ascii ; i< 100 && found == false ; i++ ) {
		if ( table7[i] != " " && table7[i] != content.tokens[now] ) {
			for ( int j = i ; table7[j] != " " && j < 100 && same == false ; j++ ) {
				if ( j == 99 ) {
					j = 0;
				} // if
				else if ( table7[j] == content.tokens[now] ) {
					location << j;
					string num2 = location.str();
					content.tokensloc[now] = "(7," + num2 + ")";
					same = true;
					location.str("");
				} // else if
				
				place = j;
			} // for
				
			if ( same == false ) {
				location << (place+1);
				string num2 = location.str();
				content.tokensloc[now] = "(7," + num2 + ")";
				found = true;
				location.str("");
				table7[place+1] = content.tokens[now];
			} // if
		} // if
		else if ( table7[i] == " " ) { 
			table7[i] = content.tokens[now];
			location << i;
			string num2 = location.str();
			content.tokensloc[now] = "(7," + num2 + ")";
			found = true; 
			location.str("");
		} // else if
		else if ( table7[i] == content.tokens[now] ) {    // 萬一有相同的字串 
			location << i;
			string num2 = location.str();
			content.tokensloc[now] = "(7," + num2 + ")";
			found = true; 
			location.str("");
		} // else if
		else if ( i == 99 && found == false && table7[i] != " " ) {  // 到底了但還沒找到 
			i = 0;
		} // else if
	} // for
} // PutInToTable()

void CutToken :: PutInToTable( int now, int tablenum, tokenType &content, vector<string> &table5, vector<string> &table6, vector<string> &table7 ) {  // 5-7
	if ( tablenum == 5 ) {
		PutInToTable5( now, content, table5 );
	} // if
	else if ( tablenum == 6 ) {
		PutInToTable6( now, content, table6 );
	} // else if
	else if ( tablenum == 7 ) {
		PutInToTable7( now, content, table7 );
	} // else if
} // PutInToTable()

void CutToken :: PrintResult( string middlename, vector<tokenType> list, vector<string> &table5, vector<string> &table6, vector<string> &table7  ) {
	fstream outfile;
	fstream outfile5;
	fstream outfile6;
	fstream outfile7;
	
	string file5 = "Table5.table";
	string file6 = "Table6.table";
	string file7 = "Table7.table";
	char* filename5 = (char*)file5.c_str();
	char* filename6 = (char*)file6.c_str();
	char* filename7 = (char*)file7.c_str();
	
	string file = middlename + "_output.txt";
	char* filename = (char*)file.c_str();
	outfile.open( filename, ios::out );
	for ( int i = 0 ; i < list.size() ; i++ ) {
		outfile << list[i].rawR << endl;
		for ( int j = 0 ; j < list[i].tokens.size() ; j++ ) {
			outfile << list[i].tokensloc[j] ;
		} // for
		
		outfile << endl;
	} // for
	
	outfile5.open( filename5, ios::out );
	for ( int i = 0 ; i < 100 ; i++ ) {
		outfile5 << table5[i] << endl;
	} // for
	
	outfile6.open( filename6, ios::out );
	for ( int i = 0 ; i < 100 ; i++ ) {
		outfile6 << table6[i] << endl;
	} // for
	
	outfile7.open( filename7, ios::out );
	for ( int i = 0 ; i < 100 ; i++ ) {
		outfile7 << table7[i] << endl;
	} // for
	
	list.clear();
	outfile.close();
	outfile5.close();
	outfile6.close();
	outfile7.close();
} // PrintResult()

/**********************************************    Assembler    ************************************************/

void Assembler :: GetOptab( vector<optType> &optab, optType &content, string line ) {  // 存optab 
	int count = 1;
	stringstream ss;
	string temptoken;
	ss << line;
	
	while ( !ss.eof() ) {
		ss >> temptoken;
		if ( count == 1 ) {
			content.instruction = temptoken;
		} // if
		else if ( count == 2 ) {
			content.opcode = temptoken;
		} // else if
		else if ( count == 3 ) {
			content.format = temptoken;
		} // else if
		else if ( count == 4 ) {
			content.notes = temptoken;
		} // else if
		else if ( count == 5 ) {
			content.m = temptoken;
		} // else if
		else if ( count == 6 ) {
			content.n = temptoken;
		} // else if
		else if ( count == 7 ) {
			content.r1 = temptoken;
		} // else if
		else if ( count == 8 ) {
			content.r2 = temptoken;
		} //else if
		
		count++;
	} // while
} // GetOptab()

void Assembler :: assembler( vector<tokenType> list, string middlename ) {   
	string input = "OPTAB.txt";
	stringstream ss;
	string temptoken;
	optType content;
	tableType tabcontent;
	tabcontent.name = "";
	char* filename = (char*)input.c_str();
    string Line;
    vector<string> table1;
	vector<string> table2;
	vector<string> table3;
	vector<string> table4;
	
    ifstream infile ; // 讀檔案
	infile.open( filename ) ;
	
	for ( int i = 0 ; i < 100 ; i++ ) {
		symtab.push_back( tabcontent );
	} // for
	
	while ( getline( infile, Line ) ) {
		GetOptab( optab, content, Line );
		optab.push_back( content );
	} // while
	
	TranslateMachineCode( list, optab, symtab );
	TranslateMachineCode( list, optab, symtab );
	
	DecToHex( locctr10, locctr );
	PrintResult( list, line, middlename );
	
	list.clear();
	optab.clear();
	table1.clear();
	table2.clear();
	table3.clear();
	table4.clear();
	symtab.clear();
	locctr10.clear();
	locctr.clear();
	line.clear();
} // assembler()

void Assembler :: PutInSymTab( vector<tableType> &symtab, vector<tokenType> &list, int listnum, int now ) {
	int ascii = 0;
	bool found = false;
	bool exist = false;
	
	for ( int i = 0 ; i < 100 ; i++ ) {
		if ( list[listnum].tokens[now] == symtab[i].name ) {
			exist = true;
		} // if
	} // for
	
	for ( int i = 0 ; i < list[listnum].tokens[now].size() ; i++ ) {
		ascii = (int)list[listnum].tokens[now][i] + ascii;
	} // for

	ascii = ascii % 100;
	
	for ( int i = 0 ; i < 100 && found == false ; i++ ) {
		if ( symtab[i].name.size() == 0 ) {    // 空的 
			if ( now == 0 && exist == false ) {             // 宣告label 
				symtab[i].name = list[listnum].tokens[now];
				symtab[i].address = locctr10[listnum];
			} // if
			else {                        // 呼叫label(但還沒定義) 
				symtab[i].name = list[listnum].tokens[now];
				symtab[i].address = -1;
				symtab[i].firstappr = listnum;  //第幾行 
			} // else
				
			found = true;
		} // if
		else if ( symtab[i].name.size() != 0 ) {
			;
		} // else if
		else if ( i == 99 && found == false && symtab[i].name.size() != 0 ) {
			i = 0;
		} // else
	} // for
} // PutInSymTab()

void Assembler :: TranslateMachineCode( vector<tokenType> &list, vector<optType> optab, vector<tableType> &symtab ) {
	bool found = false;
	int count = 0;
	int space = -1;
	int linenum = 0;
	
	for ( int i = 0 ; i < list.size() ; i++ ) {
		for ( int j = 0 ; j < list[i].tokensloc.size() && found == false ; j++ ) {
			if ( list[i].tokensloc[j][1] == '1' ) {  // instruction 
				int oploc = CheckOptab( j, list[i].tokens[j], optab );
				if ( oploc != -1 ) {
					if ( optab[oploc].format != "1" && optab[oploc].format != "2" ) {       // format3、4 
						if ( CheckFormat34( list, i, j, oploc ) ) {
							Format34( list, i, j, oploc, count );
						} // if
						else {
							list[i].mcode = "ERROR";
							locctr10.push_back( space );
						} // else
					} // if
				} // if
				
				found = true;
			} // if
			else if ( list[i].tokensloc[j][1] == '2' ) {  // pesudo
				Pesudo( list, i, j, count );
				found = true;
			} // else if
			else if ( list[i].tokens[j] == "." ) {   // comment
				locctr10.push_back( space );
				found = true;
			} // else if
		} // for
		
		if ( found == false ) {
			locctr10.push_back( space );
			line.push_back( 0 );
		} // if
		else {
			linenum = linenum + 5;
			line.push_back( linenum );
		} // else
		found = false;
	} // for
} // TranslateMachineCode()

void Assembler :: DecToHex( vector<int> locctr10, vector<string> &locctr ) {
	string Hex, temp;
	char buffer[33];
	int dec ;
	string space;
	
	for ( int i = 0 ; i < locctr10.size() ; i++ ) {
		dec = locctr10[i];
		if ( dec != -1 ) {   // -1 註解  end 
			itoa(dec, buffer, 16);
			Hex = buffer;
			Hex = ToUpperCase(Hex);
			while ( Hex.size() != 4 ) {
				Hex.insert( 0,"0" );
			} // while
			locctr.push_back( Hex );
		} // if
		else {
			locctr.push_back( space );
		} // else
	} // for
} // DecToHex

void Assembler :: Pesudo( vector<tokenType> &list, int listnum, int now, int &count ) {
	int num;
	bool found = false;
	bool exist = false;
	
	if ( list[listnum].tokensloc[now] == "(2,7)" ) { 			// EQU
		if ( now == 1 && CheckLabel(list, listnum, 0) && (CheckLabel(list, listnum, 2)||CheckNum(list, listnum, 2)) ) {
			locctr10.push_back(count);
			count = count + 3;
		} // if
		else {
			locctr10.push_back( -1 );
			list[listnum].mcode = "ERROR";
		} // else 
	} // if
	else if ( list[listnum].tokensloc[now] == "(2,2)") {  // END
		if ( ( now == 1 && CheckLabel(list, listnum, 0) && CheckLabel(list, listnum, 2) ) || 
			( now == 1 && CheckLabel(list, listnum, 0) ) || ( now == 0 && CheckLabel(list, listnum, 1) ) || 
			( now == 0 ) ) {
			locctr10.push_back( -1 );	
		} // if
		else {
			locctr10.push_back( -1 );
			list[listnum].mcode = "ERROR";
		} // else 
	} // else if
	else if ( list[listnum].tokensloc[now] == "(2,5)" ) {   // RESB
		if ( ( now == 1 && CheckLabel(list, listnum, 0) && CheckNum(list, listnum, 2) ) ||
			( now == 0 && CheckNum(list, listnum, 1) ) ) {
				locctr10.push_back(count);
				num = StrToInt( list[listnum].tokens[now+1] );
				count = count + num;
		} // if
		else {
			locctr10.push_back( -1 );
			list[listnum].mcode = "ERROR";
		} // else
	} // else if
	else if ( list[listnum].tokensloc[now] == "(2,6)" ) {   // RESW
		if ( ( now == 1 && CheckLabel(list, listnum, 0) && CheckNum(list, listnum, 2) ) ||
			( now == 0 && CheckNum(list, listnum, 1) ) ) {
				locctr10.push_back(count);
				num = StrToInt( list[listnum].tokens[now+1] );
				count = count + num*3;
		} // if
		else {
			locctr10.push_back( -1 );
			list[listnum].mcode = "ERROR";
		} // else
	} // else if
	else if ( list[listnum].tokensloc[now] == "(2,1)" ) {   // START
		if ( ( now == 1 && CheckLabel(list,listnum,0) && CheckNum(list, listnum,2) ) ||
			( now == 0 && CheckNum(list,listnum,1) ) ) {
			stringstream ss;
			ss << hex << list[listnum].tokens[now+1] ;
			ss >> count;
			locctr10.push_back(count);
		} // if
		else {
			locctr10.push_back( -1 );
			list[listnum].mcode = "ERROR";
		} // else
	} // else if
	else if ( list[listnum].tokensloc[now] == "(2,3)" ) {  // BYTE
		if ( (CheckLabel(list, listnum, 0) && now == 1 && list[listnum].tokensloc[2] == "(4,9)" && ( CheckString(list,listnum,3) || CheckNum(list,listnum,3))) || 
				(now == 0 && ( CheckString(list,listnum,2) || CheckNum(list,listnum,2)) && list[listnum].tokensloc[1] == "(4,9)") ||
				(CheckLabel(list, listnum, 0) && now == 1 && CheckNum(list,listnum,2)) || 
				(now == 0 && CheckNum(list,listnum,1)) ) {
			locctr10.push_back(count);
			if ( list[listnum].tokensloc[now+1] == "(4,9)" ) {
				if ( list[listnum].tokensloc[now+2][1] == '6' ) {  // 數字 
					list[listnum].mcode = list[listnum].tokens[now+2];
					while ( list[listnum].mcode.size() != list[listnum].tokens[now+2].size() ) {
						list[listnum].mcode.insert(0,"0");
					} // while
				} // if
				else {
					list[listnum].mcode = StrToHex( list[listnum].tokens[now+2] );    // string
				} // else
			} // if
			
			int length = list[listnum].mcode.size();
			length = length / 2;
			count = count + length;
		} // if
		else {
			locctr10.push_back( -1 );
			list[listnum].mcode = "ERROR";
		} // else 
	} // else if
	else if ( list[listnum].tokensloc[now] == "(2,4)" ) {   // WORD
		if ( (CheckLabel(list, listnum, 0) && now == 1 && list[listnum].tokensloc[2] == "(4,9)" && ( CheckString(list,listnum,3) || CheckNum(list,listnum,3))) || 
				(now == 0 && ( CheckString(list,listnum,2) || CheckNum(list,listnum,2)) && list[listnum].tokensloc[1] == "(4,9)") ||
				(CheckLabel(list, listnum, 0) && now == 1 && CheckNum(list,listnum,2)) || 
				(now == 0 && CheckNum(list,listnum,1))  ) {
			locctr10.push_back(count);
			if ( list[listnum].tokensloc[now+1] == "(4,9)" ) {
				if ( list[listnum].tokensloc[now+2][1] == '6' ) {  // 數字 
					list[listnum].mcode = list[listnum].tokens[now+2];
					while ( list[listnum].mcode.size() != list[listnum].tokens[now+2].size() ) {
						list[listnum].mcode.insert(0,"0");
					} // while
				} // if
				else {
					list[listnum].mcode = StrToHex( list[listnum].tokens[now+2] );    // string
				} // else
			} // if
			else { 			 // 直接接數字 
				stringstream ss;
				char buffer[33];
				int dec = atoi(list[listnum].tokens[now+1].c_str());
				itoa( dec, buffer, 16 );
				list[listnum].mcode = buffer;
			} // else
			
			while ( list[listnum].mcode.size() != 6 ) {
				list[listnum].mcode.insert( 0, "0" );
			} // while
			count = count + 3;
		} // if
		else {
			locctr10.push_back( -1 );
			list[listnum].mcode = "ERROR";
		} // else
	} // else if
	else if ( list[listnum].tokensloc[now] == "(2,8)" ) {  // BASE
		if ( (now == 1 && CheckLabel(list,listnum,0) && (CheckNum(list,listnum,2)||CheckLabel(list,listnum,2))) ||
			( now == 0 && (CheckNum(list,listnum,1)||CheckLabel(list,listnum,1)) )) {
				locctr10.push_back(count);
			if ( list[listnum].tokensloc[now+1][1] == '6' ) {  // 數字 
				num = StrToInt( list[listnum].tokens[now+1] );
				base = num;
			} // if
			else if ( list[listnum].tokensloc[now+1][1] == '5' ) {
				for ( int s = 0 ; s < 100 ; s++ ) {
					if ( list[listnum].tokens[now+1] == symtab[s].name ) {
						base = symtab[s].address;
					} // if
				} // for
			} // else if
		} // if
		else {
			locctr10.push_back( -1 );
			list[listnum].mcode = "ERROR";
		} // else
	} // else if
	
	if ( list[listnum].mcode != "ERROR" ) {
		for ( int i = 0 ; i < list[listnum].tokensloc.size() ; i++ ) {
			for ( int j = 0 ; j < 100 ; j++ ) {
				if ( symtab[j].name == list[listnum].tokens[i] ) {
					exist = true;
				} 
			} // for
		
			if ( list[listnum].tokensloc[i][1] == '5' && exist == false && (i==0) ) {  // label
	
				PutInSymTab( symtab, list, listnum, i );
			} // if
		} // for
	} // if
} // Pesudo()

bool Assembler :: CheckReg( vector<tokenType> list, int listnum, int now ) {
	if ( list[listnum].tokensloc[now][1] == '3' ) {
		return true;
	} // if
	
	return false;
} // CheckReg()

bool Assembler :: CheckLabel( vector<tokenType> list, int listnum, int now ) {
	if ( list[listnum].tokensloc[now][1] == '5' ) {
		return true;
	} // if
	
	return false;
} // CheckLabel()

bool Assembler :: CheckNum( vector<tokenType> list, int listnum, int now ) {
	if ( list[listnum].tokensloc[now][1] == '6' ) {
		return true;
	} // if
	
	return false;
} // CheckNum()

bool Assembler :: CheckString( vector<tokenType> list, int listnum, int now ) {
	if ( list[listnum].tokensloc[now][1] == '7' ) {
		return true;
	} // if
	
	return false;
} // CheckString()

bool Assembler :: CheckM( vector<tokenType> list, int listnum, int now ) {
	if ( CheckImm(list, listnum, now) || CheckDirect(list, listnum, now) || CheckIndirect(list, listnum, now) || 
		CheckIndex(list, listnum, now) || CheckLiteral(list, listnum, now) ) {
			return true;
	} // if
	
	return false;
} // CheckM()

bool Assembler :: CheckImm( vector<tokenType> list, int listnum, int now ) {
	for ( int i = now ; i < list[listnum].tokens.size() ; i++ ) {
		if ( list[listnum].tokensloc[i] == "(4,12)" && CheckLabel(list,listnum, i+1) ) {
			return true;
		} // if
		else if ( list[listnum].tokensloc[i] == "(4,12)" && CheckNum(list,listnum,i+1) ) {
			return true;
		} // else if
	} // for
	
	return false;
} // CheckImm()

bool Assembler :: CheckDirect( vector<tokenType> list, int listnum, int now ) {
	for ( int i = now ; i < list[listnum].tokens.size() ; i++ ) {
		if ( CheckLabel(list,listnum, i) || CheckNum(list,listnum,i) ) {
			return true;
		} // if
	} // for
	
	return false;
} // CheckDirect()

bool Assembler :: CheckIndirect( vector<tokenType> list, int listnum, int now ) {
	for ( int i = now ; i < list[listnum].tokens.size() ; i++ ) {
		if ( list[listnum].tokensloc[i] == "(4,13)" && CheckLabel(list, listnum, i+1) ) {
			return true;
		} // if
	} // for
	
	return false;
} // CheckIndirect()

bool Assembler :: CheckIndex( vector<tokenType> list, int listnum, int now ) {
	for ( int i = now ; i < list[listnum].tokens.size() ; i++ ) {
		if ( CheckLabel(list, listnum,i) && list[listnum].tokensloc[i]=="(4,13)" && list[listnum].tokensloc[i] == "(3,2)" ) {
			return true;
		} // if
	} // for
	return false;
} // CheckIndex()

bool Assembler :: CheckLiteral( vector<tokenType> list, int listnum, int now ) {
	for ( int i = now ; i < list[listnum].tokens.size() ; i++ ) {
		if ( list[listnum].tokensloc[i] == "(4,9)" && CheckString(list,listnum,i+1) ) {
			return true;
		} // if
	} // for
	
	return false;
} // CheckLiteral()

bool Assembler :: CheckFormat34( vector<tokenType> list, int listnum, int now, int oploc ) {
	if ( optab[oploc].m == "m" ) {
		if ( (now==1 && CheckLabel(list,listnum,0) && CheckM(list,listnum,2)) || (now==0&&CheckM(list,listnum,1)) ) {  // format3
			return true;
		} // if
		else if ( ( now==2 && list[listnum].tokensloc[1]=="(4,2)" && CheckLabel(list,listnum,0)&& CheckM(list,listnum,3)) ||
				(now==1 && list[listnum].tokensloc[0]=="(4,2)" &&CheckM(list,listnum,2) ) ) {
			return true;
		} // else if
	} // if
	else {
		if ( list[listnum].tokens.size() == 1 ) {
			return true;
		} // if
		else {
			return false;
		} // else
	}
	
	return false;
} // CheckFormat34()

void Assembler :: Format34( vector<tokenType> &list, int listnum, int now, int oploc, int &count ) {
	locctr10.push_back( count );
	list[listnum].mcode = optab[oploc].opcode;
	string op6bit, nixbpe, ans, buffer2, buffer3, straddr;

	bool litnum = false;
	char buffer[10];
	bool exist = false;
	bool labelfound = false;
	int symloc = 0;
	bool regfound = false;
	int addr = 0;
	
	for ( int i = 0 ; i < list[listnum].tokens.size() ; i++ ) {
		if ( list[listnum].tokensloc[i][1] == '5' ) {
			labelfound = true;
		} // if
		
		if ( list[listnum].tokensloc[i] == "(3,2)" ) {   // X
			regfound = true;
		} // if
	} // for
	
	for ( int i = 0 ; i < list[listnum].tokens.size() ; i++ ) {
		if ( labelfound == true ) {
			for ( int j = 0 ; j < 100 ; j++ ) {
				if ( list[listnum].tokens[i] == symtab[j].name ) {
					exist = true;
					symloc = j;
				} // if
			} // for
			
			if ( exist == false && list[listnum].tokensloc[i][1] == '5' && (i==0) ) {   // 第一次出現的label 
				PutInSymTab( symtab, list, listnum, i );
			} // if
			else if ( exist == true ) {  // format3 format4
				addr = symtab[symloc].address;
			} // else if
		} // if
		
		/*if ( list[listnum].tokensloc[i][1] == '6' )  {    // 不是label 
			straddr = IntToHex( list[listnum].tokens[i], 0 );
			litnum = true;
		} // if*/
	} // for 
	
	if ( regfound == true ) {  // X = 1
		addr = addr+32768;
	} // if
	
	itoa( addr, buffer, 16 );
	buffer2 = buffer;
	if ( buffer2.size() > 4 ) {
		buffer3 = buffer2.substr( buffer2.size()-4 ,buffer2.size()-1 );
	} // if
	else {
		while ( buffer2.size() != 4 ) {
			buffer2.insert( 0,"0" );
		} // while
		buffer3 = buffer2;
	} // else
	count = count + 3;
	list[listnum].mcode = list[listnum].mcode + buffer3;
	list[listnum].mcode = ToUpperCase( list[listnum].mcode );
} // Format34()

int Assembler :: CheckOptab( int now, string temp, vector<optType> optab ) {  
	bool found = false;
	for ( int i = 0 ; i < optab.size() ; i++ ) {
		if ( temp == optab[i].instruction ) {
			return i;
		} // if
	} // for
	
	if ( found == false ) {    // 第一次沒找到，轉大寫 
		for ( int i = 0 ; i < optab.size() ; i++ ) {
			string token = ToUpperCase(optab[i].instruction);
			if ( temp == token ) {
				return i;
			} // if
		} // for
	} // if
	
	return -1;
} // CheckOptab()

string Assembler :: StrToHex( string temp ) {   // 轉16進位 (table7)   C'EOF'   
    // 先轉ascii(dec),再轉hex
    vector<int> ascii;
    string hex;
    char buffer [33];
    
	for ( int i = 0 ; i < temp.size() ; i++ ) {
		ascii.push_back( (int)temp[i] );
	} // for
	
	for ( int i = 0 ; i < ascii.size(); i++ ) {
		string str;
		sprintf( buffer, "%x", ascii[i] );
		hex = hex + buffer;
	} // for
	
	hex = ToUpperCase( hex );
	return hex;
	ascii.clear();
} // StrToHex() 

int Assembler :: StrToInt( string temp ) {    
	int total = temp[0]-'0';
	for ( int i = 1 ; i < temp.size() ; i++ ) {
		int decimal = temp[i]-'0';
		total = total*10 + decimal;
	} // for
	
	return total;
} // StrToInt()

string Assembler :: IntToHex( string temp, int mode ) {    // (table6)   comp 3  
	char buffer [33];
	int total = temp[0]-'0';
	for ( int i = 1 ; i < temp.size() ; i++ ) {
		if ( mode == 0 ) {
			int decimal = temp[i]-'0';
			total = total*10 + decimal;
		} // if
		else if ( mode == 1 ) {
			int decimal = temp[i]-'1';
			total = total*10 + decimal;
		} // else if
	} // for
	
	sprintf( buffer, "%x", total );
	return buffer;
} // IntToHex()

string Assembler :: HexToBin( string temp ) {      // format3、4  
	 stringstream ss, s1;
	 int dec = 0;
	 string total;
	 char temp2;
	 string temp3;
	 ss << hex << temp;
	 ss >> dec;
	 
	 while ( dec != 1 ) {
	 	s1 << dec%2;
	 	s1 >> temp2;
	 	total.push_back( temp2 );
	 	dec = dec / 2;
	 	s1.clear();
	 } // while
	 
	 s1 << dec;
	 s1 >> temp2;
	 total.push_back( temp2 );
	 
	 while ( total.size() != 8 ) {
	 	total.push_back( '0' );
	 } // while
	
	 for ( int i = total.size()-1 ; i > 1; i-- ) {
	 	temp3 = temp3+total[i] ;
	 } // for

	 return temp3;
} // HexToBin()


string Assembler :: BinToHex( string temp ) {   
	stringstream ss;
	string Hex;
	int dec;
	char buffer[33];
	bitset<32> mybit(temp);
	dec = (int)(mybit.to_ulong());
	itoa(dec, buffer, 16);
	Hex = buffer;
	return Hex;
} // BinToHex()

string Assembler :: ToUpperCase( string temp ) {   // 把optab instruction 轉大寫  
	string str = temp;
	for ( int i = 0 ; i < temp.size() ; i++ ) {
		str[i] = toupper(temp[i]);
	} // for
	
	return str;
} // ToUpperCase()

void Assembler :: PrintResult( vector<tokenType> list, vector<int> line, string middlename ) {    // ok
	fstream outfile;
	bool end = false;
	string file = middlename + "_output.txt";
	char* filename = (char*)file.c_str();
	outfile.open( filename, ios::out );
	outfile << setw(5) << "Line" << setw(10) << "Location" << setw(15) << "Source Code" << setw(25) << "Object Code" << endl;
	outfile << setw(5) << "----" << setw(10) << "--------" << setw(15) << "-----------" << setw(25) << "-----------" << endl;
	for ( int i = 0 ; i < list.size() ; i++ ) {
		for ( int j = 0 ; j < list[i].tokens.size() ; j++ ) {
			if ( list[i].tokensloc[j] == "(2,2)" ) { 
				end = true;
			} // if
		} // for
		
		if ( list[i].mcode == "ERROR" ) {
			outfile << "	" << line[i] << "	" << locctr[i] << "		" << list[i].rawR << "		" << list[i].mcode << endl;
		} // if
		else {
			if ( end == false && line[i] != 0 ) {
				outfile << "	" << line[i] << "	" << locctr[i] << "		" << list[i].rawR << "		" << list[i].mcode << endl;
			} // if
			else if ( line[i] != 0 ) {
				outfile << "	" << line[i] << "	" << locctr[i] << "		" << list[i].rawR << endl;
			} // else 
		} // else
	} // for
	
	list.clear();
	outfile.close();
} // PrintResult()

int main() {
	int num = 0;
	string middlename;
	string input;
	vector<tokenType> list;
	
	cout << "請選擇離開或執行(請輸入0或1) -> ";
	while ( scanf( "%d", &num ) != 1 ){
	   	while( getchar() != '\n' ) ; // 讀掉緩衝區的字元
	   		cout << "輸入格式錯誤!!!請重新輸入" << endl;
			cout << "請選擇離開或執行(請輸入0或1) -> ";
    } // while
    
	while ( num != 0 ) {
    	if ( num == 1 ) {
    		cout << "請輸入要讀取的檔案名稱( ex.SICXE ): -> " ;
    		string name;
    		cin >> middlename;
			input = middlename + "_input.txt";
			char* filename = (char*)input.c_str();
       		
       		ifstream infile ; // 讀檔案
			infile.open( filename ) ;
			while ( !infile ) { // 無此檔案
				if ( strcmp ( name.c_str(), "0") == 0  )
					break  ;
				else {
					cout << "查無此檔案!!!!請重新輸入!!!!"<< endl ;
					cout << "請輸入要讀取的檔案名稱( ex.SICXE ): -> ";
					
					cin >> middlename;
					input = middlename + "_input.txt";
					filename = (char*)input.c_str();
					infile.open( filename );
				} // else
   			} // while
       		
       		TList.GetToken( middlename, filename, list );
       		AList.assembler( list, middlename );
       		cout << "執行結束 !!! " << endl; 
       		list.clear();
		} // if
		
		cout << "請選擇離開或執行(請輸入0或1) -> ";
		cin >> num;
	} // while
}


