<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="ModifyPassword.aspx.cs"
	Inherits="Web.ModifyPassword" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
	<title></title>
</head>
<body>
	<form id="form1" runat="server">
	<div>
		<table align="center">
			<tr>
				<td style="text-align: center" colspan="2">
					<span style="font-size: 24pt;">密码修改</span>
				</td>
			</tr>
			<tr>
				<td style="width: 108px" align="right">
					旧密码：
				</td>
				<td>
					<asp:TextBox ID="OldPassword" runat="server" TextMode="Password"></asp:TextBox>
					<asp:RequiredFieldValidator ID="RequiredFieldValidator1" runat="server" ControlToValidate="OldPassword"
						ErrorMessage="不能为空！" Display="Dynamic"></asp:RequiredFieldValidator>
				</td>
			</tr>
			<tr>
				<td style="width: 108px" align="right">
					新密码：
				</td>
				<td>
					<asp:TextBox ID="NewPassword" runat="server" TextMode="Password"></asp:TextBox>
					<asp:RequiredFieldValidator ID="RequiredFieldValidator2" runat="server" ControlToValidate="NewPassword"
						ErrorMessage="新密码不能为空！" Width="178px" Display="Dynamic"></asp:RequiredFieldValidator>
				</td>
			</tr>
			<tr>
				<td style="width: 108px" align="right">
					确认新密码：
				</td>
				<td>
					<asp:TextBox ID="ConfirmPassword" runat="server" TextMode="Password"></asp:TextBox>
					<asp:CompareValidator ID="CompareValidator1" runat="server" ControlToCompare="ConfirmPassword"
						ControlToValidate="NewPassword" ErrorMessage="密码不一致！" Display="Dynamic"></asp:CompareValidator>
				</td>
			</tr>
			<tr>
				<td colspan="2" align="center">
					<asp:Button ID="ButtonConfirm" runat="server" Text="修改" OnClick="OnButtonConfirm" />
					&nbsp;&nbsp;
					<asp:Button ID="ButtonReset" runat="server" Text="重置" OnClick="OnButtonReset" />
				</td>
			</tr>
			<tr>
				<td align="center" colspan="2">
					<asp:Button ID="ButtonBack" runat="server" OnClientClick="javascript:window.history.back();"
						Text="返回" CausesValidation="False" />
				</td>
			</tr>
		</table>
	</div>
	</form>
</body>
</html>
