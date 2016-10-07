USE SPJ;

INSERT INTO S(`SNO`, `SNAME`, `STATUS`, `CITY`) VALUES('S1', '精益', 20, '天津'), ('S2', '盛锡', 10, '北京');
INSERT INTO P(`PNO`, `PNAME`, `COLOR`, `WEIGHT`) VALUES('P1', '螺母', '红', '12'), ('P2', '螺栓', '绿', '17'), ('P3', '螺丝刀', '蓝', '105.5');
INSERT INTO J(`JNO`, `JNAME`, `CITY`) VALUES('J1', '一汽', '北京'), ('J2', '半导体厂', '南京'), ('J3', '', '常州');

INSERT INTO SPJ(`SNO`, `PNO`, `JNO`, `QTY`) VALUES('S1', 'P1', 'J1', 200);
INSERT INTO SPJ(`SNO`, `PNO`, `JNO`) VALUES('S1', 'P1', 'J3');

USE school;

INSERT INTO Student(`Sno`, `Sname`, `Ssex`, `Sage`, `Sdept`) VALUES
('9512101', '李勇', '男', '19', '计算机系'),
('9512103', '王敏', '女', '20', '计算机系'),
('9521101', '张莉', '女', '22', '信息系'),
('9521102', '吴宾', '男', '21', '信息系'),
('9521103', '张海', '男', '20', '信息系'),
('9531101', '钱小平','女', '18', '数学系'),
('9531102', '王大力', '男', '19', '数学系');

INSERT INTO Course(`Cno`, `Cname`, `Ccredit`, `Semster`, `Period`) VALUES
('C01', '计算机导论', '3', '1', '3'),
('C02', 'VB', '4', '3', '4'),
('C03', '计算机网络', '4', '7', '4'),
('C04', '数据库基础', '6', '6', '4'), 
('C05', '高等数学', '8', '1', '8');

INSERT INTO SC(`Sno`, `Cno`, `Grade`) VALUES 
('9512101', 'C03', '95'),
('9512103', 'C03', '51'),
('9512101', 'C05', '80'),
('9512103', 'C05', '60'),
('9521101', 'C05', '72'),
('9521102', 'C05', '80'),
('9521103', 'C05', '45'),
('9531101', 'C05', '81'),
('9531102', 'C05', '94'),
('9512101', 'C01', NULL),
('9531102', 'C01', NULL),
('9512101', 'C02', '87'),
('9512101', 'C04', '76');

UPDATE Student SET Student.SAGE = Student.SAGE + 1; -- 所有学生年龄增加一岁
UPDATE SC SET SC.Grade = 85 WHERE SC.Sno = 9512101 AND SC.Cno = 'C01'; -- 修改“9512101”学生的“C01”课程成绩为85
UPDATE SC SET SC.Grade = 75 WHERE SC.Sno = 9531102 AND SC.Cno = 'C01'; -- 修改“9531102”学生的“C01”课程成绩为70
DELETE FROM SC WHERE SC.Sno = 9531102 AND SC.Cno = 'C05'; -- 删除“9531102”学生“C05”课程的成绩记录
