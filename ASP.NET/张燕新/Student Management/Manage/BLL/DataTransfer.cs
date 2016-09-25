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
			string sql = string.Format("SELECT Courses.CourseID,Courses.CourseName,Teacher.TeacherName,SelectCourse.`Status`,SelectCourse.Score,Courses.Credit FROM Courses LEFT JOIN SelectCourse ON Courses.CourseID=SelectCourse.CourseID LEFT JOIN Teacher ON Courses.TeacherID=Teacher.TeacherID WHERE SelectCourse.StudentID = '{0}'", StudentID);
			DataSet ds = DBHelper.getDataSet(sql, "SelectCourse");
			return ds;
		}

		//获取学生已修课程信息
		public DataSet GetFinishCourse(string StudentID)
		{
			string sql = string.Format("SELECT Courses.CourseID,Courses.CourseName,Teacher.TeacherName,SelectCourse.`Status`,SelectCourse.Score,Courses.Credit FROM Courses,SelectCourse,Teacher WHERE Courses.CourseID=SelectCourse.CourseID  AND Courses.TeacherID=Teacher.TeacherID AND SelectCourse.StudentID = '{0}' AND SelectCourse.`Status`='已选' ORDER BY Courses.CourseID", StudentID);
			DataSet ds = DBHelper.getDataSet(sql, "FinishCourse");
			return ds;
		}
		

		//获取教师任课信息
		public DataSet GetTeachCourseInfo(string TeacherID)
		{
			string sql = string.Format("SELECT Courses.CourseID,Courses.CourseName FROM Courses WHERE Courses.TeacherID = '{0}'", TeacherID);
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

		//获取教师的课程信息
		public DataSet GetCourseInfo(string TeacherID)
		{
			string sql = string.Format("SELECT Courses.* FROM Courses LEFT JOIN Teacher ON Courses.TeacherID=Teacher.TeacherID WHERE Teacher.TeacherID = {0}", TeacherID);
			DataSet ds = DBHelper.getDataSet(sql, "CourseInfo");
			return ds;
		}

		//选课
		public int Select(string CourseID, string StudentID)
		{
			string sql = string.Format("SELECT SelectCourse.`Status` From SelectCourse WHERE CourseID = '{0}' AND StudentID = '{1}'", CourseID, StudentID);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj == null)
			{
				sql = string.Format("INSERT INTO SelectCourse VALUES('{0}','{1}','已选')", CourseID, StudentID);
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
					sql = string.Format("UPDATE SelectCourse SET SelectCourse.`Status`='未选' WHERE CourseID='{0}' AND StudentID = '{1}'", CourseID, StudentID);
					if (DBHelper.ExecuteNonQuery(sql) == 0) return 0;
					else return 2;
				}
				else return 0;
			}
		}

		//添加学生
		public bool AddStudent(string ID, string Name, string PassWord, string Sex, string Age, string Major, string Class)
		{

			string sql = string.Format("select Users.UserRole from Users where UserName = '{0}'", ID);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj != null) return false;
			sql = string.Format("INSERT INTO Users VALUES('{0}','{2}','Student');INSERT INTO Student VALUES('{0}','{1}','{2}','{3}','{4}','{5}')", ID, Name, PassWord, Sex, Age, Major, Class);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//添加教师
		public bool AddTeacher(string ID, string Name, string PassWord, string Sex, string Age, string Major)
		{
			string sql = string.Format("select Users.UserRole from Users where UserName = '{0}'", ID);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj != null) return false;
			sql = string.Format("INSERT INTO Users VALUES('{0}','{2}','Teacher');INSERT INTO Teacher VALUES('{0}','{1}','{2}','{3}','{4}')", ID, Name, PassWord, Sex, Age, Major);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//添加课程
		public bool AddCourse(string CourseID, string Name, string TeacherID, string Credit)
		{
			string sql = string.Format("select Courses.CourseID from Courses where CourseID = '{0}'", CourseID);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj != null) return false;
			sql = string.Format("INSERT INTO Courses VALUES('{0}','{1}','{2}','{3}')", CourseID, Name, TeacherID, Credit);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//修改学生信息
		public bool UpdateStudent(string ID, string Name, string Sex, string Age, string Major, string Class)
		{
			string sql = string.Format("UPDATE Student SET StudentName = '{1}',StudentSex ='{2}',StudentAge ='{3}',StudentMajor ='{4}',StudentClass = '{5}' WHERE StudentID = '{0}'", ID, Name, Sex, Age, Major, Class);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//删除学生信息
		public bool DeleteStudent(string ID)
		{
			string sql = string.Format("DELETE FROM Student WHERE Student.StudentID = '{0}';DELETE FROM Users WHERE Users.UserName = '{0}'",ID);
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

		//删除教师信息
		public bool DeleteTeacher(string ID)
		{
			string sql = string.Format("DELETE FROM Teacher WHERE Teacher.TeacherID = '{0}';DELETE FROM Users WHERE Users.UserName = '{0}'", ID);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//修改课程信息
		public bool UpdateCourse(string ID, string Name, string TeacherID, string Credit)
		{
			string sql = string.Format("UPDATE Courses SET CourseName='{1}',Credit = '{3}' WHERE CourseID = '{0}' and TeacherID = '{2}'", ID, Name, TeacherID, Credit);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//删除课程信息
		public bool DeleteCourse(string ID)
		{
			string sql = string.Format("DELETE FROM Courses WHERE Courses.CourseID = '{0}'", ID);
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
