<%@ Page Title="" Language="C#" MasterPageFile="~/TeacherMasterPage.Master" AutoEventWireup="true" CodeBehind="T_info.aspx.cs" Inherits="Web.WelcomeTeacher" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
	<div>
		<table>
			<tr>
				<td style="width:96px; text-align: right;">
					教师工号：
				</td>
				<td style="width:360px; text-align: left;" colspan="3">
					<asp:Label ID="TextTeacherID" runat="server"></asp:Label>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					教师姓名：
				</td>
				<td style="width:360px; text-align: left;" colspan="3">
					<asp:Label ID="TextTeacherName" runat="server"></asp:Label>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					教师性别：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:Label ID="TextTeacherSex" runat="server"></asp:Label>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					教师年龄：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:Label ID="TextTeacherAge" runat="server"></asp:Label>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					教师专业：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:Label ID="TextTeacherMajor" runat="server"></asp:Label>
				</td>
			</tr>
		</table>
	</div>
</asp:Content>
