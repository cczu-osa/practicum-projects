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
	public partial class WelcomeStudent : System.Web.UI.Page
	{
		private DataTransfer transfer = new DataTransfer();

		protected void Page_Load(object sender, EventArgs e)
		{
			MySqlDataReader dr=transfer.GetSelfinfo(Session["UserName"].ToString(), Session["UserRole"].ToString());
			dr.Read();
			TextStudentID.Text = dr.GetString(0);
			TextStudentName.Text = dr.GetString(1);
			TextStudentSex.Text = dr.GetString(2);
			TextStudentAge.Text = dr.GetString(3);
			TextStudentMajor.Text = dr.GetString(4);
			TextStudentClass.Text = dr.GetString(5);
		}
	}
}