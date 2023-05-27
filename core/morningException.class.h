class MorningException : public exception{
	private:
		string errorMessage = "";
	public:
		MorningException(string msg){
			errorMessage = msg;
		}
		MorningException(const char *fmt, ...){
			// Format buffer size
			va_list args;
			va_start(args, fmt);
			int len = vsnprintf(NULL, 0, fmt, args);
			va_end(args);
			if(len<0){
				errorMessage = fmt;
			}

			// Format the message
			char msg[len + 1];
			va_start(args, fmt);
			vsnprintf(msg, len+1, fmt, args);
			va_end(args);

			errorMessage = msg;
		}
		MorningException(){}

		void setError(string err){
			errorMessage = err;
		}
		virtual const char *what() const throw(){
			return errorMessage.c_str();
		}
};
