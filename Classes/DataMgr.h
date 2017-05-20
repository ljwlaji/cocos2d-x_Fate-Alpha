#ifndef __DATA_MGR_H__
#define __DATA_MGR_H__
#ifdef __APPLE__
#include "sqlite/sqlite3.h"
#else
#include "sqlite\sqlite3.h"
#endif
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
	bool TestOpenDB();
	bool PExcute(const char* args);
	bool selectUnitDataList(Result& _res, const char* format, ...);
private:
	std::chrono::system_clock::time_point _dbOpenTime;
	std::string DB_PATCH_URL;
};
#endif