﻿<%@ Page Title="" Language="C#" MasterPageFile="~/TeacherMasterPage.Master" AutoEventWireup="true" CodeBehind="T_AddCourse.aspx.cs" Inherits="Web.T_AddCourse" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
	<div>
		<table>
			<tr>
				<td style="width:96px; text-align: right;">
					课程名：
				</td>
				<td style="width:360px; text-align: left;" colspan="3">
					<asp:DropDownList ID="DropDownList" runat="server" Width="180px" AutoPostBack="True"
						OnSelectedIndexChanged="DropDownList_SelectedIndexChanged" OnTextChanged="DropDownList_TextChanged">
					</asp:DropDownList>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					学分：
				</td>
				<td style="width:360px; text-align: left;" colspan="3">
					<asp:Label ID="TextCredit" runat="server"></asp:Label>
				</td>
			</tr>
			<tr>
				<td style="width:96px; text-align: right">
					上课地点：
				</td>
				<td style="width:360px; text-align: left;" colspan="3">
					<asp:TextBox ID="TextWhere" runat="server"></asp:TextBox>
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
