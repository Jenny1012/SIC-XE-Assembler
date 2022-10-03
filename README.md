# SIC-XE-Assembler  
系統程式document 	10827245 桑怡蓁  
1、	開發平台: Windows 10  
2、	使用開發環境: DEV-C++  
3、	使用程式語言: C++  
4、	所選擇的組合語言: SIC、SIC/XE  
5、	未完成的功能:不知道自己未完成的功能有哪些，但是老師的input檔案是沒有問題的，所以應該是沒有吧(單引號若只有一個會無窮迴圈)  
6、	程式設計:  
   
使用者說明: 選單列會先顯示一行"請選擇離開或執行(請輸入0或1) -> "  的字樣，輸入1後，再輸入所需檔名即可。  
  
檔名為SIC (數字或其他直接接上即可，不需要空白，我有接上_input.txt，所以只需要前面那段)，以下舉例使用者輸入，輸出檔名即為SIC(接上的字串或其他東西)_output.txt  
  
＃ ex. 請輸入要讀取的檔案名稱( ex.SIC ): -> SIC (此為使用者輸入)  
   Input檔名:SIC_input.txt  
	  Output檔名:SIC_output.txt  
＃ ex. 請輸入要讀取的檔案名稱( ex.SIC ): -> SICXE (此為使用者輸入)  
Input檔名:SICXE_input.txt  
	  Output檔名:SICXE_output.txt  
  
  
   Functions (參數省略) :   
	SIC/XE:  
		string StrToHex( string temp );   // DEC->HEX  
		string IntToHex( string temp, int mode );    
		void assembler( vector<tokenType> list, string middlename );  // assembler  
		void TranslateMachineCode( vector<tokenType> &list, vector<optType> optab, vector<tableType> &symtab, bool pass2 );    
		void GetOptab( vector<optType> &optab, optType &content, string line );  
		string ToUpperCase( string temp ) ;  // 轉大寫  
		int CheckOptab( int now, string temp, vector<optType> optab );  
		void Format1( vector<tokenType> &list, int listnum, int now, int oploc, int &count, bool pass2 );  
		void Format2( vector<tokenType> &list, int listnum, int now, int oploc, int &count, bool pass2 );  
		void Format34( vector<tokenType> &list, int listnum, int now, int oploc, int &count, bool pass2 );  
		string HexToBin( string temp );  
		string NIXBPE( vector<tokenType> &list, int listnum, int oploc );  
		string BinToHex( string temp );  
		void DecToHex( vector<int> locctr10, vector<string> &locctr );  
		void PutInSymTab( vector<tableType> &symtab, vector<tokenType> &list, int listnum, int now );  
		void Pesudo( vector<tokenType> &list, int listnum, int now, int &count, bool pass2 );  
		int StrToInt( string temp );  
		void PrintResult( vector<tokenType> list, vector<int> line, string middlename );  
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
		bool CheckFormat2( vector<tokenType> list, int listnum, int now, int oploc );  
		vector<optType> optab;  
		vector<tableType> symtab;    // 變數、符號的位址   
		vector<int> locctr10;  
		vector<int> line;  
		vector<string> locctr;    // 正在處理指令的位址   
		int base = 0;  
	SIC :  
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

程式說明 :   

(1)	將切好的token去做判斷是屬於指令、pesudo code、還是註解  
(2)	判斷個別的類語法有沒有錯(syntax)，若有錯則不翻machine code並跳過此行程式  
(3)	分好類之後(SIC/XE)若是指令則看他是format1、2、3、4，再翻machine code，(SIC)則看有沒有X register，再翻machine code。  
(4)	將symbol存好之後，若有forward reference則做pass2  
(5)	pass2 將為算好位址、距離的label做machine code計算  
(6)	輸出翻好的檔案  
(7)	結束程式  
