<%@ Page Title="" Language="C#" MasterPageFile="~/AdminMasterPage.Master" AutoEventWireup="true" CodeBehind="A_ChangeTeacher.aspx.cs" Inherits="Web.A_ChangeTeacher" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
<div>
	<asp:GridView ID="GridView" runat="server" AutoGenerateColumns="False" 
		onrowcancelingedit="GridView_RowCancelingEdit" 
		onrowdeleting="GridView_RowDeleting" onrowediting="GridView_RowEditing" 
		onrowupdating="GridView_RowUpdating">
		<Columns>
			<asp:BoundField DataField="TeacherID" HeaderText="教师工号" ReadOnly="True" />
			<asp:BoundField DataField="TeacherName" HeaderText="教师姓名" />
			<asp:BoundField DataField="TeacherSex" HeaderText="性别" />
			<asp:BoundField DataField="TeacherAge" HeaderText="年龄" />
			<asp:BoundField DataField="TeacherMajor" HeaderText="专业" />
			<asp:CommandField ShowEditButton="True" ShowDeleteButton="True" />
		</Columns>
	</asp:GridView>
</div>
</asp:Content>
