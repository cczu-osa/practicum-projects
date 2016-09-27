<%@ Page Title="" Language="C#" MasterPageFile="~/AdminMasterPage.Master" AutoEventWireup="true" CodeBehind="A_ChangeStudent.aspx.cs" Inherits="Web.A_ChangeStudent" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
<div>
	<asp:GridView ID="GridView" runat="server" AutoGenerateColumns="False" 
		onrowcancelingedit="GridView_RowCancelingEdit" 
		onrowdeleting="GridView_RowDeleting" onrowediting="GridView_RowEditing" 
		onrowupdating="GridView_RowUpdating">
		<Columns>
			<asp:BoundField DataField="StudentID" HeaderText="学生学号" ReadOnly="True" />
			<asp:BoundField DataField="StudentName" HeaderText="学生姓名" />
			<asp:BoundField DataField="StudentSex" HeaderText="性别" />
			<asp:BoundField DataField="StudentAge" HeaderText="年龄" />
			<asp:BoundField DataField="StudentMajor" HeaderText="专业" />
			<asp:BoundField DataField="StudentClass" HeaderText="班级" />
			<asp:BoundField DataField="StudentAllScoreRequire" HeaderText="学分要求" 
				NullDisplayText="180" />
			<asp:CommandField ShowEditButton="True" ShowDeleteButton="True" />
		</Columns>
	</asp:GridView>
</div>
</asp:Content>
