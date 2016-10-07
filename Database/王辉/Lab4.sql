USE school;

CREATE VIEW IS_STUDENT AS SELECT * FROM Student WHERE Sdept='信息系';
CREATE VIEW CJ_STUDENT AS SELECT DISTINCT Student.Sno, Student.Sname, Student.Ssex, Student.Sage, Student.Sdept FROM Student INNER JOIN SC ON SC.`Sno`=Student.`Sno` WHERE SC.Grade < 60;
CREATE VIEW AVG_CJ AS SELECT Student.Sname, AVG(SC.Grade) AS AVERAGE FROM Student INNER JOIN SC ON Student.`Sno` = SC.`Sno` GROUP BY Student.Sno ORDER BY AVERAGE DESC;

/** UPDATE IS_STUDENT SET IS_STUDENT.Sage = IS_STUDENT.Sage + 1; **/

INSERT INTO IS_STUDENT(Sno, Sname, Ssex, Sage) VALUES(9531103, '张玉', '女', 21);

SELECT * FROM AVG_CJ WHERE AVERAGE > 60;

DELIMITER &&
CREATE PROCEDURE student_Grade1()
BEGIN
    SELECT Student.Sno, Student.Sname, Course.Cname, SC.Grade FROM
        Student INNER JOIN SC ON SC.`Sno` = Student.Sno INNER JOIN Course ON SC.Cno = Course.Cno
        ORDER BY Student.Sno;
END &&
DELIMITER ;

/** SET @sdept_in='信息系'; **/

DELIMITER &&
CREATE PROCEDURE student_Grade2(IN dept_in VARCHAR(20))
BEGIN
    SELECT Student.Sno, Student.Sname, Course.Cname, SC.Grade FROM
        Student INNER JOIN SC ON SC.`Sno` = Student.Sno INNER JOIN Course ON SC.Cno = Course.Cno
        WHERE Student.Sdept = dept_in
        ORDER BY Student.Sno;
END &&
DELIMITER ;

DELIMITER &&
CREATE PROCEDURE student_Grade3(OUT grade INT, IN sname_in VARCHAR(20), cname_in VARCHAR(20))
BEGIN
    SET grade = -1;
    SELECT SC.Grade into grade FROM
        Student INNER JOIN SC ON SC.`Sno` = Student.Sno INNER JOIN Course ON SC.Cno = Course.Cno
        WHERE Student.Sname = sname_in AND Course.Cname = cname_in
        ORDER BY Student.Sno;
END &&
DELIMITER ;

DELIMITER &&
CREATE PROCEDURE student_Grade4(OUT res INT, IN sno_in CHAR(7), cno_in VARCHAR(20))
BEGIN
    DECLARE cnter INT;
    SET cnter = -1;
    SELECT COUNT(*) into cnter FROM SC WHERE SC.Cno = cno_in;
    IF cnter > 50 THEN
        SET res = -1;
    ELSE
        SELECT COUNT(*) into cnter FROM SC WHERE SC.Sno = sno_in;
        IF cnter > 3 THEN
            SET res = 0;
        ELSE
            INSERT INTO SC(Sno, Cno) VALUES(sno_in, cno_in);
            SET res = 1;
        END IF;
    END IF;
END &&
DELIMITER ;