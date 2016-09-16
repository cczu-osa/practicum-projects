<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="TeacherLeft.ascx.cs" Inherits="Web.TeacherLeft" %>
<asp:TreeView ID="TreeView1" runat="server" ShowLines="True">
	<Nodes>
		<asp:TreeNode Text="教学管理" Value="班级信息"></asp:TreeNode>
		<asp:TreeNode Text="修改密码" Value="修改密码"></asp:TreeNode>
		<asp:TreeNode Text="退出系统" Value="退出系统"></asp:TreeNode>
	</Nodes>
</asp:TreeView>

