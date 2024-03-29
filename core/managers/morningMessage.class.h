struct morning_message{
	string clientHost = "";
	string messageDate = "";
	string messageBody = "";
	string messageLength = "";

	int id = -1;
	int keyId = -1;
	int groupId = -1;
	bool read = false;
	bool selfDestruct = false;
	string subject = "";
	string date = "";
	string message = "";
	size_t messageSize = 0;
};
typedef struct morning_message mornmsg;
class MorningMessage{
	private:
		string tableName = "morn_message";
		size_t colCount = 9;
		string colNames[9] = {
			"id",
			"key_id",
			"group_id",
			"was_read",
			"self_destruct",
			"subject",
			"date",
			"message",
			"message_size"
		};
		int sql_id = 0;
		int sql_key_id = 1;
		int sql_group_id = 2;
		int sql_read = 3;
		int sql_self_destruct = 4;
		int sql_subject = 5;
		int sql_date = 6;
		int sql_message = 7;
		int sql_message_size = 8;

		MorningIO io;
		MorningConfig config;
		
		XmlSnake xmlSnake;
		SqlSnake sqlSnake;

	public:
		string err = "";

		sqltable_t generateTable(void){
			sqltable_t table;
			table.name = tableName;
			table.colCount = colCount;
			table.cols = new sqlcolumn_t[colCount]{
				sqlSnake.generatePrimaryColumn(colNames[sql_id], "INT", "NOT NULL AUTO_INCREMENT"),
				sqlSnake.generateForeignColumn(colNames[sql_key_id], "INT", "NOT NULL", "morn_friends", "id"),
				//sqlSnake.generateForeignColumn(colNames[sql_group_id], "INT", "DEFAULT 0", "morn_group", "id"),
				sqlSnake.generateColumn(colNames[sql_group_id], "INT", "DEFAULT 0"),
				sqlSnake.generateColumn(colNames[sql_read], "INT", "DEFAULT 0"),
				sqlSnake.generateColumn(colNames[sql_self_destruct], "INT", "DEFAULT 0"),
				sqlSnake.generateColumn(colNames[sql_subject], "VARCHAR(100)", ""),
				sqlSnake.generateColumn(colNames[sql_date], "TIMESTAMP", "DEFAULT CURRENT_TIMESTAMP"),
				sqlSnake.generateColumn(colNames[sql_message], "TEXT", "NOT NULL"),
				sqlSnake.generateColumn(colNames[sql_message_size], "INT", "NOT NULL")
			};
			return table;
		}

		void setupTable(){
			sqlSnake = config.getSql();
			sqltable_t table = generateTable();
			if(!sqlSnake.createTable(table)){
				throw MorningException("Failed to generate message table : %s", sqlSnake.getError().c_str());
			}
		}

	void setConfig(MorningConfig conf){
		config = conf;
	}


	string getCurrentDateTime(void){
		time_t t = time(NULL);
		struct tm *tm = localtime(&t);
		char str[64];
		strftime(str, sizeof(str), "%c", tm);
		string ret = str;
		return ret;
	}

	bool storeClientMessage(mornmsg msg){
		sqlSnake = config.getSql();
		sqlinsert_t insert;
		insert.table = tableName;
		insert.count = 6;
		insert.cols = new string[6]{
			colNames[sql_key_id],
			colNames[sql_group_id],
			colNames[sql_self_destruct],
			colNames[sql_subject],
			colNames[sql_message],
			colNames[sql_message_size]
		};
		insert.values = new string[6]{
			to_string(msg.keyId),
			to_string(msg.groupId),
			(msg.selfDestruct) ? "1" : "0",
			msg.subject,
			msg.message,
			to_string(msg.messageSize)
		};

		if(!sqlSnake.secureInsert(insert)){
			err = sqlSnake.getError() + "\n"+sqlSnake.getFailedQuery();
			return false;
		}
		return true;
	}

	size_t getUnreadMessages(int keyId, mornmsg *messages, int max){
		sqlSnake = config.getSql();
		sqlselect_t select;
		select.table = tableName;
		select.colCount = colCount;
		select.cols = colNames;
		select.hasWhere = true;
		select.wheres = sqlSnake.addToWhere(
			select.wheres,
			sqlSnake.generateWhere(colNames[sql_key_id], "=", to_string(keyId)),
			""	
		);
		select.wheres = sqlSnake.addToWhere(
			select.wheres,
			sqlSnake.generateWhere(colNames[sql_read], "=", "0"),
			"AND"
		);

		if(!sqlSnake.secureSelect(select)){
			throw MorningException("Failed to select unread messages : %s", sqlSnake.getError().c_str());
		}
		size_t ret = 0;
		sqlresults_t res = sqlSnake.getResults();
		if(res.resultCount <= 0){
			return 0;
		}
		if(res.fieldCount <= 0){
			throw MorningException("Failed to select unread messages : No fields found.");
		}
		ret = (size_t)res.resultCount;
		for(int i=0; i<ret; i++){
			if(i >= max){
				return max;
			}
			messages[i].id = atoi(res.results[i].values[sql_id].c_str());
		        messages[i].keyId = atoi(res.results[i].values[sql_key_id].c_str());
		        messages[i].groupId = atoi(res.results[i].values[sql_group_id].c_str());
		        messages[i].read = (res.results[i].values[sql_read] == "1") ? true : false;
		        messages[i].selfDestruct = (res.results[i].values[sql_read] == "1") ? true : false;
		        messages[i].subject = sqlSnake.desanitize(res.results[i].values[sql_subject]);
		        messages[i].date = sqlSnake.desanitize(res.results[i].values[sql_date]);
		        messages[i].message = sqlSnake.desanitize(res.results[i].values[sql_message]);
		        messages[i].messageSize = atoi(res.results[i].values[sql_message_size].c_str());
		}
		return ret;
	}

	size_t getReadMessages(int keyId, mornmsg *messages, int max){
                sqlSnake = config.getSql();
                sqlselect_t select;
                select.table = tableName;
                select.colCount = colCount;
                select.cols = colNames;
                select.hasWhere = true;
                select.wheres = sqlSnake.addToWhere(
                        select.wheres,
                        sqlSnake.generateWhere(colNames[sql_key_id], "=", to_string(keyId)),
                        ""
                );
                select.wheres = sqlSnake.addToWhere(
                        select.wheres,
                        sqlSnake.generateWhere(colNames[sql_read], "=", "1"),
                        "AND"
                );

                if(!sqlSnake.secureSelect(select)){
                        throw MorningException("Failed to select unread messages : %s", sqlSnake.getError().c_str());
                }
                size_t ret = 0;
                sqlresults_t res = sqlSnake.getResults();
                if(res.resultCount <= 0){
                        return 0;
                }
                if(res.fieldCount <= 0){
                        throw MorningException("Failed to select unread messages : No fields found.");
                }
                ret = (size_t)res.resultCount;
                for(int i=0; i<ret; i++){
                        if(i >= max){
                                return max;
                        }
			messages[i].id = atoi(res.results[i].values[sql_id].c_str());
                        messages[i].keyId = atoi(res.results[i].values[sql_key_id].c_str());
                        messages[i].groupId = atoi(res.results[i].values[sql_group_id].c_str());
                        messages[i].read = (res.results[i].values[sql_read] == "1") ? true : false;
                        messages[i].selfDestruct = (res.results[i].values[sql_read] == "1") ? true : false;
                        messages[i].subject = sqlSnake.desanitize(res.results[i].values[sql_subject]);
                        messages[i].date = sqlSnake.desanitize(res.results[i].values[sql_date]);
                        messages[i].message = sqlSnake.desanitize(res.results[i].values[sql_message]);
                        messages[i].messageSize = atoi(res.results[i].values[sql_message_size].c_str());
                }
                return ret;
        }

	bool deleteMessage(mornmsg target){
		sqlSnake = config.getSql();
		sqlwherelist_t wheres;
		if(!sqlSnake.secureDelete(tableName, sqlSnake.addToWhere(
							wheres,
							sqlSnake.generateWhere(colNames[sql_id], "=", to_string(target.id)),
							""
						))){
			throw MorningException("Failed to delete message : %s", sqlSnake.getError().c_str());
		}
		return true;
	}

	bool updateMessage(mornmsg target){
		sqlSnake = config.getSql();
		sqlupdate_t update;
		update.table = tableName;
		update.valueCount = colCount-3;
                update.cols = new string[colCount-3]{
			colNames[sql_read],
                	colNames[sql_self_destruct],
                	colNames[sql_subject],
                	colNames[sql_date],
                	colNames[sql_message],
                	colNames[sql_message_size]
		};
		update.values = new string[colCount-3]{
			(target.read) ? "1" : "0",
			(target.selfDestruct) ? "1" : "0",
			target.subject,
			target.date,
			target.message,
			to_string(target.messageSize)
		};
		update.wheres = sqlSnake.addToWhere(
			update.wheres,
			sqlSnake.generateWhere(colNames[sql_id], "=", to_string(target.id)),
			""
		);
		if(!sqlSnake.secureUpdate(update)){
			throw MorningException("Failed to update message column : %s", sqlSnake.getError().c_str());
		}
		return true;
	}

	string generateSendString(mornmsg msg){
		if(!xmlSnake.openStringWriter())
			throw MorningException("Failed to open xml string writer.");
		if(!xmlSnake.startWritingString())
			throw MorningException("Failed to start xml string writer.");
		if(!xmlSnake.startWritingElement("root"))
                        throw MorningException("Failed to create <root> element.");
		/* Start processing the meat*/
		if(!xmlSnake.writeElement("selfDestruct", (msg.selfDestruct) ? "true" : "false"))
                        throw MorningException("Failed to write clientHost to send string.");
		if(!xmlSnake.writeElement("subject", msg.subject))
                        throw MorningException("Failed to write clientHost to send string.");
		if(!xmlSnake.writeElement("message", msg.message))
                        throw MorningException("Failed to write clientHost to send string.");
		if(!xmlSnake.writeElement("messageSize", to_string(msg.messageSize)))
                        throw MorningException("Failed to write clientHost to send string.");
		/* close out the processor. */
		if(!xmlSnake.stopWritingElement())
                        throw MorningException("Failed to stop writing trusted keys storage location to config file.");
                if(!xmlSnake.stopWritingFile())
                        throw MorningException("Failed to stop writing file.");
		string message = xmlSnake.getCreatedString();
		xmlSnake.freeWriterOutput();

		return message;
	}
};
