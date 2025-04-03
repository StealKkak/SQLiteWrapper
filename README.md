# 개요
파이썬의 sqlite3 모듈을 비슷하게 구현한 헤더 온리 래핑 라이브러리 입니다 </br>
코드를 수정하든 자기꺼라고 구라치고 배포하든 맘대루 쓰세여!

# 종속성
SQLite3 기반 래퍼 라이브러리니 당연히 SQLite3이 필요하겠죠? DLL을 쓰든 아말감을 쓰든 어떻게든 SQLite3을 집어넣으세요!

# 사용법
```cpp
SQLiteWrapper* db = new SQLiteWrapper("database.db");//DB 연결
db.execute("INSERT INTO users VALUES = (?, ?, ?)", {"123", "StealKKak", "19"); //std::vector<std::string> >
db.query("SELECT * FROM users WHERE id = ?", {"123"});
std::vector<std::string> result = db.fetchone();
std::vector<std::vector<std::string> > results = db.fetchall();
delete db;
```
