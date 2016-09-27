using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using BLL;

namespace Web
{
	public partial class A_AddCourse : System.Web.UI.Page
	{
		private DataTransfer transfer = new DataTransfer();

		protected void Page_Load(object sender, EventArgs e)
		{

		}

		protected void OnButtonAdd(object sender, EventArgs e)
		{
			if (transfer.AddCourse(TextCourseID.Text.ToString(), TextCourseName.Text.ToString(), TextCredit.Text.ToString()))
			{
				Response.Write("<script language=javascript>alert('添加课程成功！')</script>");
				TextCourseID.Text = "";
				TextCourseName.Text = "";
				TextCredit.Text = "";
			}
			else
			{
				Response.Write("<script language=javascript>alert('添加课程失败，请检查后重试！')</script>");
			}
		}

		protected void OnButtonReset(object sender, EventArgs e)
		{
			TextCourseID.Text = "";
			TextCourseName.Text = "";
			TextCredit.Text = "";
		}
	}
}