#ifndef __SQLITE_H__
#define __SQLITE_H__
#include "cocos2d.h"
#include "external\sqlite3\include\sqlite3.h"

#define DB_FILE_NAME_ENC "EncryptedDB.db"
#define ENCRYPT_DB_PASSWORD "23da3i3kJLale"

USING_NS_CC;
class UnitData : public Node
{
public:
	int _id;
	std::string _name;
	float _weight;
	CREATE_FUNC(UnitData);
};
class DataManager
{
private:
	static DataManager* mManager;
	std::chrono::system_clock::time_point _dbOpenTime;
public:
	DataManager();
	static DataManager* getInstance();
	sqlite3* openDB();
	sqlite3* openDBAndStartTransaction();
	bool closeDB(sqlite3 *db);
	bool commitTransactionAndCloseDB(sqlite3 *db);
	void createTable();
	void insertOrUpdateUnitData(int id, std::string name, float weight);
	Vector<UnitData*> selectUnitDataList();
};

#endif