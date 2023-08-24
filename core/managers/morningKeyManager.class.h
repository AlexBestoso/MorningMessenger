struct morning_key_record{
	int id = -1;
	bool trusted = false;
	string alias = "";
	string serverAlias = "";
	string cbip = "";
	string cbhost = "";
	int cbport = -1;
	string date = "";
	string pubkey = "";
	string justification = "";
};
typedef struct morning_key_record friendkey_t;

struct morning_key_file{
	int id = 0;
	string publickey = "";
	string alias = "";
	string message = "";
};
typedef struct morning_key_file keyfile;

class MorningKeyManager{
	private:
		string fileName = "";
		string publicKey = "";
		string alias = "";
		string message = "";

		string tableName = "morn_friends";
		string colNames[10] = {
			"id",
			"trusted",
			"alias",
			"server_alias",
			"cbip",
			"cbhost",
			"cbport",
			"date",
			"pubkey",
			"justification"
		};
		size_t colCount = 10;
		int sql_id = 0;
                int sql_trusted = 1;
                int sql_alias = 2;
                int sql_server_alias = 3;
		int sql_cbip = 4;
                int sql_cbhost = 5;
                int sql_cbport = 6;
                int sql_date = 7;
                int sql_key = 8;
		int sql_justification = 9;

		
		EncryptionSnake encryptionSnake;
		XmlSnake xmlSnake;
		FileSnake fileSnake;
		SqlSnake sqlSnake;

		MorningConfig config;
		MorningIO io;
	public:
	
	size_t recentAllocSize = 0;

	int untrustedKeyCount(void){
		int ret = 0;
		string q = "SELECT COUNT(*) FROM "+tableName+" WHERE "+colNames[sql_trusted]+"=0;";
		sqlSnake = config.getSql();
		if(!sqlSnake.newQuery(q))
			throw MorningException("Failed to get key count : %s", sqlSnake.getError().c_str());

		sqlresults_t res = sqlSnake.getResults();
		if(res.resultCount <= 0)
			throw MorningException("Failed to get key count : No sql results.\n");
		
		if(res.fieldCount <= 0)
			throw MorningException("Failed to get key count : No Sql Fields.\n");

		ret = atoi(res.results[0].values[0].c_str());
		return ret;
	}

	int trustedKeyCount(void){
                int ret = 0;
                string q = "SELECT COUNT(*) FROM "+tableName+" WHERE "+colNames[sql_trusted]+"=1;";
                sqlSnake = config.getSql();
                if(!sqlSnake.newQuery(q))
                        throw MorningException("Failed to get key count : %s", sqlSnake.getError().c_str());

                sqlresults_t res = sqlSnake.getResults();
                if(res.resultCount <= 0)
                        throw MorningException("Failed to get key count : No sql results.\n");

                if(res.fieldCount <= 0)
                        throw MorningException("Failed to get key count : No Sql Fields.\n");

                ret = atoi(res.results[0].values[0].c_str());
                return ret;
        }

	bool denyUntrustedKey(int id){
		sqlSnake = config.getSql();
		sqlwherelist_t wheres;
		wheres = sqlSnake.addToWhere(
				wheres, 
				sqlSnake.generateWhere(colNames[sql_id], "=", to_string(id), true), 
				""
		);
		if(!sqlSnake.secureDelete(tableName, wheres)){
			return false;
		}		
		return true;
	}

	bool deleteTrustedKey(int id){
		return denyUntrustedKey(id);
	}

	bool isKeyTrusted(string pubkey){
		sqlSnake = config.getSql();
		sqlselect_t select;
		select.table = tableName;
		select.colCount = 1;
		select.cols = new string[1]{
			colNames[sql_trusted]
		};
		select.hasWhere = true;
		select.wheres = sqlSnake.addToWhere(
			select.wheres,
			sqlSnake.generateWhere(colNames[sql_key], "=", pubkey, true),
			""
		);
		if(!sqlSnake.secureSelect(select))
			throw MorningException("Failed to check if key is trusted : %s", sqlSnake.getError().c_str());

		sqlresults_t res = sqlSnake.getResults();
		if(res.fieldCount <= 0)
			throw MorningException("Failed to check if key is trusted : No fields in result.");

		if(res.resultCount <= 0){
			return false;
		}

		int trusted = atoi(res.results[0].values[0].c_str());
		if(trusted == 1)
			return true;

		return false;
	}

	bool approveUntrustedKey(int id){
		sqlSnake = config.getSql();
		sqlupdate_t update;
		update.table = tableName;
		update.valueCount = 1;
		update.cols = new string[1]{
			colNames[sql_trusted]
		};
		update.values = new string[1]{
			"1"
		};
		update.wheres = sqlSnake.addToWhere(
			update.wheres,
			sqlSnake.generateWhere(colNames[sql_id], "=", to_string(id)),
			""
		);
		if(!sqlSnake.secureUpdate(update))
			throw MorningException("Failed to approve key : %s", sqlSnake.getError().c_str());

		return true;
	}

	bool revokeTrustedKey(int id){
                sqlSnake = config.getSql();
                sqlupdate_t update;
                update.table = tableName;
                update.valueCount = 1;
                update.cols = new string[1]{
                        colNames[sql_trusted]
                };
                update.values = new string[1]{
                        "0"
                };
                update.wheres = sqlSnake.addToWhere(
                        update.wheres,
                        sqlSnake.generateWhere(colNames[sql_id], "=", to_string(id)),
                        ""
                );
                if(!sqlSnake.secureUpdate(update))
                        throw MorningException("Failed to revoke key : %s", sqlSnake.getError().c_str());

                return true;
        }

	keyfile *fetchUntrustedKeys(){
		sqlSnake = config.getSql();

		sqlselect_t select;
		select.table = tableName;
		select.colCount = colCount;
		select.cols = colNames;
		select.hasWhere = true;
		
		select.wheres = sqlSnake.addToWhere(
			select.wheres,
			sqlSnake.generateWhere(colNames[sql_trusted], "=", "0", false),
			""
		);
		if(!sqlSnake.secureSelect(select)){
			throw MorningException("Failed to fetch untrusted keys : %s", sqlSnake.getError().c_str());
		}

		sqlresults_t res = sqlSnake.getResults();

		if(res.fieldCount <= 0){
			throw MorningException("Failed to fetch untrusted keys : No fields in result.");
		}

		keyfile *untrustedKeys = NULL;
		size_t keyCount = res.resultCount;
		if(keyCount <= 0){
			return NULL;
		}
		untrustedKeys = new keyfile[keyCount];

		for(int i=0; i<keyCount; i++){
			untrustedKeys[i].id = atoi(res.results[i].values[sql_id].c_str());
			untrustedKeys[i].alias = sqlSnake.desanitize(res.results[i].values[sql_alias]);
			untrustedKeys[i].message = sqlSnake.desanitize(res.results[i].values[sql_justification]);
			untrustedKeys[i].publickey = sqlSnake.desanitize(res.results[i].values[sql_key]);
		}

		recentAllocSize = keyCount;
		return untrustedKeys;
	}

	keyfile *fetchTrustedKeys(){
                sqlSnake = config.getSql();

                sqlselect_t select;
                select.table = tableName;
                select.colCount = colCount;
                select.cols = colNames;
                select.hasWhere = true;

                select.wheres = sqlSnake.addToWhere(
                        select.wheres,
                        sqlSnake.generateWhere(colNames[sql_trusted], "=", "1", false),
                        ""
                );
                if(!sqlSnake.secureSelect(select)){
                        throw MorningException("Failed to fetch trusted keys : %s", sqlSnake.getError().c_str());
                }

                sqlresults_t res = sqlSnake.getResults();

                if(res.fieldCount <= 0){
                        throw MorningException("Failed to fetch trusted keys : No fields in result.");
                }

                keyfile *trustedKeys = NULL;
                size_t keyCount = res.resultCount;
                if(keyCount <= 0){
                        return NULL;
                }
                trustedKeys = new keyfile[keyCount];

                for(int i=0; i<keyCount; i++){
                        trustedKeys[i].id = atoi(res.results[i].values[sql_id].c_str());
                        trustedKeys[i].alias = sqlSnake.desanitize(res.results[i].values[sql_alias]);
                        trustedKeys[i].message = sqlSnake.desanitize(res.results[i].values[sql_justification]);
                        trustedKeys[i].publickey = sqlSnake.desanitize(res.results[i].values[sql_key]);
                }

                recentAllocSize = keyCount;
                return trustedKeys;
	}

	int getIdByKey(string pubkey){
		int ret = 0;
		sqlSnake = config.getSql();

                sqlselect_t select;
                select.table = tableName;
                select.colCount = colCount;
                select.cols = colNames;
                select.hasWhere = true;

                select.wheres = sqlSnake.addToWhere(
                        select.wheres,
                        sqlSnake.generateWhere(colNames[sql_trusted], "=", "1", false),
                        ""
                );
		select.wheres = sqlSnake.addToWhere(
			select.wheres,
			sqlSnake.generateWhere(colNames[sql_key], "=", pubkey, true),
			"AND"
		);
                if(!sqlSnake.secureSelect(select)){
                        throw MorningException("Failed to fetch trusted keys : %s", sqlSnake.getError().c_str());
                }

                sqlresults_t res = sqlSnake.getResults();

                if(res.fieldCount <= 0){
                        throw MorningException("Failed to fetch trusted keys : No fields in result.");
                }
		if(res.resultCount <= 0){
			return -1;
		}

		return atoi(res.results[0].values[sql_id].c_str());
	}

	size_t queryKeys(bool trusted, friendkey_t * ret, int max){
		sqlSnake = config.getSql();

                sqlselect_t select;
                select.table = tableName;
                select.colCount = colCount;
                select.cols = colNames;
                select.hasWhere = true;

                select.wheres = sqlSnake.addToWhere(
                        select.wheres,
                        sqlSnake.generateWhere(colNames[sql_trusted], "=", (trusted) ? "1" : "0", false),
                        ""
                );
                if(!sqlSnake.secureSelect(select)){
                        throw MorningException("Failed to fetch trusted keys : %s", sqlSnake.getError().c_str());
                }

                sqlresults_t res = sqlSnake.getResults();

		if(res.resultCount <= 0){
			return 0;
		}
                if(res.fieldCount <= 0){
                        throw MorningException("Failed to fetch trusted keys : No fields in result.");
                }

		for(int i=0; i<res.resultCount; i++){
			if(i>max){
				res.resultCount = max;
				io.out(MORNING_IO_GENERAL, "Max return value hit.\n");
				break;
			}
			ret[i].id = atoi(res.results[i].values[sql_id].c_str());
        		ret[i].trusted = (res.results[i].values[sql_trusted] == "1") ? true : false; 
        		ret[i].alias = res.results[i].values[sql_alias];
        		ret[i].serverAlias = res.results[i].values[sql_server_alias];
        		ret[i].cbip = res.results[i].values[sql_cbip];
        		ret[i].cbhost = res.results[i].values[sql_cbhost];
        		ret[i].cbport = atoi(res.results[i].values[sql_cbport].c_str());
        		ret[i].date = res.results[i].values[sql_date];
        		ret[i].pubkey = res.results[i].values[sql_key];
	        	ret[i].justification = res.results[i].values[sql_justification];
		}
		return (size_t)res.resultCount;
	}

	sqltable_t generateTable(void){
                sqltable_t ret;
                ret.name = tableName;
                ret.colCount = colCount;
                ret.cols = new sqlcolumn_t[colCount];
                ret.cols[sql_id] = sqlSnake.generatePrimaryColumn(colNames[sql_id], "INT", "NOT NULL AUTO_INCREMENT");
		ret.cols[sql_trusted] = sqlSnake.generateColumn(colNames[sql_trusted], "INT", "NOT NULL DEFAULT 0");
		ret.cols[sql_alias] = sqlSnake.generateColumn(colNames[sql_alias], "VARCHAR(50)", "NOT NULL");
		ret.cols[sql_server_alias] = sqlSnake.generateColumn(colNames[sql_server_alias], "VARCHAR(50)", "NOT NULL");
		ret.cols[sql_cbip] = sqlSnake.generateColumn(colNames[sql_cbip], "VARCHAR(20)", "NOT NULL");
		ret.cols[sql_cbhost] = sqlSnake.generateColumn(colNames[sql_cbhost], "VARCHAR(300)", "NOT NULL");
		ret.cols[sql_cbport] = sqlSnake.generateColumn(colNames[sql_cbport], "VARCHAR(6)", "NOT NULL");
		ret.cols[sql_date] = sqlSnake.generateColumn(colNames[sql_date], "TIMESTAMP", "DEFAULT CURRENT_TIMESTAMP");
                ret.cols[sql_key] = sqlSnake.generateColumn(colNames[sql_key], "TEXT", "NOT NULL");
		ret.cols[sql_justification] = sqlSnake.generateColumn(colNames[sql_justification], "VARCHAR(200)", "");
                return ret;
        }

	void setupTable(void){
		sqlSnake = config.getSql();
		sqltable_t t = generateTable();
                if(!sqlSnake.createTable(t)){
                        throw MorningException(sqlSnake.getError());
                }
	}

	bool createUntrusted(friendkey_t friendKey){
		if(friendKey.pubkey == ""){
			io.out(MORNING_IO_ERROR, "No public key\n");
			return false;
		}
		sqlSnake = config.getSql();

		sqlselect_t select;
		select.table = tableName;
		select.colCount = 1;
		select.cols = new string[1]{
			colNames[sql_id]
		};	
		select.hasWhere = true;
		select.wheres = sqlSnake.addToWhere(
				select.wheres, 
				sqlSnake.generateWhere(colNames[sql_key], "=", friendKey.pubkey, true), 
				""
		);
		if(!sqlSnake.secureSelect(select)){
			throw MorningException("Failed to run key management integrity check : %s\n", sqlSnake.getError().c_str());
		}
		sqlresults_t result;
		result = sqlSnake.getResults();
		if(result.resultCount > 0){
			return false;
		}

		sqlinsert_t insert;
		insert.table = tableName;
		insert.count = 7;
		insert.cols = new string[7]{
			colNames[sql_key],
			colNames[sql_alias],
			colNames[sql_server_alias],
			colNames[sql_justification],
			colNames[sql_cbip],
			colNames[sql_cbhost],
			colNames[sql_cbport]
		};
		insert.values = new string[7]{
			friendKey.pubkey,
			friendKey.alias,
			friendKey.serverAlias,
			friendKey.justification,
			friendKey.cbip,
			friendKey.cbhost,
			to_string(friendKey.cbport)
		};
		if(!sqlSnake.secureInsert(insert)){
			throw MorningException("Failed to store untrusted key.\n");
		}
		return true;
	}
};
