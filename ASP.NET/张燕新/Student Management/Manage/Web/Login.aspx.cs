using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using BLL;

namespace Web
{
	public partial class Login : System.Web.UI.Page
	{
		private DataTransfer transfer = new DataTransfer();

		protected void Page_Load(object sender, EventArgs e)
		{
			if (!IsPostBack)
			{
				Session["UserName"] = "";
				Session["UserRole"] = "";
			}
		}
		protected void OnButtonLogin(object sender, EventArgs e)
		{
			string UserName = TextUserName.Text.ToString().Trim();
			string PassWord = TextPassWord.Text.ToString().Trim();
			int status=0;
			status = transfer.Login(UserName, PassWord);
			switch (status)
			{
				case 0://用户名或密码错误
					Response.Write("<script language=javascript>alert('您输入的用户名或密码错误，请检查后重新输入！')</script>");
					break;
				case 1://学生登陆成功
					Session["UserName"] = UserName;
                    Session["UserRole"] = "Student";
					Response.Write("<script language=javascript>alert('登录成功！')</script>");
					Response.Redirect("S_info.aspx");
					break;
				case 2://教师登陆成功
					Session["UserName"] = UserName;
					Session["UserRole"] = "Teacher";
					Response.Write("<script language=javascript>alert('登录成功！')</script>");
					Response.Redirect("T_info.aspx");
					break;
				case 3://管理员登陆成功
					Session["UserName"] = UserName;
					Session["UserRole"] = "Admin";
					Response.Write("<script language=javascript>alert('登录成功！')</script>");
					Response.Redirect("A_info.aspx");
					break;
			}
		}
	}
}