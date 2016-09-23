<%@ Page Title="" Language="C#" MasterPageFile="~/StudentMasterPage.Master" AutoEventWireup="true" CodeBehind="S_info.aspx.cs" Inherits="Web.WelcomeStudent" %>
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
					<asp:Label ID="TextStudentID" runat="server"></asp:Label>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					学生姓名：
				</td>
				<td style="width:360px; text-align: left;" colspan="3">
					<asp:Label ID="TextStudentName" runat="server"></asp:Label>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					学生性别：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:Label ID="TextStudentSex" runat="server"></asp:Label>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					学生年龄：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:Label ID="TextStudentAge" runat="server"></asp:Label>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					学生专业：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:Label ID="TextStudentMajor" runat="server"></asp:Label>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					学生班级：
				</td>
				<td style="width:360px; text-align: left" colspan="3">
					<asp:Label ID="TextStudentClass" runat="server"></asp:Label>
				</td>
			</tr>
		</table>
	</div>
</asp:Content>
