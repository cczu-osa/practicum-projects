USE school;

SELECT * FROM Student ORDER BY `Sage`;                                                                          -- (2) 查询全体学生的信息，将学生按年龄升序排序；
SELECT `Sno`, `Sname`, 2016 - `Sage` AS `BirthYear` FROM Student WHERE `Sdept` = '信息系';                       -- (3) 查询“信息系”学生的学号，姓名和出生年份；
SELECT Student.`Sno` FROM Student INNER JOIN SC ON Student.`Sno` = SC.`Sno` WHERE SC.`Grade` < 60;              -- (4) 查询考试不及格的学生的学号；
SELECT Student.`Sno`, Student.`Sname` FROM Student WHERE Student.`Sno` IN (SELECT SC.`Sno` FROM SC);            -- (5) 查询选修了课程的学生的学号和姓名；
SELECT Student.`Sname`, Student.`Sdept`, Student.`Sage` FROM Student WHERE Student.`Sage` BETWEEN 20 AND 23;    -- (6) 查询年龄在20-23岁之间的学生的姓名，系，年龄；
SELECT Student.`Sno`, Student.`Sname` FROM Student WHERE Student.`Sno` IN (
    SELECT Sno FROM SC INNER JOIN Course ON Course.`Cno` = SC.`Cno` WHERE Course.`Cname` = '计算机导论'
) AND Student.`Sno` IN (
    SELECT Sno FROM SC INNER JOIN Course ON Course.`Cno` = SC.`Cno` WHERE Course.`Cname` = '高等数学'
);                                                                                                              -- (7) 查询同时选修了“计算机导论”，“高等数学”课程的学生的学号，姓名；
SELECT * FROM Student WHERE Student.`Sname` LIKE '张_';                                                          -- (8) 查询姓“张”的学生的基本信息；
SELECT COUNT(*) FROM (SELECT DISTINCT Sno FROM SC) AS TMP;                                                      -- (9) 查询选修了课程的学生的总人数；
SELECT Student.`Sno`, Student.`Sname`, SC.`Grade` FROM Student INNER JOIN 
    SC ON SC.`Sno` = Student.`Sno` WHERE SC.`Cno` = 'C05' ORDER BY Student.`Sno` AND SC.`Grade`;                -- (10) 查询选修了“C05”课程的的学生成绩单，要求输出学号，姓名，成绩，结果按班级升序，成绩降序排列；
SELECT Course.`Cno`, Course.`Cname`, AVG(SC.`Grade`) as `AverageGrade`, COUNT(SC.`Sno`) as `Quantity` FROM 
    Course INNER JOIN SC ON SC.`Cno` = Course.`Cno` GROUP BY Course.`Cno`;                                      -- (11) 统计各门课程的成绩，要求输出课程代号，课程名，平均成绩，选修人数。（成绩为NULL值的不统计）；
SELECT Course.`Cno`, Course.`Cname`, COUNT(*) AS `Quantity` FROM 
    Course INNER JOIN SC ON SC.`Cno` = Course.`Cno` AND SC.`Grade` < 60 GROUP BY SC.`Cno`;                      -- (12) 统计各门课程的不及格人数，要求输出课程代号，课程名，不及格人数；
SELECT Student.`Sno`, Student.`Sname`, Student.`Sdept` FROM Student WHERE EXISTS(
    SELECT SC.`Sno`, AVG(SC.`Grade`) AS `AVERAGE` FROM SC GROUP BY SC.`Sno` HAVING `AVERAGE` > 75 AND Student.`Sno` = SC.`Sno`
);                                                                                                              -- (13) 查询选修平均成绩在75分以上的学生的学号，姓名，所在系；
SELECT * FROM Student INNER JOIN (SELECT Sdept FROM Student WHERE Sname = '王大力') AS TMP 
    ON TMP.`Sdept` = Student.`Sdept`;                                                                           -- (14) 查询与“王大力”同一个系的学生的基本信息；
SELECT DISTINCT Student.`Sno`, Student.`Sname`, Student.`Sdept` FROM Student INNER JOIN SC ON SC.`Sno` = Student.Sno INNER JOIN Course ON SC.Cno = Course.Cno
    WHERE Course.`Cname` != 'VB' OR Course.Cname != '数据库基础';                                                 -- (15) 查询未选修“VB”或“数据库基础”两门课的学生的学号，姓名，系名；
SELECT Student.`Sno`, Student.`Sname`, Student.`Sdept` FROM Student INNER JOIN SC ON SC.`Sno` = Student.Sno INNER JOIN Course ON SC.Cno = Course.Cno
    WHERE Course.`Cname` = '高等数学' ORDER BY SC.Grade LIMIT 3;                                                  -- (16) 输出“高等数学”课程成绩前三名的学生的学号，姓名，系名；
SELECT Student.`Sno`, Student.`Sname`, Student.`Sdept` FROM Student WHERE NOT EXISTS (
    SELECT Course.`Cno` FROM Course WHERE Course.`Cno` NOT IN (
        SELECT Course.`Cno` FROM Course LEFT JOIN SC ON SC.`Cno` = Course.`Cno` INNER JOIN Student AS TMP ON TMP.`Sno` = SC.`Sno` WHERE TMP.`Sname` = Student.`Sname`
    )
);                                                                                                               -- (17) 查询选修了全部课程的学生的学号，姓名，系名；
SELECT Student.`Sno`, Student.`Sname`, Student.`Sdept` FROM Student WHERE EXISTS (
    SELECT SC.`Sno`, Count(*) AS CNT FROM SC GROUP BY SC.`Sno` HAVING CNT IN (SELECT Count(Course.`Cno`) as CNT FROM Course) AND Student.`Sno` = SC.`Sno`
);
