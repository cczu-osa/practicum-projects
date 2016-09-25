<%@ Page Title="" Language="C#" MasterPageFile="~/StudentMasterPage.Master" AutoEventWireup="true" CodeBehind="S_Graduation.aspx.cs" Inherits="Web.S_Graduation" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
	<div>
		<asp:GridView ID="GridView" runat="server" AutoGenerateColumns="False">
			<Columns>
				<asp:BoundField HeaderText="课程编号" DataField="CourseID" />
				<asp:BoundField HeaderText="课程名称" DataField="CourseName" />
				<asp:BoundField HeaderText="任课教师" DataField="TeacherName" />
				<asp:BoundField DataField="Score" HeaderText="成绩" NullDisplayText="0" ReadOnly="True" />
				<asp:BoundField DataField="Credit" HeaderText="学分" />
			</Columns>
		</asp:GridView>
	</div>
</asp:Content>
