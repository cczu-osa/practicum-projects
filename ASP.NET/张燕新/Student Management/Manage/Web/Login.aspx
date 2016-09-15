<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Login.aspx.cs" Inherits="Web.Login" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
	<title></title>
	<style type="text/css">
		.style1
		{
			height: 25px;
		}
		.style2
		{
			width: 242px;
			height: 25px;
		}
	</style>
</head>
<body>
	<form id="form1" runat="server">
	<div>
		<table align="center">
			<tr>
				<td colspan="2" style="text-align: center">
					<span style="font-size: 24pt;">学生学籍管理系统</span>
				</td>
			</tr>
			<tr>
				<td align="right" class="style1">
					<span>用户名：</span>
				</td>
				<td class="style2">
					<asp:TextBox ID="TextUserName" runat="server" Width="150px"></asp:TextBox>
					<asp:RequiredFieldValidator ID="RequiredFieldValidator1" runat="server" ControlToValidate="TextUserName"
						ErrorMessage="不能为空！" Display="Dynamic"></asp:RequiredFieldValidator>
				</td>
			</tr>
			<tr>
				<td align="right">
					<span>密&nbsp;码：</span>
				</td>
				<td style="height: 32px; width: 242px;">
					<asp:TextBox ID="TextPassWord" runat="server" TextMode="Password" Width="150px"></asp:TextBox>
					<asp:RequiredFieldValidator ID="RequiredFieldValidator2" runat="server" ControlToValidate="TextPassWord"
						ErrorMessage="不能为空！" Display="Dynamic"></asp:RequiredFieldValidator>
				</td>
			</tr>
			<tr>
				<td colspan="2">
					<asp:Label ID="LabelMessage" runat="server" ForeColor="Red"></asp:Label>
				</td>
			</tr>
			<tr>
				<td align="center" colspan="2">
					<asp:Button ID="ButtonLogin" runat="server" Text="登陆" OnClick="OnButtonLogin" />
					&nbsp;&nbsp;
					<asp:Button ID="ButtonClose" runat="server" Text="退出" OnClientClick="JavaScript:window.close();return false" />
				</td>
			</tr>
		</table>
	</div>
	</form>
</body>
</html>
