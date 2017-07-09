#include "DataMgr.h"
#include "NotifyMgr.h"
DataMgr* _DataMgr = nullptr;

DataMgr::DataMgr()
{
	_DataMgr = this;
	DB_PATCH_URL = CCFileUtils::sharedFileUtils()->getWritablePath() + DB_FILE_NAME;
}

DataMgr::~DataMgr() {}

DataMgr* DataMgr::GetInstance()
{
	if (!_DataMgr)
		_DataMgr = new DataMgr();
	return _DataMgr;
}

bool DataMgr::TestOpenDB()
{
	sqlite3 *db = nullptr;
	auto status = sqlite3_open(DB_PATCH_URL.c_str(), &db);
	if (status != SQLITE_OK)
	{
		closeDB(db);
		return false;
	}
	closeDB(db);
	return true;
}

sqlite3* DataMgr::openDB()
	{
	// DBファイルを開いた時間を記憶しておく（処理時間計測のため）
	_dbOpenTime = std::chrono::system_clock::now();
	// SQLiteから読込
	sqlite3 *db = nullptr;
	// DBファイルオープン
	auto status = sqlite3_open(DB_PATCH_URL.c_str(), &db);
	if (status != SQLITE_OK)
	{
		return nullptr;
	}
	return db;
}


bool DataMgr::closeDB(sqlite3 *db)
{
	auto status = sqlite3_close(db);
	if (status != SQLITE_OK)
	{
		return false;
	}
	auto duration = std::chrono::system_clock::now() - _dbOpenTime;
	return true;
}

bool DataMgr::PExcute(const char * args)
{
	sqlite3 *db = openDB();
	int status = 0;
	// エラーメッセージ格納用
	char* errorMessage = NULL;
	status = sqlite3_exec(db, args, nullptr, nullptr, &errorMessage);
	if (status != SQLITE_OK)
	{
		return false;
	}
	return true;
}

bool DataMgr::selectUnitDataList(Result& _res, const char* format, ...)
{
	if (!format)
		return false;

	va_list ap;
	char szQuery[MAX_QUERY_LEN];
	va_start(ap, format);
	int res = vsnprintf(szQuery, MAX_QUERY_LEN, format, ap);
	va_end(ap);

	if (res == -1)
		return false;

	sqlite3 *db = openDB();
	// Select
	sqlite3_stmt *stmt = nullptr;
	int rowcount = 0;
	int check = sqlite3_prepare_v2(db, szQuery, -1, &stmt, nullptr);
	if (check == SQLITE_OK)
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
			_res[rowcount] = _info;
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