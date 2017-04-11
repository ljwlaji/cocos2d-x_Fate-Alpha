#include "DataMgr.h"
DataMgr* _DataMgr = nullptr;

DataMgr::DataMgr()
{
	_DataMgr = this;
}

DataMgr::~DataMgr() {}

DataMgr* DataMgr::GetInstance()
{
	if (!_DataMgr)
		_DataMgr = new DataMgr();
	return _DataMgr;
}
sqlite3* DataMgr::openDB()
{
	// DB¥Õ¥¡¥¤¥ë¤òé_¤¤¤¿•rég¤òÓ›‘›¤·¤Æ¤ª¤¯£¨„IÀí•régÓ‹œy¤Î¤¿¤á£©
	_dbOpenTime = std::chrono::system_clock::now();
	// SQLite¤«¤éÕiÞz
	std::string dbPath = FileUtils::getInstance()->getWritablePath() + DB_FILE_NAME;
	sqlite3 *db = nullptr;
	// DB¥Õ¥¡¥¤¥ë¥ª©`¥×¥ó
	auto status = sqlite3_open(dbPath.c_str(), &db);
	if (status != SQLITE_OK)
	{
		CCLOG("¨‹sqlite3_open failed.");
		return nullptr;
	}
	CCLOG("¡ðDB opened successfully. File : %s", dbPath.c_str());
	return db;
}


bool DataMgr::closeDB(sqlite3 *db)
{
	auto status = sqlite3_close(db);
	if (status != SQLITE_OK)
	{
		CCLOG("¨‹Closing DB failed.");
		return false;
	}
	auto duration = std::chrono::system_clock::now() - _dbOpenTime;
	CCLOG("¡ðDB Closed. time : %dms.", (int)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
	return true;
}

bool DataMgr::selectUnitDataList(const char* args, Result& _Result)
{
	sqlite3 *db = openDB();
	// Select
	sqlite3_stmt *stmt = nullptr;
	int rowcount = 0;
	if (sqlite3_prepare_v2(db, args, -1, &stmt, nullptr) == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			std::vector<RowInfo> _info;
			for (int k = 0; k != sqlite3_column_count(stmt); k++)
			{
				char msg[255];
				snprintf(msg, 255, "%s", sqlite3_column_text(stmt, k));
				RowInfo _text = msg;
				_info.push_back(_text);
			}
			_Result[rowcount] = _info;
			rowcount++;
		}
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		closeDB(db);
		return true;
	}
	else
	{
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		closeDB(db);
		return false;
	}
}