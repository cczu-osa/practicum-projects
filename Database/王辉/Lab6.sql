USE school;

delimiter |

CREATE TRIGGER SC_check BEFORE INSERT ON SC
FOR EACH ROW
BEGIN
    DECLARE cnter INT;
    SELECT COUNT(*) INTO cnter FROM SC WHERE SC.Cno = NEW.Cno;
    IF cnter > 6 THEN
        signal sqlstate '45000' set message_text = 'The course is full.';
    END IF;
END;

CREATE TRIGGER Student_delete_move BEFORE DELETE ON Student
FOR EACH ROW
BEGIN
    INSERT INTO Deleted_Student(`Sno`, `Sname`, `Ssex`, `Sage`, `Sdept`) VALUES(OLD.`Sno`, OLD.`Sname`, OLD.`Ssex`, OLD.`Sage`, OLD.`Sdept`);   
END;
|

delimiter ;