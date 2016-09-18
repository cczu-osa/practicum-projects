<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="StudentLeft.ascx.cs" Inherits="Web.StudentLeft" %>
<asp:TreeView ID="TreeView1" runat="server" ShowLines="True">
	<Nodes>
		<asp:TreeNode Text="学生选课" Value="学生选课" NavigateUrl="~/S_SelectCourse.aspx"></asp:TreeNode>
		<asp:TreeNode Text="修改密码" Value="修改密码" NavigateUrl="~/ModifyPassword.aspx"></asp:TreeNode>
		<asp:TreeNode Text="退出系统" Value="退出系统" NavigateUrl="~/Login.aspx"></asp:TreeNode>
	</Nodes>
</asp:TreeView>

