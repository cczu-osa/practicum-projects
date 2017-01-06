# 建表

``` sql
DROP DATABASE IF EXISTS db;
CREATE DATABASE db;
```

### 1. 字典
#### 1.1 dict 字典表
| 字段名 | 字段类型 | 默认值 | 说明 |
|-------|--------|--------|-----|
|dict_id|INTEGER||字典 ID|
|dict_category|INTEGER||字典类型|
|dict_value|VARCHAR(100)||字典值|

**dict_category 为常亮，值如下**
```
0 => 新闻类型字典
1 => 新闻状态字典
2 => 文件类型字典
3 => 地区字典
```

```sql
DROP TABLE IF EXISTS dict;
CREATE TABLE dict(
    dict_id INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,
    dict_category INTEGER,
    dict_value VARCHAR(500)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

### 2. 执法人员表
#### 2.1 officer 表
| 字段名 | 字段类型 | 默认值 | 说明 |
|-------|--------|--------|-----|
|officer_id|INTEGER||执法人员 ID|
|officer_name|VARCHAR(50)||执法人员名字|
|officer_gender|VARCHAR(10)||执法人员性别|
|officer_major|VARCHAR(50)||执法人员专业|
|officer_job|VARCHAR(50)||执法人员职务|
|officer_license_id|VARCHAR(50)||执法人员许可编号|
|officer_locale_id|INTEGER||执法人员工作地区，外键|
|user_id|INTEGER||执法人员用户 ID（用于登录）|

```sql
DROP TABLE IF EXISTS officer;
CREATE TABLE officer(
    officer_id INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,
    officer_name VARCHAR(50) NOT NULL,
    officer_gender VARCHAR(50),
    officer_major VARCHAR(50),
    officer_job VARCHAR(50),
    officer_license_id VARCHAR(50),
    officer_locale VARCHAR(50),
    user_id INTEGER NOT NULL,
    FOREIGN KEY (user_id) REFERENCES uesr(user_id) ON UPDATE CASCADE ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

### 3. 新闻政策表
#### 3.1 news 表
| 字段名 | 字段类型 | 默认值 | 说明 |
|-------|--------|--------|-----|
|news_policy_id|INTEGER||新闻政策 ID|
|news_title|VARCHAR(250)||新闻名称|
|news_content|TEXT||新闻内容|
|news_date|DATE||新闻日期|
|news_type_id|INTEGER||新闻类型 ID，外键|
|new_status_id|INTEGER||新闻状态 ID，外键|
|file_type_id|INTEGER||文件类型 ID，外键|
|publish_officer_id|INTEGER||发布的执法人员 ID，外键|

```sql
DROP TABLE IF EXISTS news;
CREATE TABLE news(
    news_policy_id INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,
    news_title VARCHAR(250) NOT NULL,
    news_content TEXT NOT NULL,
    news_date DATE,
    news_type_id INTEGER,
    news_status_id INTEGER,
    file_type_id INTEGER,
    publish_officer_id INTEGER,
    FOREIGN KEY (news_type_id) REFERENCES dict(dict_id) ON UPDATE CASCADE ON DELETE CASCADE,
    FOREIGN KEY (news_status_id) REFERENCES dict(dict_id) ON UPDATE CASCADE ON DELETE CASCADE,
    FOREIGN KEY (file_type_id) REFERENCES dict(dict_id) ON UPDATE CASCADE ON DELETE CASCADE,
    FOREIGN KEY (publish_officer_id) REFERENCES dict(dict_id) ON UPDATE CASCADE ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

### 4. 访问控制
#### 4.1 tokens 表
| 字段名 | 字段类型 | 默认值 | 说明 |
|-------|--------|--------|-----|
|access_token|VARCHAR(100)||访问秘钥|
|user_id|INTEGER||创建者 ID，外键|

```sql
DROP TABLE IF EXISTS tokens;
CREATE TABLE tokens(
    access_token VARCHAR(100) PRIMARY KEY NOT NULL,
    user_id INTEGER NOT NULL,
    FOREIGN KEY (user_id) REFERENCES uesr(user_id) ON UPDATE CASCADE ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```
#### 4.2 user 表
| 字段名 | 字段类型 | 默认值 | 说明 |
|-------|--------|--------|-----|
|user_id|INTEGER||用户 ID|
|username|VARCHAR(100)||登录名|
|password|VARCHAR(100)||登录密码|
|role|INTEGER||用户角色|

```sql
CREATE TABLE user(
    user_id INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,
    username VARCHAR(100) NOT NULL,
    password VARCHAR(100) NOT NULL,
    role INTEGER NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```
