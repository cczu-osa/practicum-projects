using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using BLL;

namespace Web
{
	public partial class A_AddStudent : System.Web.UI.Page
	{
		private DataTransfer transfer = new DataTransfer();

		protected void Page_Load(object sender, EventArgs e)
		{

		}
		
		protected void OnButtonAdd(object sender, EventArgs e)
		{
			if (transfer.AddStudent(TextStudentID.Text.ToString(), TextStudentName.Text.ToString(), TextStudentPassWord.Text.ToString()))
			{
				Response.Write("<script language=javascript>alert('添加学生成功！')</script>");
				TextStudentID.Text = "";
				TextStudentName.Text = "";
				TextStudentPassWord.Text = "";
				TextConfirmPassWord.Text = "";
			}
			else
			{
				Response.Write("<script language=javascript>alert('添加学生失败，请检查后重试！')</script>");
			}
		}

		protected void OnButtonReset(object sender, EventArgs e)
		{
			TextStudentID.Text = "";
			TextStudentName.Text = "";
			TextStudentPassWord.Text = "";
			TextConfirmPassWord.Text = "";
		}
	}
}