CREATE DATABASE OOP;
USE OOP;
CREATE TABLE qwe(asd DOUBLE, rty INT, zxc CHAR, PRIMARY KEY(asd));
INSERT INTO qwe(rty, zxc, asd) VALUES (1, "a", 2.1233242398479238098230948329084347897878);
INSERT INTO qwe(zxc, asd, rty) VALUES ("b", 1.1, 3);
INSERT INTO qwe(asd, rty, zxc) VALUES (4.2, 5, "c");
select * from qwe;
UPDATE qwe SET asd=5 WHERE zxc="a";
select * from qwe;
DROP DATABASE OOP;
