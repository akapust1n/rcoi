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
INSERT INTO News(title,body) VALUES("gd", "ghj");


drop database if exists lab2NewsTest;
create database lab2NewsTest;
use lab2NewsTest;
CREATE TABLE News LIKE lab2News.News;

drop database if exists lab2Users;
create database lab2Users;

use lab2Users;

CREATE TABLE IF NOT EXISTS Users (
    ID  INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL UNIQUE,
    password VARCHAR(255),
    rating INT NOT NULL DEFAULT  0,
    creationData TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
)  ENGINE=INNODB;

drop database if exists lab2UsersTest;
create database lab2UsersTest;
use lab2UsersTest;
CREATE TABLE Users LIKE lab2Users.Users;

drop database if exists lab2Comments;
create database lab2Comments;

use lab2Comments;
CREATE TABLE IF NOT EXISTS Comments (
    ID  INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    newsId INT NOT NULL,
    userId INT NOT NULL,
    body TEXT,
   rating INT NOT NULL DEFAULT 0,
    creationData TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    KEY(newsId, userId)
)  ENGINE=INNODB;
INSERT INTO Comments(newsId, userId, body) VALUES(1,  2," comment1");
INSERT INTO Comments(newsId, userId, body) VALUES(1,  1," comment2");

drop database if exists lab2CommentsTest;
create database lab2CommentsTest;
use lab2CommentsTest;
CREATE TABLE Comments LIKE lab2Comments.Comments;


