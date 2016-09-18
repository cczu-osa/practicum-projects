<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="TeacherLeft.ascx.cs" Inherits="Web.TeacherLeft" %>
<asp:TreeView ID="TreeView1" runat="server" ShowLines="True">
	<Nodes>
		<asp:TreeNode Text="教学管理" Value="班级信息" NavigateUrl="~/T_TeachManage.aspx"></asp:TreeNode>
		<asp:TreeNode Text="修改密码" Value="修改密码" NavigateUrl="~/ModifyPassword.aspx"></asp:TreeNode>
		<asp:TreeNode Text="退出系统" Value="退出系统" NavigateUrl="~/Login.aspx"></asp:TreeNode>
	</Nodes>
</asp:TreeView>

