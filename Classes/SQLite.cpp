#include "SQLite.h"
DataManager* DataManager::mManager = NULL;
DataManager::DataManager()
{

}
DataManager* DataManager::getInstance()
{
	if (mManager == NULL)
	{
		mManager = new DataManager();
	}
	return mManager;
}
sqlite3* DataManager::openDB()
{
	_dbOpenTime = std::chrono::system_clock::now();
	std::string dbPath = FileUtils::getInstance()->getWritablePath() + DB_FILE_NAME_ENC;
	sqlite3 *db = nullptr;
	auto status = sqlite3_open(dbPath.c_str(), &db);
	if (status != SQLITE_OK)
	{
		CCLOG("sqlite3_open failed.");
		return nullptr;
	}
	return db;

}

// DB�򥪩`�ץ󤷤ƥȥ�󥶥������򥹥��`�Ȥ���

sqlite3* DataManager::openDBAndStartTransaction()
{
	sqlite3 *db = this->openDB();
	if (db)
	{
		// �ȥ�󥶥�������_ʼ
		auto status = sqlite3_exec(db, "BEGIN;", nullptr, nullptr, nullptr);
		if (status != SQLITE_OK)
		{
			CCLOG("��Starting transaction failed.");
			return nullptr;
		}
	}
	return db;
}

// DB�򥯥�`��

bool DataManager::closeDB(sqlite3 *db)
{
	auto status = sqlite3_close(db);
	if (status != SQLITE_OK)
	{
		CCLOG("��Closing DB failed.");
		return false;
	}
	auto duration = std::chrono::system_clock::now() - _dbOpenTime;
	CCLOG("��DB Closed. time : %dms.", (int)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
	return true;
}

// �ȥ�󥶥������򥳥ߥåȤ���DB�򥯥�`������

bool DataManager::commitTransactionAndCloseDB(sqlite3 *db)
{
	auto status = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
	if (status != SQLITE_OK)
	{
		CCLOG("��Commiting transaction failed.");
		return false;
	}
	return this->closeDB(db);
}

void DataManager::createTable()
{
	char* errorMessage = NULL;
	sqlite3 *db = this->openDB();	// �ȥ�󥶥������ʤ��ӾA�ˤ���
	int status;
	std::string createDBSQL = FileUtils::getInstance()->getStringFromFile("res/createTable.sql");
	status = sqlite3_exec(db, createDBSQL.c_str(), nullptr, nullptr, &errorMessage);
	if (status != SQLITE_OK)
	{
		cocos2d::log("��Creating table failed. Message : %s", errorMessage);
		CCASSERT(false, errorMessage);
	}
	// DB�ե����륯��`��
	this->closeDB(db);
}

void DataManager::insertOrUpdateUnitData(int id, std::string name, float weight)
{
	sqlite3 *db = this->openDBAndStartTransaction();
	int status = 0;
	// ����`��å��`����{��
	char* errorMessage = NULL;
	// InsertSQL
	auto insertSQL = sqlite3_mprintf("insert or replace into unit_data(id,name,weight) values(%d,%Q,%f)",
		id, name.c_str(), weight
		);
	status = sqlite3_exec(db, insertSQL, nullptr, nullptr, &errorMessage);
	if (status != SQLITE_OK)
	{
		CCLOG("��Inserting UnitData data failed. Message : %s", errorMessage);
		CCASSERT(false, errorMessage);
	}
	else
	{
		CCLOG("��UnitData data successfully updated. no : %d", id);
	}
	// ��ţ�����ƤϤ����ʤ���
	sqlite3_free(insertSQL);
	this->commitTransactionAndCloseDB(db);
}

Vector<UnitData*> DataManager::selectUnitDataList()
{
	Vector<UnitData*> unitDataList;
	sqlite3 *db = this->openDB();
	sqlite3_stmt *stmt = nullptr;
	auto selectSQL = "select id,name,weight from unit_data order by id desc";
	if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			UnitData *unitData = UnitData::create();
			unitData->_id = (int)sqlite3_column_int(stmt, 0);
			unitData->_name = StringUtils::format("%s", sqlite3_column_text(stmt, 1));
			unitData->_weight = (float)sqlite3_column_double(stmt, 2);
			unitDataList.pushBack(unitData);
		}
	}
	else
	{
		CCASSERT(false, "Select UnitDataList error.");
	}
	// Statement�򥯥�`
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	this->closeDB(db);
	return unitDataList;
}

