<%@ Page Title="" Language="C#" MasterPageFile="~/AdminMasterPage.Master" AutoEventWireup="true"
	CodeBehind="A_AddStudent.aspx.cs" Inherits="Web.A_AddStudent" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
	<div>
		<table>
			<tr>
				<td style="width:96px; text-align: right;">
					学号：
				</td>
				<td style="width:360px; text-align: left;" colspan="3">
					<asp:TextBox ID="TextStudentID" runat="server"></asp:TextBox>
					<asp:RequiredFieldValidator ID="RequiredFieldValidator1" runat="server" ControlToValidate="TextStudentID"
						ErrorMessage="请输入学号" Display="Dynamic"></asp:RequiredFieldValidator>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					学生姓名：
				</td>
				<td style="width:360px; text-align: left;" colspan="3">
					<asp:TextBox ID="TextStudentName" runat="server"></asp:TextBox>
					<asp:RequiredFieldValidator ID="RequiredFieldValidator2" runat="server" ControlToValidate="TextStudentName"
						ErrorMessage="请输入学生姓名" Display="Dynamic"></asp:RequiredFieldValidator>
				</td>
			</tr>
			<tr>
				<td style="width:96px;text-align: right">
					密码：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:TextBox ID="TextStudentPassWord" runat="server"></asp:TextBox>
					<asp:RequiredFieldValidator ID="RequiredFieldValidator3" runat="server" ControlToValidate="TextStudentPassword"
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
					<asp:CompareValidator ID="CompareValidator1" runat="server" ControlToCompare="TextStudentPassword"
						ControlToValidate="TextConfirmPassWord" ErrorMessage="两次密码不相符" Display="Dynamic"></asp:CompareValidator>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					学生性别：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:TextBox ID="TextStudentSex" runat="server"></asp:TextBox>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					学生年龄：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:TextBox ID="TextStudentAge" runat="server"></asp:TextBox>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					学生专业：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:TextBox ID="TextStudentMajor" runat="server"></asp:TextBox>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					学生班级：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:TextBox ID="TextStudentClass" runat="server"></asp:TextBox>
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
