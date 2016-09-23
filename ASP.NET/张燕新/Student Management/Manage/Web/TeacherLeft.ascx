<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="TeacherLeft.ascx.cs" Inherits="Web.TeacherLeft" %>
<asp:TreeView ID="TreeView1" runat="server" ShowLines="True">
	<Nodes>
		<asp:TreeNode NavigateUrl="~/T_info.aspx" Text="个人信息" Value="个人信息">
		</asp:TreeNode>
		<asp:TreeNode Text="教学管理" Value="班级信息" NavigateUrl="~/T_TeachManage.aspx"></asp:TreeNode>
		<asp:TreeNode Text="课程管理" Value="课程管理">
			<asp:TreeNode NavigateUrl="~/T_AddCourse.aspx" Text="添加课程" Value="添加课程">
			</asp:TreeNode>
			<asp:TreeNode NavigateUrl="~/T_ChangeCourse.aspx" Text="课程信息维护" Value="课程信息维护">
			</asp:TreeNode>
		</asp:TreeNode>
		<asp:TreeNode Text="修改密码" Value="修改密码" NavigateUrl="~/ModifyPassword.aspx"></asp:TreeNode>
		<asp:TreeNode Text="退出系统" Value="退出系统" NavigateUrl="~/Login.aspx"></asp:TreeNode>
	</Nodes>
</asp:TreeView>

