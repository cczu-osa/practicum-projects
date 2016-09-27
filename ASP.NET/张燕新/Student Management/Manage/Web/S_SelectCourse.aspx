<%@ Page Title="" Language="C#" MasterPageFile="~/StudentMasterPage.Master" AutoEventWireup="true" CodeBehind="S_SelectCourse.aspx.cs" Inherits="Web.S_SelectCourse" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
	<div>
		<asp:GridView ID="GridView" runat="server" AutoGenerateColumns="False" OnRowCommand="RowCommand">
			<Columns>
				<asp:BoundField HeaderText="课程编号" DataField="CourseID" />
				<asp:BoundField HeaderText="课程名称" DataField="CourseName" />
				<asp:BoundField DataField="TeacherID" HeaderText="教师编号" />
				<asp:BoundField HeaderText="任课教师" DataField="TeacherName" />
				<asp:BoundField HeaderText="选课情况" DataField="Status" NullDisplayText="未选" />
				<asp:BoundField DataField="Score" HeaderText="成绩" NullDisplayText="0" ReadOnly="True" />
				<asp:BoundField DataField="Credit" HeaderText="学分" />
				<asp:ButtonField ButtonType="Button" HeaderText="选课" Text="选课" CommandName="SelectCourse" />
				<asp:ButtonField ButtonType="Button" HeaderText="退选" Text="退选" CommandName="QuitCourse" />
			</Columns>
		</asp:GridView>
	</div>
</asp:Content>
