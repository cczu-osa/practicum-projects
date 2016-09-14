<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="StudentLeft.ascx.cs" Inherits="Web.StudentLeft" %>
<asp:TreeView ID="TreeView1" runat="server">
	<Nodes>
		<asp:TreeNode Text="学生选课" Value="学生选课"></asp:TreeNode>
		<asp:TreeNode Text="修改密码" Value="修改密码"></asp:TreeNode>
	</Nodes>
</asp:TreeView>

