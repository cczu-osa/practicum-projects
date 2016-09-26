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
			TextGradScore.Text = transfer.GetStudentGradScore(Session["UserName"].ToString()).ToString();
			TextGradScoreRequire.Text = string.Format("60");
			int sum=0;
			for (int i = 0; i < GridView.Rows.Count; i++)
			{
				int score= Int32.Parse(GridView.Rows[i].Cells[3].Text.ToString());
				if (score >= 60)
				{
					sum += Int32.Parse(GridView.Rows[i].Cells[4].Text.ToString());
				}
			}
			TextAllScore.Text = sum.ToString();
			TextAllScoreRequire.Text = transfer.GetStudentAllScoreRequire(Session["UserName"].ToString()).ToString();
		}

		//毕业检查
		protected void OnButton(object sender, EventArgs e)
		{
			bind();
			int GradScore = Int32.Parse(TextGradScore.Text);
			int GradScoreRequire = Int32.Parse(TextGradScoreRequire.Text);
			int AllScore = Int32.Parse(TextAllScore.Text);
			int AllScoreRequire = Int32.Parse(TextAllScoreRequire.Text);

			if ((GradScore >= GradScoreRequire) && (AllScore >= AllScoreRequire))
			{
				Response.Write("<script language=javascript>alert('已达到毕业要求！')</script>");
			}
			else
			{
				Response.Write("<script language=javascript>alert('未达到毕业要求！')</script>");
			}
		}
	}
}