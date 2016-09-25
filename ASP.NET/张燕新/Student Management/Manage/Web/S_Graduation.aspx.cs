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
	public partial class S_Graduation : System.Web.UI.Page
	{
		private DataTransfer transfer = new DataTransfer();

		protected void Page_Load(object sender, EventArgs e)
		{
			if (!Page.IsPostBack) bind();
		}


		//GridView绑定数据
		private void bind()
		{
			DataSet ds = transfer.GetFinishCourse(Session["UserName"].ToString());
			GridView.DataSource = ds.Tables["FinishCourse"].DefaultView;
			GridView.DataBind();
			ds.Dispose();
		}
	}
}