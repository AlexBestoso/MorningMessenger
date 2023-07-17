#define MORNING_IO_GENERAL 0
#define MORNING_IO_ERROR 1
#define MORNING_IO_FAILURE 1
#define MORNING_IO_SUCCESS 2
#define MORNING_IO_NONE 3
#define MORNING_IO_INPUT 4
#define MORNING_IO_QUESTION 5
class MorningIO{
	private:
		string msgStem_general = "[*] ";
		string msgStem_error = "[E] ";
		string msgStem_success = "[+] ";
		string msgStem_question = "[?] ";
		string msgStem_input = "[INPUT] ";

		string getType(int type){
			if(type == MORNING_IO_GENERAL)
				return msgStem_general;
			else if(type == MORNING_IO_ERROR)
				return msgStem_error;
			else if(type == MORNING_IO_SUCCESS)
				return msgStem_success;
			else if(type == MORNING_IO_NONE)
				return "";
			else if(type == MORNING_IO_INPUT)
				return msgStem_input;
			else if(type == MORNING_IO_QUESTION)
				return msgStem_question;
			else
				return msgStem_general;
		}
	public:
		MorningIO(){}
	
		void outf(int type, const char *fmt, ...){
			// Format buffer size
                        va_list args;
                        va_start(args, fmt);
                        int len = vsnprintf(NULL, 0, fmt, args);
                        va_end(args);
                        if(len<0)return;

                        // Format the message
                        char msg[len + 1];
                        va_start(args, fmt);
                        vsnprintf(msg, len+1, fmt, args);
                        va_end(args);
			
			string msgOut = msg;
			msgOut = getType(type) + msg;
                        printf("%s", msgOut.c_str());
		}

		void out(int type, string msg){
			string msgOut = getType(type) + msg;
			printf("%s", msgOut.c_str());
		}

		string inWithSpace(int type, string msg){
			if(msg != "")
                                out(type, msg);
                        string ret = "";
                        cout.flush();
			cin.ignore();
			getline(cin, ret);
			cin.ignore();
                        return ret;
		}

		string inString(int type, string msg){
			if(msg != "")
				out(type, msg);
			string ret = "";
			cout.flush();
			cin >> ret;
			return ret;
		}
};
