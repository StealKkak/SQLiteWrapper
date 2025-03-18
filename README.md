맘대루 쓰세여
사용법:
```cpp
SQLiteWrapper* db = new SQLiteWrapper("database.db");//DB 연결

std::vector<std::string> params = {"id", "name", "type"};
db.execute("INSERT INTO users VALUES = (?, ?, ?)", params);

std::vector<std::string> param = {"id"};
db.query("SELECT * FROM users WHERE id = ?", param);
std::vector<std::string> result = db.fetchone();
std::vector<std::vector<std::string> > results = db.fetchall();

delete db;
```
