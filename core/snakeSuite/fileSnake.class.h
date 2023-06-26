#define FILE_SNAKE_REGULAR 1
#define FILE_SNAKE_DIR 2
#define FILE_SNAKE_CHARDEV 3
#define FILE_SNAKE_BLOCKDEV 4
#define FILE_SNAKE_FIFO 5
#define FILE_SNAKE_SYMLINK 7
#define FILE_SNAKE_SOCKET 8

class FileSnake{
	private:
		int _fd = -1;
		DIR *_dirFd = NULL;
		string openedFile = "";
		size_t _fileSize = -1;
		
		bool _openForRead(string fileName){
			if(!fileExists(fileName))
				return false;
			_closeFile();
			_fd = open(fileName.c_str(), O_RDONLY);
			if(_fd <= 0){
				return false;
			}

			openedFile = fileName;
			return true;
		}

		bool _openForWriteAppend(string fileName){
			_closeFile();
			if(!fileExists(fileName)){
				_fd = open(fileName.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
				if(_fd <= 0)
					return false;
				openedFile = fileName;
				return true;
			}else{
				_fd = open(fileName.c_str(), O_APPEND | O_WRONLY);
				if(_fd <= 0)
					return false;
				openedFile = fileName;
				return true;
			}
		}
		bool _openForWriteTrunc(string fileName){
			_closeFile();
			if(!fileExists(fileName)){
				_fd = open(fileName.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
				if(_fd <= 0)
					return false;
				openedFile = fileName;
				return true;
			}else{
				_fd = open(fileName.c_str(), O_TRUNC | O_WRONLY);
				if(_fd <= 0)
					return false;
				openedFile = fileName;
				return true;
			}
		}

		size_t _readFile(string fileName, char *outBuffer, size_t outSize){
			size_t ret = -1;
			if(!_openForRead(fileName)){
				return -1;
			}
			
			ret = read(_fd, outBuffer, outSize);
			_closeFile();

			return ret;
		}
		size_t _writeFileTrunc(string fileName, char *inBuffer, size_t inSize){
			size_t ret = -1;
			if(!_openForWriteTrunc(fileName)){
				return -1;
			}
			ret = write(_fd, inBuffer, inSize);
			_closeFile();

			return ret;
		}
		size_t _writeFileAppend(string fileName, char *inBuffer, size_t inSize){
			size_t ret = -1;
			if(!_openForWriteAppend(fileName)){
				return -1;
			}
			ret = write(_fd, inBuffer, inSize);
			_closeFile();

			return ret;
		}

		void _closeFile(){
			close(_fd);
			_fd = -1;
		}

		void _openDir(string target){
                        _dirFd = opendir(target.c_str());
                        if(_dirFd == NULL){
                                fprintf(stderr, "[E] Failed to open Dir\n");
                        }
                }

                void _closeDir(void){
                        closedir(_dirFd);
                }
	public:
	size_t recentActivity = -1;
	bool dirExists(string dir){
		struct stat st;
		if(stat(dir.c_str(), &st) != 0)
			return false;
		return true;
	}
	bool makeDir(string dir){
		if(!this->dirExists(dir)){
			if(mkdir(dir.c_str(), 0700))
				return false;
		}
		return true;
	}
	bool fileExists(string file){
		struct stat st;
		if(stat(file.c_str(), &st) != 0)
			return false;
		return true;
	}

	size_t getFileSize(string fileName){
		struct stat st;
		if(stat(fileName.c_str(), &st) != 0)
			return -1;
		_fileSize = st.st_size;
		return (size_t)st.st_size;
	}

	bool writeFileTrunc(string fileName, char *inBuffer, size_t bufferSize){
		recentActivity = _writeFileTrunc(fileName, inBuffer, bufferSize);
		if(recentActivity  != bufferSize)
			return false;
		return true;
	}

	bool writeFileAppend(string fileName, char *inBuffer, size_t bufferSize){
		recentActivity = _writeFileAppend(fileName, inBuffer, bufferSize);
		if(recentActivity != bufferSize)
			return false;
		return true;
	}

	bool removeFile(string fileName){
		if(unlink(fileName.c_str()) != 0)
			return false;
		return true;
	}

	bool readFile(string fileName, char *outBuffer, size_t fileSize){
		if(fileSize <= 0)
			return false;

		memset(outBuffer, 0x00, fileSize);


		recentActivity = _readFile(fileName, outBuffer, fileSize);
		if(recentActivity <= 0)	
			return false;
		
		return true;
	}

	string *listDir(string dirName){
                struct dirent *entry;
                _openDir(dirName.c_str());
                if(_dirFd == NULL){
                        fprintf(stderr, "[E] Failed to open Dir\n");
                        return NULL;
                }
                size_t fileCount = 0;
                while((entry = readdir(_dirFd)) != NULL){
                        string compare = entry->d_name;
                        if(compare == "." || compare == "..")
                                continue;
                        fileCount++;
                }
                _closeDir();

                string *ret = new string[fileCount+1];
                int cnt = 0;

                _openDir(dirName.c_str());
                if(_dirFd == NULL){
                        fprintf(stderr, "[E] Failed to open Dir\n");
                        return NULL;
                }
                while((entry = readdir(_dirFd)) != NULL){
                        string compare = entry->d_name;
                        if(compare == "." || compare == "..")
                                continue;
                        ret[cnt] = entry->d_name;
                        cnt++;
                }
                ret[cnt] = "";
                _closeDir();
                return ret;


        }

	bool perm(string file, mode_t perms){
		if(chmod(file.c_str(), perms) == 0)
			return true;
		else
			return false;
	}

	/*
	 * listDir ignores ./ and ../ so this should be safe, lol.
	 * it should try to delete if it detects a symlink...not sure if that's a good idea or not yet.
	 * */
	bool removeDirRecursive(string targetDir){
		string *files = listDir(targetDir);
		int ctr = 0;
		while(files[ctr] != ""){
			string target = targetDir + "/" + files[ctr];
			if(getFileType(target) == FILE_SNAKE_DIR){
				if(!removeDirRecursive(target)){
					return false;
				}
			}else{
				removeFile(target);
			}
			ctr++;
		}
		return rmdir(targetDir.c_str()) == 0 ? true : false;
	}

	int getFileType(string fileName){
		struct stat st;
		stat(fileName.c_str(), &st);
		if(S_ISREG(st.st_mode))
			return FILE_SNAKE_REGULAR;

		if(S_ISDIR(st.st_mode))
			return FILE_SNAKE_DIR;

		if(S_ISCHR(st.st_mode))
			return FILE_SNAKE_CHARDEV;

		if(S_ISBLK(st.st_mode))
			return FILE_SNAKE_BLOCKDEV;

		if(S_ISFIFO(st.st_mode))
			return FILE_SNAKE_FIFO; 

		if(S_ISLNK(st.st_mode))
			return FILE_SNAKE_SYMLINK;

		if(S_ISSOCK(st.st_mode))
			return FILE_SNAKE_SOCKET;
		return -1;
	}
};
