<%@ Page Title="" Language="C#" MasterPageFile="~/TeacherMasterPage.Master" AutoEventWireup="true" CodeBehind="T_TeachManage.aspx.cs" Inherits="Web.T_TeachManage" %>

<%@ Register Assembly="System.Web.DataVisualization, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35"
	Namespace="System.Web.UI.DataVisualization.Charting" TagPrefix="asp" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
	<asp:DropDownList ID="DropDownList" runat="server" Width="180px" 
		AutoPostBack="True" onselectedindexchanged="DropDownList_SelectedIndexChanged" 
		ontextchanged="DropDownList_TextChanged">
	</asp:DropDownList>
	<br />
	<asp:GridView ID="GridView" runat="server" AutoGenerateColumns="False" 
		onrowcancelingedit="GridView_RowCancelingEdit" 
		onrowediting="GridView_RowEditing" onrowupdating="GridView_RowUpdating">
		<Columns>
			<asp:BoundField DataField="StudentID" HeaderText="学生学号" ReadOnly="True" />
			<asp:BoundField DataField="StudentName" HeaderText="学生姓名" ReadOnly="True" />
			<asp:BoundField DataField="Score" HeaderText="成绩" NullDisplayText="0" />
			<asp:CommandField ShowEditButton="True" />
		</Columns>
	</asp:GridView>
	<asp:Chart ID="Chart1" runat="server" Width="400px" BorderDashStyle="Solid" Palette="BrightPastel" imagetype="Png"  BackSecondaryColor="White" BackGradientStyle="TopBottom" BorderWidth="2" backcolor="#D3DFF0" BorderColor="26, 59, 105">
		<Titles>
			<asp:Title Text="学生成绩分布表">
			</asp:Title>
		</Titles>
		<Series>
			<asp:Series Name="Series1"　ChartType="Bubble" MarkerSize="8" MarkerStyle="Circle">
			</asp:Series>
		</Series>
		<ChartAreas>
			<asp:ChartArea Name="ChartArea1"　BorderColor="64, 64, 64, 64" BackSecondaryColor="Transparent" BackColor="64, 165, 191, 228" ShadowColor="Transparent" BackGradientStyle="TopBottom">
			</asp:ChartArea>
		</ChartAreas>
	</asp:Chart>
</asp:Content>
