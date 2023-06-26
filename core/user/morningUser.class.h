class MorningUser{
	private:
		string userTableName = "morn_user";
		int sql_id = 0;
		int sql_username = 1;
		int sql_password = 2;
		string tableCols[3] = {
			"user_id",
			"user_username",
			"user_password"
		};
		
		int id = -1;
		string username = "";
		string password = "";
		string salt = "sfggs546bAB$%^ab6$AB46ab6BAR";
		string pepper = "ghfghGRSABT$BZS$ZB$^bz$^";

		SqlSnake sql;
		EncryptionSnake encryption;

		MorningIO io;

		string error = "";

	public:

	sqltable_t buildTable(void){
		sqltable_t ret;
		ret.name = this->userTableName;
		ret.colCount = 3;
		ret.cols = new sqlcolumn_t[3];
		ret.cols[0] = sql.generatePrimaryColumn(tableCols[0], "INT", "NOT NULL AUTO_INCREMENT");
		ret.cols[1] = sql.generateColumn(tableCols[1], "VARCHAR(50)", "NOT NULL");
		ret.cols[2] = sql.generateColumn(tableCols[2], "VARCHAR(64)", "NOT NULL");
		return ret;
	}

	string hashPassword(string pass){
		pass = salt+pass+pepper;
		pass = encryption.sha256(pass, pass.length(), false);
		if(encryption.didFail()){
                        encryption.printError();
                        error = "Failed to hash password.";
                        return "";
                }
		return pass;

	}
	void setSql(SqlSnake sql){
		this->sql = sql;
	}

	string getError(void){
		return error;
	}

	bool newUser(SqlSnake sql, string user, string pass){
		error = "";
		pass = hashPassword(pass);
		if(pass == "")
			return false;

		sqlinsert_t insert;
		insert.table = userTableName;
		insert.count = 2;
		insert.cols = new string[2]{
			tableCols[sql_username],
			tableCols[sql_password]
		};
		insert.values = new string[2]{
			user,
			pass
		};
		if(!sql.secureInsert(insert)){
			error = sql.getError();
			return false;
		}
		sql.close();
		return true;
	}

	bool login(SqlSnake sql, string user, string pass){
		error = "";
                pass = hashPassword(pass);
                if(pass == ""){
			error = "Failed to hash password.";
                        return false;
		}

		sqlselect_t select;
		select.table = userTableName;
		select.colCount = 3;
		select.cols = tableCols;
		select.hasWhere = true;
		select.wheres = sql.addToWhere(select.wheres, sql.generateWhere(
					tableCols[sql_username], "=", user, true), "");
		select.wheres = sql.addToWhere(select.wheres, sql.generateWhere(
					tableCols[sql_password], "=", pass, true), "AND");
		if(!sql.secureSelect(select)){
			error = "Failed to run select statement : " + sql.getError();
			return false;
		}
		sqlresults_t res = sql.getResults();
		if(res.fieldCount != 3){
			error = "SQL failed to find required fields.\n";
			return false;
		}
		if(res.resultCount <= 0){
			error = "Invalid user credentails.\n";
			return false;
		}
		id = atoi(res.results[0].values[sql_id].c_str());
		username = res.results[0].values[sql_username];
		password = res.results[0].values[sql_password];
		sql.close();
		return true;
	}
};
