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
			string sql = string.Format("select * from Users where UserName = '{0}' and PassWord = '{1}'",UserName, PassWord);
			MySqlDataReader dr = DBHelper.DRgetDataReader(sql);
			if (!dr.Read()) return 0;
			string UserRole = dr.GetString(2);
			if (UserRole == "Student") return 1;
			if (UserRole == "Teacher") return 2;
			if (UserRole == "Admin") return 3;
			return 0;
		}

		//修改密码
		public int ModifyPassword(string UserName,string OldPassWord,string NewPassWord)
		{
			string sql = string.Format("select * from Users where UserName = '{0}' and PassWord = '{1}'", UserName, OldPassWord);
			MySqlDataReader dr = DBHelper.DRgetDataReader(sql);
			if (!dr.Read()) return 0;
			else
			{
				sql = string.Format("update Users set PassWord = '{1}' where UserName = '{0}'", UserName, NewPassWord);
				//return DBHelper.getDataCommond(sql);
			}
			return 0;
		}
	}
}
