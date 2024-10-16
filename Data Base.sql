DROP DATABASE IF EXISTS Tanks;
CREATE DATABASE Tanks;
USE Tanks;

CREATE TABLE Player (id INT PRIMARY KEY, name TEXT, score INT);
CREATE TABLE Map (id INT PRIMARY KEY);
CREATE TABLE Game (id INT PRIMARY KEY, id_Winner INT, id_Map INT, FOREIGN KEY (id_Map) REFERENCES Map (id));
CREATE TABLE Participant (id_P INT, id_G INT, FOREIGN KEY (id_P) REFERENCES Player (id), FOREIGN KEY (id_G) REFERENCES Game (id));

INSERT INTO Player (id, name, score) VALUES (1, 'A', 10);
INSERT INTO Player (id, name, score) VALUES (2, 'B', 15);
INSERT INTO Player (id, name, score) VALUES (3, 'C', 2);
INSERT INTO Player (id, name, score) VALUES (4, 'D', 0);
INSERT INTO Player (id, name, score) VALUES (5, 'E', 12);

INSERT INTO Map (id) VALUES (1);
INSERT INTO Map (id) VALUES (2);
INSERT INTO Map (id) VALUES (3);

INSERT INTO Game (id, id_Winner, id_Map) VALUES (1, 2, 2);
INSERT INTO Game (id, id_Winner, id_Map) VALUES (2, 5, 1);
INSERT INTO Game (id, id_Winner, id_Map) VALUES (3, 5, 3);

INSERT INTO Participant (id_P, id_G) VALUES (1, 1);
INSERT INTO Participant (id_P, id_G) VALUES (2, 1);
INSERT INTO Participant (id_P, id_G) VALUES (3, 1);
INSERT INTO Participant (id_P, id_G) VALUES (4, 1);
INSERT INTO Participant (id_P, id_G) VALUES (5, 1);
INSERT INTO Participant (id_P, id_G) VALUES (1, 2);
INSERT INTO Participant (id_P, id_G) VALUES (2, 2);
INSERT INTO Participant (id_P, id_G) VALUES (3, 2);
INSERT INTO Participant (id_P, id_G) VALUES (4, 3);
INSERT INTO Participant (id_P, id_G) VALUES (5, 3);