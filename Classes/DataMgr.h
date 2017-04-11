#ifndef __DATA_MGR_H__
#define __DATA_MGR_H__

#include "sqlite\sqlite3.h"
#include "cocos2d.h"
#include "Types.h"

USING_NS_CC;
#define DB_FILE_NAME "Datas.db"
#define DB_PASSWORD "test"

#define sDataMgr DataMgr::GetInstance()

class DataMgr
{
public:
	DataMgr();
	~DataMgr();
	static DataMgr* GetInstance();
	bool closeDB(sqlite3 *db);
	sqlite3* openDB();
	bool selectUnitDataList(const char* args, Result& _Result);
private:
	std::chrono::system_clock::time_point _dbOpenTime;
};
#endif