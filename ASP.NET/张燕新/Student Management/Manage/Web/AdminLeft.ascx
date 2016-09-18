<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="AdminLeft.ascx.cs" Inherits="Web.AdminLeft" %>
<asp:TreeView ID="TreeView1" runat="server" ShowLines="True">
	<Nodes>
		<asp:TreeNode Text="学生管理" Value="学生管理">
			<asp:TreeNode Text="添加学生" Value="添加学生" NavigateUrl="~/A_AddStudent.aspx"></asp:TreeNode>
			<asp:TreeNode Text="编辑学生信息" Value="编辑学生信息" NavigateUrl="~/A_ChangeStudent.aspx"></asp:TreeNode>
		</asp:TreeNode>
		<asp:TreeNode Text="教师管理" Value="教师管理">
			<asp:TreeNode Text="添加教师" Value="添加教师" NavigateUrl="~/A_AddTeacher.aspx"></asp:TreeNode>
			<asp:TreeNode Text="教师信息维护" Value="教师信息维护" NavigateUrl="~/A_ChangeTeacher.aspx"></asp:TreeNode>
		</asp:TreeNode>
		<asp:TreeNode Text="课程管理" Value="课程管理">
			<asp:TreeNode Text="添加课程" Value="添加课程" NavigateUrl="~/A_AddCourse.aspx"></asp:TreeNode>
			<asp:TreeNode Text="课程信息维护" Value="课程信息维护" NavigateUrl="~/A_ChangeCourse.aspx"></asp:TreeNode>
		</asp:TreeNode>
		<asp:TreeNode Text="修改密码" Value="修改密码" NavigateUrl="~/ModifyPassword.aspx"></asp:TreeNode>
		<asp:TreeNode Text="退出系统" Value="退出系统" NavigateUrl="~/Login.aspx"></asp:TreeNode>
	</Nodes>
</asp:TreeView>

