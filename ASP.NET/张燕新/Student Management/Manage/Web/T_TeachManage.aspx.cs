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
				bind();
			}
		}

		private DataTable CreatData()
		{
			DataTable dt = new DataTable();
			dt.Columns.Add("Grade", System.Type.GetType("System.String"));
			dt.Columns.Add("Count", System.Type.GetType("System.Int32"));

			string[] n = new string[] { "不及格", "及格", "中等", "良好", "优秀" };
			string[] s = new string[] { "60","70","80","90","100"};
 			Int32[] c = new Int32[5];

			for (int i = 0; i < 5; i++)
			{
				c[i] = Int32.Parse(transfer.GetStudentCount(DropDownList.SelectedValue.ToString(), Session["UserName"].ToString(), s[i]).ToString());
			}
			for (int i = 4; i > 0; i--)
			{
				c[i] = c[i] - c[i - 1];
			}
			for (int i = 0; i < 5; i++)
			{
				DataRow dr = dt.NewRow();
				dr["Grade"] = n[i];
				dr["Count"] = c[i];
				dt.Rows.Add(dr);
			}
			return dt;
		}

		//GridView绑定数据
		private void bind()
		{
			DataSet ds = transfer.GetTeachStudent(DropDownList.SelectedValue.ToString());
			GridView.DataSource = ds.Tables["TeachStudent"].DefaultView;
			GridView.DataBind();
			ds.Dispose();

			DataTable dt = CreatData();
			Chart1.DataSource = dt;//绑定数据
			Chart1.Series["Series1"].ChartType = System.Web.UI.DataVisualization.Charting.SeriesChartType.Line;//设置图表类型
			Chart1.Series[0].XValueMember = "Grade";//X轴数据成员列
			Chart1.Series[0].YValueMembers = "Count";//Y轴数据成员列
			Chart1.Series[0].LegendText = "Grade";
			Chart1.ChartAreas["ChartArea1"].AxisX.Title = "成绩";//X轴标题
			Chart1.ChartAreas["ChartArea1"].AxisY.Title = "人数";//Y轴标题
			Chart1.ChartAreas["ChartArea1"].AxisX.MajorGrid.Enabled = false;//不显示竖着的分割线
			Chart1.Series[0].IsValueShownAsLabel = true;//显示坐标值
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