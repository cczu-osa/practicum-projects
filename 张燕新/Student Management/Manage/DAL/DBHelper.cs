using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using MySql.Data.MySqlClient;

namespace DAL
{
	public partial class DBHelper
	{
		static private string strConnectionString = System.Configuration.ConfigurationManager.ConnectionStrings["ConnectionString"].ConnectionString;

		public DBHelper()
		{

		}

		// 创建并返回数据库连接
		static private MySqlConnection getDBConnection()
		{
			return new MySqlConnection(strConnectionString);
		}

		// 执行单条SQL操作语句
		static public int ExecuteNonQuery(string sql)
		{
			MySqlConnection conn = getDBConnection();
			MySqlCommand cmd = new MySqlCommand(sql, conn);
			try
			{
				conn.Open();
				if (conn.State == ConnectionState.Closed)
					conn.Open();

				int rows = cmd.ExecuteNonQuery();
				return rows;
			}
			catch (Exception ex)
			{
				throw (ex);
			}
			finally
			{
				conn.Close();
				conn.Dispose();
				cmd.Dispose();
			}
		}

		// 执行SQL查询语句返回第一个值
		static public object ExecuteScalar(string sql)
		{
			MySqlConnection conn = getDBConnection();
			MySqlCommand cmd = new MySqlCommand(sql, conn);
			try
			{
				conn.Open();
				if (conn.State == ConnectionState.Closed)
					conn.Open();
				object obj = cmd.ExecuteScalar();
				return obj;
			}
			catch (Exception ex)
			{
				throw (ex);
			}
			finally
			{
				conn.Close();
				conn.Dispose();
				cmd.Dispose();
			}
		}

		// 返回DataReader
		static public MySqlDataReader ExecuteReader(string sql)
		{
			MySqlConnection conn = getDBConnection();
			MySqlCommand cmd = new MySqlCommand(sql, conn);
			try
			{
				conn.Open();
				if (conn.State == ConnectionState.Closed)
					conn.Open();
				MySqlDataReader dr = cmd.ExecuteReader();
				return dr;
			}
			catch (Exception ex)
			{
				throw (ex);
			}
			finally
			{
				conn.Close();
				conn.Dispose();
				cmd.Dispose();
			}
		}

		// 返回DataSet
		static public DataSet getDataSet(string cmdTxt, string TblName)
		{
			MySqlConnection conn = getDBConnection();
			MySqlCommand cmd = new MySqlCommand(cmdTxt, conn);
			MySqlDataAdapter da = new MySqlDataAdapter(cmd);
			DataSet ds = new DataSet();
			try
			{
				conn.Open();
				if (conn.State == ConnectionState.Closed)
					conn.Open();
				da.Fill(ds, TblName);
			}
			catch (Exception ex)
			{
				throw (ex);
			}
			finally
			{
				conn.Close();
				conn.Dispose();
				cmd.Dispose();
				da.Dispose();
			}
			return ds;
		}
	}
}
