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
			bind();
		}

		//GridView绑定数据
		private void bind()
		{
			DataSet ds = transfer.GetSelectCourse(Session["UserName"].ToString());
			GridView.DataSource = ds.Tables["SelectStatus"].DefaultView;
			GridView.DataBind();
		}
		// GridView的RowCommand事件
		protected void RowCommand(object sender, GridViewCommandEventArgs e)
		{
			int index = Convert.ToInt32(e.CommandArgument);
			string CourseID =GridView.Rows[index].Cells[0].Text.ToString();
			string StudentID = Session["UserName"].ToString();
			if (e.CommandName == "SelectCourse")
			{
				int ret=transfer.Select(CourseID,StudentID);
				switch (ret)
				{
					case 0:
						Response.Write("<script language=javascript>alert('选课操作失败，请稍后重试！')</script>");
						break;
					case 1:
						Response.Write("<script language=javascript>alert('抱歉，你已选择该课程！')</script>");
						break;
					case 2:
						Response.Write("<script language=javascript>alert('恭喜，选课成功！')</script>");
						break;
				}
				bind();
			}
			if (e.CommandName == "QuitCourse")
			{
				int ret = transfer.Quit(CourseID, StudentID);
				switch (ret)
				{
					case 0:
						Response.Write("<script language=javascript>alert('退课操作失败，请稍后重试！')</script>");
						break;
					case 1:
						Response.Write("<script language=javascript>alert('抱歉，你未选择该课程！')</script>");
						break;
					case 2:
						Response.Write("<script language=javascript>alert('恭喜，退课成功！')</script>");
						break;
				}
				bind();
			}
		}
	}
}