drop database if exists lab2News;
create database lab2News;

use lab2News;

CREATE TABLE IF NOT EXISTS News (
    ID  INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    body TEXT,
    creationDate TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
)  ENGINE=INNODB;

INSERT INTO News(title,body) VALUES("ab", "cde");


drop database if exists lab2Users;
create database lab2Users;

use lab2Users;

CREATE TABLE IF NOT EXISTS Users (
    ID  INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL UNIQUE,
    password VARCHAR(255),
    creationData TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
)  ENGINE=INNODB;


drop database if exists lab2Comments;
create database lab2Comments;

use lab2Comments;
CREATE TABLE IF NOT EXISTS Comments (
    ID  INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    newsId INT NOT NULL,
    userId INT NOT NULL,
    body TEXT,
    creationData TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    KEY(newsId, userId)
)  ENGINE=INNODB;
