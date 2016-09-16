<%@ Page Title="" Language="C#" MasterPageFile="~/TeacherMasterPage.Master" AutoEventWireup="true" CodeBehind="T_TeachManage.aspx.cs" Inherits="Web.T_TeachManage" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
	<asp:DropDownList ID="DropDownList" runat="server" Width="180px" 
		AutoPostBack="True" onselectedindexchanged="DropDownList_SelectedIndexChanged" 
		ontextchanged="DropDownList_TextChanged">
	</asp:DropDownList>
	<br />
	<asp:GridView ID="GridView" runat="server" AutoGenerateColumns="False">
		<Columns>
			<asp:BoundField DataField="StudentID" HeaderText="学生学号" ReadOnly="True" />
			<asp:BoundField DataField="StudentName" HeaderText="学生姓名" ReadOnly="True" />
		</Columns>
	</asp:GridView>
</asp:Content>
