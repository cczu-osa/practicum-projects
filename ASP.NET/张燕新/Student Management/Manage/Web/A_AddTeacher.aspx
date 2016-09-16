<%@ Page Title="" Language="C#" MasterPageFile="~/AdminMasterPage.Master" AutoEventWireup="true" CodeBehind="A_AddTeacher.aspx.cs" Inherits="Web.A_AddTeacher" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
	<div>
		<table>
			<tr>
				<td style="width:96px; text-align: right;">
					工号：
				</td>
				<td style="width:360px; text-align: left;" colspan="3">
					<asp:TextBox ID="TextTeacherID" runat="server"></asp:TextBox>
					<asp:RequiredFieldValidator ID="RequiredFieldValidator1" runat="server" ControlToValidate="TextTeacherID"
						ErrorMessage="请输入工号" Display="Dynamic"></asp:RequiredFieldValidator>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					教师姓名：
				</td>
				<td style="width:360px; text-align: left;" colspan="3">
					<asp:TextBox ID="TextTeacherName" runat="server"></asp:TextBox>
					<asp:RequiredFieldValidator ID="RequiredFieldValidator2" runat="server" ControlToValidate="TextTeacherName"
						ErrorMessage="请输入教师姓名" Display="Dynamic"></asp:RequiredFieldValidator>
				</td>
			</tr>
			<tr>
				<td style="width:96px;text-align: right">
					密码：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:TextBox ID="TextTeacherPassWord" runat="server"></asp:TextBox>
					<asp:RequiredFieldValidator ID="RequiredFieldValidator3" runat="server" ControlToValidate="TextTeacherPassword"
						ErrorMessage="请输入密码" Display="Dynamic"></asp:RequiredFieldValidator>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					确认密码：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:TextBox ID="TextConfirmPassWord" runat="server"></asp:TextBox>
					<asp:RequiredFieldValidator ID="RequiredFieldValidator4" runat="server" ControlToValidate="TextConfirmPassWord"
						ErrorMessage="请确认密码" Display="Dynamic"></asp:RequiredFieldValidator>
					<asp:CompareValidator ID="CompareValidator1" runat="server" ControlToCompare="TextTeacherPassword"
						ControlToValidate="TextConfirmPassWord" ErrorMessage="两次密码不相符" Display="Dynamic"></asp:CompareValidator>
				</td>
			</tr>
		</table>
		<table>
			<tr>
				<td style="width:133px; text-align: center">
					<asp:Button ID="ButtonAdd" runat="server" OnClick="OnButtonAdd" Text="增加" Width="88px" />
				</td>
				<td style="width:133px; text-align: center">
					<asp:Button ID="ButtonReset" runat="server" OnClick="OnButtonReset" Text="重置" Width="88px" />
				</td>
			</tr>
		</table>
	</div>
</asp:Content>
