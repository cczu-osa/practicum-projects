<%@ Page Title="" Language="C#" MasterPageFile="~/StudentMasterPage.Master" AutoEventWireup="true" CodeBehind="S_Graduation.aspx.cs" Inherits="Web.S_Graduation" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
	<div>
		<asp:GridView ID="GridView" runat="server" AutoGenerateColumns="False">
			<Columns>
				<asp:BoundField HeaderText="课程编号" DataField="CourseID" />
				<asp:BoundField HeaderText="课程名称" DataField="CourseName" />
				<asp:BoundField HeaderText="任课教师" DataField="TeacherName" />
				<asp:BoundField DataField="Score" HeaderText="成绩" NullDisplayText="0" ReadOnly="True" />
				<asp:BoundField DataField="Credit" HeaderText="学分" />
			</Columns>
		</asp:GridView>
	</div>
	<div>
		<table>
			<tr>
				<td>
					<span>毕业设计成绩：</span>
				</td>
				<td>
					<asp:Label ID="TextGradScore" runat="server"></asp:Label>
				</td>
				<td>
					<span>毕业要求：</span>
				</td>
				<td>
					<asp:Label ID="TextGradScoreRequire" runat="server"></asp:Label>
				</td>
			</tr>
			<tr>
				<td>
					<span>总学分绩点：</span>
				</td>
				<td>
					<asp:Label ID="TextAllScore" runat="server"></asp:Label>
				</td>
				<td>
					<span>毕业要求：</span>
				</td>
				<td>
					<asp:Label ID="TextAllScoreRequire" runat="server"></asp:Label>
				</td>
			</tr>
			<tr>
				<td colspan="4" align="center">
					<asp:Button ID="Button" runat="server" Text="毕业检查" OnClick="OnButton"/>
				</td>
			</tr>
		</table>
	</div>
</asp:Content>
