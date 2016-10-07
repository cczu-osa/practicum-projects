UPDATE SC SET SC.GRADE = 100 WHERE SC.SNO = '9512101' AND SC.CNO = 'C01';
COMMIT;


SELECT * FROM SC;
INSERT INTO SC(`Sno`, `Cno`, `Grade`) VALUES('9531102', 'C05', 120);
SELECT * FROM SC;
ROLLBACK;
SELECT * FROM SC;
COMMIT;


START Transaction;
SELECT * FROM SC;
INSERT INTO SC(`Sno`, `Cno`, `Grade`) VALUES('9531102', 'C05', 120);
SAVEPOINT SP;
INSERT INTO SC(`Sno`, `Cno`, `Grade`) VALUES('9531102', 'C04', 120);
SELECT * FROM SC;
ROLLBACK to SP;
SELECT * FROM SC;
COMMIT;


DELIMITER ||
CREATE PROCEDURE t_InserSC(IN sno_in CHAR(7), cno_in VARCHAR(20), grade_in SMALLINT)
BEGIN
    DECLARE cnter INT;
    SELECT COUNT(*) INTO CNTER FROM Student WHERE Student.Sno = sno_in;
    IF cnter = 0 THEN
        signal sqlstate '45000' set message_text = 'Student doesnt exist.';
    END IF;
    SELECT COUNT(*) INTO CNTER FROM Course WHERE Course.Cno = cno_in;
    IF cnter = 0 THEN
        signal sqlstate '45000' set message_text = 'Course doesnt exist.';
    END IF;
    INSERT INTO SC(`Sno`, `Cno`, `Grade`) VALUES(sno_in, cno_in, grade_in);
END ||
DELIMITER ;

CALL t_InserSC('9521102', 'C03', NULL);
CALL t_InserSC('9521107', 'C03', NULL);
CALL t_InserSC('9521102', 'C11', NULL);
