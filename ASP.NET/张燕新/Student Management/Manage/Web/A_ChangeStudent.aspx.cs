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
	public partial class A_ChangeStudent : System.Web.UI.Page
	{
		private DataTransfer transfer = new DataTransfer();

		protected void Page_Load(object sender, EventArgs e)
		{
			if (!Page.IsPostBack) bind();
		}

		//GridView绑定数据
		private void bind()
		{
			DataSet ds = transfer.GetStudentInfo();
			GridView.DataSource = ds.Tables["StudentInfo"].DefaultView;
			GridView.DataBind();
			ds.Dispose();
		}

		protected void GridView_RowEditing(object sender, GridViewEditEventArgs e)
		{
			GridView.EditIndex = e.NewEditIndex;
			bind();
		}

		protected void GridView_RowUpdating(object sender, GridViewUpdateEventArgs e)
		{
			string ID = GridView.Rows[e.RowIndex].Cells[0].Text.ToString();
			string Name = ((TextBox)GridView.Rows[e.RowIndex].Cells[1].Controls[0]).Text.ToString();
			string Sex = ((TextBox)GridView.Rows[e.RowIndex].Cells[2].Controls[0]).Text.ToString();
			string Age = ((TextBox)GridView.Rows[e.RowIndex].Cells[3].Controls[0]).Text.ToString();
			string Major = ((TextBox)GridView.Rows[e.RowIndex].Cells[4].Controls[0]).Text.ToString();
			string Class = ((TextBox)GridView.Rows[e.RowIndex].Cells[5].Controls[0]).Text.ToString();
			transfer.UpdateStudent(ID, Name, Sex, Age, Major, Class);
			GridView.EditIndex = -1;
			bind();
		}

		protected void GridView_RowCancelingEdit(object sender, GridViewCancelEditEventArgs e)
		{
			GridView.EditIndex = -1;
			bind();
		}

		protected void GridView_RowDeleting(object sender, GridViewDeleteEventArgs e)
		{
			string ID = GridView.Rows[e.RowIndex].Cells[0].Text.ToString();
			string Name = GridView.Rows[e.RowIndex].Cells[1].Text.ToString();
			transfer.DeleteStudent(ID);
			GridView.EditIndex = -1;
			bind();
		}

	}
}