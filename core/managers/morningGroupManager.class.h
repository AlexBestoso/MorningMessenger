struct morning_group{
	int id = -1;
	string name = "";
};
typedef struct morning_group morngroup_t;
class MorningGroupManager{
	private:
		string tableName = "morn_group";
		size_t colCount = 2;
		string colNames[2] = {
			"id",
			"name"
		};
		int sql_id = 0;
		int sql_name = 1;
		
		SqlSnake sqlSnake;
		
		MorningConfig config;
	public:
	
		sqltable_t generateTable(){
			sqltable_t ret;
			ret.name = tableName;
			ret.colCount = colCount;
			ret.cols = new sqlcolumn_t[2]{
				sqlSnake.generatePrimaryColumn(colNames[sql_id], "INT", "NOT NULL AUTO_INCREMENT"),
				sqlSnake.generateColumn(colNames[sql_name], "VARCHAR(50)", "NOT NULL")
			};
			return ret;
		}

		void setupTable(void){
			sqlSnake = config.getSql();
			sqltable_t table = generateTable();

			if(!sqlSnake.createTable(table)){
				throw MorningException("Failed to create group table : %s", sqlSnake.getError().c_str());
			}
		}
};
