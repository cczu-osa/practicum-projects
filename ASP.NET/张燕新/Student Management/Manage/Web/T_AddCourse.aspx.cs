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
	public partial class T_AddCourse : System.Web.UI.Page
	{
		private DataTransfer transfer = new DataTransfer();

		protected void Page_Load(object sender, EventArgs e)
		{
			if (!Page.IsPostBack)
			{
				DataSet ds = transfer.GetCourseInfo();
				DropDownList.DataSource = ds.Tables["CourseInfo"].DefaultView;
				DropDownList.DataTextField = "CourseName";
				DropDownList.DataValueField = "CourseID";
				DropDownList.DataBind();
				ds.Dispose();
				bind();
			}
		}

		//Label绑定数据
		private void bind()
		{
			TextCredit.Text = transfer.GetCourseCredit(DropDownList.SelectedValue.ToString()).ToString();
		}

		protected void OnButtonAdd(object sender, EventArgs e)
		{
			if (transfer.AddTeacherCourse(DropDownList.SelectedValue.ToString(), Session["UserName"].ToString(),TextWhere.Text.ToString()))
			{
				Response.Write("<script language=javascript>alert('添加课程成功！')</script>");
				TextWhere.Text = "";
			}
			else
			{
				Response.Write("<script language=javascript>alert('添加课程失败，请检查后重试！')</script>");
			}
		}

		protected void OnButtonReset(object sender, EventArgs e)
		{
			TextWhere.Text = "";
		}

		protected void DropDownList_SelectedIndexChanged(object sender, EventArgs e)
		{
			bind();
		}

		protected void DropDownList_TextChanged(object sender, EventArgs e)
		{
			bind();
		}
	}
}