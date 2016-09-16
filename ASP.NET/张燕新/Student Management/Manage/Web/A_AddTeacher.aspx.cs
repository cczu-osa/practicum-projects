using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using BLL;

namespace Web
{
	public partial class A_AddTeacher : System.Web.UI.Page
	{
		private DataTransfer transfer = new DataTransfer();

		protected void Page_Load(object sender, EventArgs e)
		{

		}

		protected void OnButtonAdd(object sender, EventArgs e)
		{
			if (transfer.AddTeacher(TextTeacherID.Text.ToString(), TextTeacherName.Text.ToString(), TextTeacherPassWord.Text.ToString()))
			{
				Response.Write("<script language=javascript>alert('添加教师成功！')</script>");
				TextTeacherID.Text = "";
				TextTeacherName.Text = "";
				TextTeacherPassWord.Text = "";
				TextConfirmPassWord.Text = "";
			}
			else
			{
				Response.Write("<script language=javascript>alert('添加教师失败，请检查后重试！')</script>");
			}
		}

		protected void OnButtonReset(object sender, EventArgs e)
		{
			TextTeacherID.Text = "";
			TextTeacherName.Text = "";
			TextTeacherPassWord.Text = "";
			TextConfirmPassWord.Text = "";
		}
	}
}