﻿<%@ Page Title="" Language="C#" MasterPageFile="~/AdminMasterPage.Master" AutoEventWireup="true" CodeBehind="A_ChangeCourse.aspx.cs" Inherits="Web.A_ChangeCourse" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
	<div>
		<asp:GridView ID="GridView" runat="server" AutoGenerateColumns="False" 
			onrowcancelingedit="GridView_RowCancelingEdit" 
			onrowdeleting="GridView_RowDeleting" onrowediting="GridView_RowEditing" 
			onrowupdating="GridView_RowUpdating">
			<Columns>
				<asp:BoundField DataField="CourseID" HeaderText="课程编号" ReadOnly="True" />
				<asp:BoundField DataField="CourseName" HeaderText="课程名称" />
				<asp:BoundField DataField="Credit" HeaderText="学分" />
				<asp:CommandField ShowEditButton="True" ShowDeleteButton="True" />
			</Columns>
		</asp:GridView>
	</div>
</asp:Content>
