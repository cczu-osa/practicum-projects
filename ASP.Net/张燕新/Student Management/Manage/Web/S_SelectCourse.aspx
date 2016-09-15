<%@ Page Title="" Language="C#" MasterPageFile="~/StudentMasterPage.Master" AutoEventWireup="true" CodeBehind="S_SelectCourse.aspx.cs" Inherits="Web.S_SelectCourse" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
	<asp:GridView ID="GridView1" runat="server">
		<Columns>
			<asp:BoundField HeaderText="课程编号" />
			<asp:BoundField HeaderText="课程名称" />
			<asp:BoundField HeaderText="任课教师" />
			<asp:BoundField HeaderText="选课情况" />
			<asp:ButtonField ButtonType="Button" HeaderText="选课" Text="选课" 
				CommandName="SelectCourse" />
			<asp:ButtonField ButtonType="Button" HeaderText="退选" Text="退选" 
				CommandName="QuitCourse" />
		</Columns>
	</asp:GridView>
</asp:Content>
