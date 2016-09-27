using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using MySql.Data.MySqlClient;
using DAL;

namespace BLL
{
	public partial class DataTransfer
	{
		//帐号登录
		public int Login(string UserName, string PassWord)
		{
			string sql = string.Format("select Users.UserRole from Users where UserName = '{0}' and PassWord = '{1}'", UserName, PassWord);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj == null) return 0;
			string UserRole = obj.ToString();
			if (UserRole == "Student") return 1;
			else if (UserRole == "Teacher") return 2;
			else if (UserRole == "Admin") return 3;
			else return 0;
		}

		//个人信息
		public MySqlDataReader GetSelfinfo(string ID,string Role)
		{
			string sql = string.Format("select * from {1} where {1}ID = '{0}'", ID, Role);
			MySqlDataReader dr = DBHelper.ExecuteReader(sql);
			return dr;
		}

		//修改密码
		public bool ModifyPassword(string UserName,string OldPassWord,string NewPassWord)
		{
			string sql = string.Format("select Users.UserRole from Users where UserName = '{0}' and PassWord = '{1}'", UserName, OldPassWord);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj == null) return false;
			else
			{
				sql = string.Format("update Users set PassWord = '{1}' where UserName = '{0}'", UserName, NewPassWord);
				if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
				else return true;
			}
		}

		//获取学生选课信息
		public DataSet GetSelectCourse(string StudentID)
		{
			string sql = string.Format("DROP TEMPORARY TABLE IF EXISTS Temp;CREATE TEMPORARY TABLE Temp SELECT * FROM SelectCourse Where StudentID='11001';SELECT TeacherCourse.CourseID,Courses.CourseName,TeacherCourse.TeacherID,Teacher.TeacherName,Temp.`Status`,Temp.Score,Courses.Credit FROM TeacherCourse LEFT JOIN Courses ON TeacherCourse.CourseID=Courses.CourseID LEFT JOIN Teacher ON TeacherCourse.TeacherID=Teacher.TeacherID LEFT JOIN Temp ON (TeacherCourse.CourseID=Temp.CourseID AND TeacherCourse.TeacherID=Temp.TeacherID)", StudentID);
			DataSet ds = DBHelper.getDataSet(sql, "SelectCourse");
			return ds;
		}

		//获取学生已修课程信息
		public DataSet GetFinishCourse(string StudentID)
		{
			string sql = string.Format("SELECT TeacherCourse.CourseID,Courses.CourseName,Teacher.TeacherName,SelectCourse.`Status`,SelectCourse.Score,Courses.Credit FROM TeacherCourse,Courses,SelectCourse,Teacher WHERE TeacherCourse.CourseID=SelectCourse.CourseID AND TeacherCourse.CourseID=Courses.CourseID AND TeacherCourse.TeacherID=Teacher.TeacherID AND SelectCourse.StudentID = '{0}' AND SelectCourse.`Status`='已选' ORDER BY Courses.CourseID", StudentID);
			DataSet ds = DBHelper.getDataSet(sql, "FinishCourse");
			return ds;
		}

		//获取教师任课信息
		public DataSet GetTeachCourseInfo(string TeacherID)
		{
			string sql = string.Format("SELECT TeacherCourse.CourseID,Courses.CourseName FROM TeacherCourse JOIN Courses ON TeacherCourse.CourseID=Courses.CourseID WHERE TeacherCourse.TeacherID = '{0}'", TeacherID);
			DataSet ds = DBHelper.getDataSet(sql, "TeachCourse");
			return ds;
		}

		//获取上课学生信息
		public DataSet GetTeachStudent(string CourseID)
		{
			string sql = string.Format("DROP TEMPORARY TABLE IF EXISTS Temp;CREATE TEMPORARY TABLE Temp SELECT * FROM SelectCourse Where CourseID='{0}';SELECT Student.*,Temp.Score FROM Student,Temp WHERE Student.StudentID=Temp.StudentID", CourseID);
			DataSet ds = DBHelper.getDataSet(sql, "TeachStudent");
			return ds;
		}
		
		//获取所有学生信息
		public DataSet GetStudentInfo()
		{
			string sql = string.Format("SELECT * FROM Student");
			DataSet ds = DBHelper.getDataSet(sql, "StudentInfo");
			return ds;
		}

		//获取所有教师信息
		public DataSet GetTeacherInfo()
		{
			string sql = string.Format("SELECT * FROM Teacher");
			DataSet ds = DBHelper.getDataSet(sql, "TeacherInfo");
			return ds;
		}

		//获取学生毕业设计成绩
		public object GetStudentGradScore(string ID)
		{
			string sql = string.Format("SELECT SelectCourse.Score FROM SelectCourse WHERE CourseID='00000' AND StudentID='{0}'", ID);
			return DBHelper.ExecuteScalar(sql);
		}

		//获取学生毕业学分要求
		public object GetStudentAllScoreRequire(string ID)
		{
			string sql = string.Format("SELECT Student.StudentAllScoreRequire FROM Student WHERE StudentID ='{0}'", ID);
			return DBHelper.ExecuteScalar(sql);
		}

		//获取课程信息
		public DataSet GetCourseInfo()
		{
			string sql = string.Format("SELECT Courses.* FROM Courses");
			DataSet ds = DBHelper.getDataSet(sql, "CourseInfo");
			return ds;
		}

		//获取课程对应学分
		public object GetCourseCredit(string ID)
		{
			string sql = string.Format("SELECT Courses.Credit FROM Courses WHERE Courses.CourseID = '{0}'", ID);
			return DBHelper.ExecuteScalar(sql);
		}

		//获取教师的课程信息
		public DataSet GetTeacherCourseInfo(string TeacherID)
		{
			string sql = string.Format("SELECT TeacherCourse.CourseID,Courses.CourseName,Courses.Credit,TeacherCourse.`Where` FROM TeacherCourse LEFT JOIN Courses ON TeacherCourse.CourseID=Courses.CourseID WHERE TeacherCourse.TeacherID = '{0}'", TeacherID);
			DataSet ds = DBHelper.getDataSet(sql, "TeacherCourseInfo");
			return ds;
		}

		//选课
		public int Select(string CourseID,string TeacherID, string StudentID)
		{
			string sql = string.Format("SELECT SelectCourse.`Status` From SelectCourse WHERE CourseID = '{0}' AND StudentID = '{1}'", CourseID, StudentID);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj == null)
			{
				sql = string.Format("INSERT INTO SelectCourse VALUES('{0}','{1}','{2}','已选','0')", CourseID, TeacherID, StudentID);
				if (DBHelper.ExecuteNonQuery(sql) == 0) return 0;
				else return 2;
			}
			else
			{
				string Status = obj.ToString();
				if (Status == "已选") return 1;
				else if (Status == "未选")
				{
					sql = string.Format("UPDATE SelectCourse SET SelectCourse.`Status`='已选' WHERE CourseID='{0}' AND StudentID = '{1}'", CourseID, StudentID);
					if (DBHelper.ExecuteNonQuery(sql) == 0) return 0;
					else return 2;
				}
				else return 0;
			}
		}

		//退课
		public int Quit(string CourseID, string StudentID)
		{
			string sql = string.Format("SELECT SelectCourse.`Status` From SelectCourse WHERE CourseID = '{0}' AND StudentID = '{1}'", CourseID, StudentID);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj == null)
			{
				return 1;
			}
			else
			{
				string Status = obj.ToString();
				if (Status == "未选") return 1;
				else if (Status == "已选")
				{
					sql = string.Format("DELETE FROM SelectCourse WHERE  WHERE CourseID='{0}' AND StudentID = '{1}'", CourseID, StudentID);
					if (DBHelper.ExecuteNonQuery(sql) == 0) return 0;
					else return 2;
				}
				else return 0;
			}
		}

		//增加操作

		//添加学生
		public bool AddStudent(string ID, string Name, string PassWord, string Sex, string Age, string Major, string Class, string AllScoreRequire)
		{
			string sql = string.Format("select Users.UserRole from Users where UserName = '{0}'", ID);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj != null) return false;
			sql = string.Format("INSERT INTO Users VALUES('{0}','{2}','Student');INSERT INTO Student VALUES('{0}','{1}','{3}','{4}','{5}','{6}','{7}')", ID, Name, PassWord, Sex, Age, Major, Class, AllScoreRequire);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//添加教师
		public bool AddTeacher(string ID, string Name, string PassWord, string Sex, string Age, string Major)
		{
			string sql = string.Format("select Users.UserRole from Users where UserName = '{0}'", ID);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj != null) return false;
			sql = string.Format("INSERT INTO Users VALUES('{0}','{2}','Teacher');INSERT INTO Teacher VALUES('{0}','{1}','{3}','{4}','{5}')", ID, Name, PassWord, Sex, Age, Major);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//管理员添加课程
		public bool AddCourse(string CourseID, string Name, string Credit)
		{
			string sql = string.Format("select Courses.CourseID from Courses where CourseID = '{0}'", CourseID);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj != null) return false;
			sql = string.Format("INSERT INTO Courses VALUES('{0}','{1}','{2}')", CourseID, Name, Credit);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//教师添加课程
		public bool AddTeacherCourse(string CourseID, string TeacherID, string Where)
		{
			string sql = string.Format("SELECT TeacherCourse.`Where` FROM TeacherCourse WHERE CourseID='{0}' AND TeacherID='{1}'", CourseID, TeacherID);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj != null) return false;
			sql = string.Format("INSERT INTO TeacherCourse VALUES('{0}','{1}','{2}')", CourseID, TeacherID, Where);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//删除操作

		//删除学生信息
		public bool DeleteStudent(string ID)
		{
			string sql = string.Format("DELETE FROM Student WHERE Student.StudentID = '{0}';DELETE FROM Users WHERE Users.UserName = '{0}'",ID);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//删除教师信息
		public bool DeleteTeacher(string ID)
		{
			string sql = string.Format("DELETE FROM Teacher WHERE Teacher.TeacherID = '{0}';DELETE FROM Users WHERE Users.UserName = '{0}'", ID);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//删除课程信息
		public bool DeleteCourse(string ID)
		{
			string sql = string.Format("DELETE FROM Courses WHERE Courses.CourseID = '{0}';DELETE FROM TeacherCourse WHERE TeacherCourse.CourseID = '{0}'", ID);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//删除教师课程信息
		public bool DeleteTeacherCourse(string ID,string TeacherID)
		{
			string sql = string.Format("DELETE FROM TeacherCourse WHERE TeacherCourse.CourseID = '{0}' AND TeacherCourse.TeacherID='{1}'", ID, TeacherID);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//修改操作

		//修改学生信息
		public bool UpdateStudent(string ID, string Name, string Sex, string Age, string Major, string Class, string AllScoreRequire)
		{
			string sql = string.Format("UPDATE Student SET StudentName = '{1}',StudentSex ='{2}',StudentAge ='{3}',StudentMajor ='{4}',StudentClass = '{5}',StudentAllScoreRequire='{6}' WHERE StudentID = '{0}'", ID, Name, Sex, Age, Major, Class, AllScoreRequire);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//修改教师信息
		public bool UpdateTeacher(string ID, string Name, string Sex, string Age, string Major)
		{
			string sql = string.Format("UPDATE Teacher SET TeacherName = '{1}',TeacherSex ='{2}',TeacherAge ='{3}',TeacherMajor ='{4}' WHERE TeacherID = '{0}'", ID, Name, Sex, Age, Major);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//修改课程信息
		public bool UpdateCourse(string ID, string Name, string Credit)
		{
			string sql = string.Format("UPDATE Courses SET CourseName='{1}',Credit = '{2}' WHERE CourseID = '{0}'", ID, Name, Credit);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//修改教师课程信息
		public bool UpdateTeacherCourse(string ID, string TeacherID, string Where)
		{
			string sql = string.Format("UPDATE TeacherCourse SET TeacherCourse.`Where` = '{2}' WHERE CourseID = '{0}' and TeacherID = '{2}'", ID, TeacherID, Where);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//修改学生成绩
		public bool UpdateScore(string CourseID,string StudentID, string Score)
		{
			string sql = string.Format("UPDATE SelectCourse SET Score='{2}' WHERE CourseID = '{0}' AND StudentID = '{1}'", CourseID, StudentID, Score);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}
	}
}
