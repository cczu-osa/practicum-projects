using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using MySql.Data.MySqlClient;
using BLL;

namespace Web
{
	public partial class WelcomeTeacher : System.Web.UI.Page
	{
		private DataTransfer transfer = new DataTransfer();

		protected void Page_Load(object sender, EventArgs e)
		{
			MySqlDataReader dr = transfer.GetSelfinfo(Session["UserName"].ToString(), Session["UserRole"].ToString());
			dr.Read();
			TextTeacherID.Text = dr.GetString(0);
			TextTeacherName.Text = dr.GetString(1);
			TextTeacherSex.Text = dr.GetString(2);
			TextTeacherAge.Text = dr.GetString(3);
			TextTeacherMajor.Text = dr.GetString(4);
		}
	}
}