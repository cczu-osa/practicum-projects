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
	public partial class T_TeachManage : System.Web.UI.Page
	{
		private DataTransfer transfer = new DataTransfer();
		
		protected void Page_Load(object sender, EventArgs e)
		{
			if (!Page.IsPostBack)
			{
				DataSet ds = transfer.GetTeachCourseInfo(Session["UserName"].ToString());
				DropDownList.DataSource = ds.Tables["TeachCourse"].DefaultView;
				DropDownList.DataTextField = "CourseName";
				DropDownList.DataValueField = "CourseID";
				DropDownList.DataBind();
				ds.Dispose();
			}
			bind();
		}

		//GridView绑定数据
		private void bind()
		{
			DataSet ds = transfer.GetTeachStudent(DropDownList.SelectedValue.ToString());
			GridView.DataSource = ds.Tables["TeachStudent"].DefaultView;
			GridView.DataBind();
			ds.Dispose();
		}

		protected void DropDownList_SelectedIndexChanged(object sender, EventArgs e)
		{
			bind();
		}

		protected void DropDownList_TextChanged(object sender, EventArgs e)
		{
			bind();
		}

		protected void GridView_RowEditing(object sender, GridViewEditEventArgs e)
		{
			GridView.EditIndex = e.NewEditIndex;
			bind();
		}

		protected void GridView_RowUpdating(object sender, GridViewUpdateEventArgs e)
		{
			string CourseID = DropDownList.SelectedValue.ToString();
			string StudentID = GridView.Rows[e.RowIndex].Cells[0].Text.ToString();
			string Score = ((TextBox)GridView.Rows[e.RowIndex].Cells[2].Controls[0]).Text.ToString();
			transfer.UpdateScore(CourseID, StudentID, Score);
			GridView.EditIndex = -1;
			bind();
		}

		protected void GridView_RowCancelingEdit(object sender, GridViewCancelEditEventArgs e)
		{
			GridView.EditIndex = -1;
			bind();
		}
	}
}