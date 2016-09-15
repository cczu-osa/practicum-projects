using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data;
using BLL;

namespace Web
{
	public partial class S_SelectCourse : System.Web.UI.Page
	{
		private DataTransfer transfer = new DataTransfer();

		protected void Page_Load(object sender, EventArgs e)
		{
			DataSet ds = transfer.GetSelectCourse(Session["UserName"].ToString());
			GridView1.DataSource = ds.Tables["SelectStatus"].DefaultView;
			GridView1.DataBind();
		}
	}
}