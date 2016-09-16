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
		// 帐号登录
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
			string sql = string.Format("DROP TEMPORARY TABLE IF EXISTS Temp;CREATE TEMPORARY TABLE Temp SELECT * FROM SelectCourse Where StudentID='{0}';SELECT Courses.CourseID,Courses.CourseName,Teacher.TeacherName,Temp.Status FROM Courses LEFT JOIN Temp ON Courses.CourseID=Temp.CourseID LEFT JOIN Teacher ON Courses.TeacherID=Teacher.TeacherID", StudentID);
			DataSet ds = DBHelper.getDataSet(sql, "SelectStatus");
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
		public bool AddStudent(string ID, string Name, string PassWord)
		{

			string sql = string.Format("select Users.UserRole from Users where UserName = '{0}'", ID);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj != null) return false;
			sql = string.Format("INSERT INTO Users VALUES('{0}','{2}','Student');INSERT INTO Student VALUES('{0}','{1}')", ID, Name, PassWord);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}

		//添加教师
		public bool AddTeacher(string ID, string Name, string PassWord)
		{
			string sql = string.Format("select Users.UserRole from Users where UserName = '{0}'", ID);
			object obj = DBHelper.ExecuteScalar(sql);
			if (obj != null) return false;
			sql = string.Format("INSERT INTO Users VALUES('{0}','{2}','Teacher');INSERT INTO Teacher VALUES('{0}','{1}')", ID, Name, PassWord);
			if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
			else return true;
		}
	}
}
