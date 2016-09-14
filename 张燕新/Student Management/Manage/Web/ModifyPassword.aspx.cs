using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using BLL;

namespace Web
{
	public partial class ModifyPassword : System.Web.UI.Page
	{
		private DataTransfer transfer = new DataTransfer();

		protected void Page_Load(object sender, EventArgs e)
		{

		}
		protected void OnButtonConfirm(object sender, EventArgs e)
		{
			transfer.ModifyPassword(Session["UserName"].ToString(),OldPassword.Text.ToString(),NewPassword.Text.ToString());
		}
		protected void OnButtonReset(object sender, EventArgs e)
		{
			OldPassword.Text = "";
			NewPassword.Text = "";
			ConfirmPassword.Text = "";
		}
	}
}