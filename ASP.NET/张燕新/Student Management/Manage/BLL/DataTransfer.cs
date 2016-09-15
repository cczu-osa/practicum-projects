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
			string sql = string.Format("select * from Users where UserName = '{0}' and PassWord = '{1}'", UserName, OldPassWord);
			MySqlDataReader dr = DBHelper.ExecuteReader(sql);
			if (!dr.Read()) return false;
			else
			{
				sql = string.Format("update Users set PassWord = '{1}' where UserName = '{0}'", UserName, NewPassWord);
				if (DBHelper.ExecuteNonQuery(sql) == 0) return false;
				else return true;
			}
		}

		//获取学生选课信息
		//SQL语句未写。
		public DataSet GetSelectCourse(string StudentID)
		{
			//string sql = string.Format("DROP TEMPORARY TABLE IF EXISTS Temp;CREATE TEMPORARY TABLE Temp SELECT * FROM SelectCourse Where StudentID='{0}';SELECT Courses.*,Temp.Status FROM Courses LEFT JOIN Temp ON Courses.CourseID=Temp.CourseID;", StudentID);
			string sql = string.Format("SELECT Courses.CourseID,Courses.CourseName,Teacher.TeacherName,SelectCourse.Status FROM Courses INNER JOIN SelectCourse ON Courses.CourseID=SelectCourse.CourseID INNER JOIN Teacher ON Courses.TeacherID=Teacher.TeacherID WHERE SelectCourse.StudentID='{0}'", StudentID);
			DataSet ds = DBHelper.getDataSet(sql, "SelectStatus");
			return ds;
		}
	}
}
