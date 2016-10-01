/*
Navicat MySQL Data Transfer

Source Server         : MySQL
Source Server Version : 50520
Source Host           : localhost:3306
Source Database       : Manage

Target Server Type    : MYSQL
Target Server Version : 50520
File Encoding         : 65001

Date: 2016-10-01 19:40:54
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `Courses`
-- ----------------------------
DROP TABLE IF EXISTS `Courses`;
CREATE TABLE `Courses` (
  `CourseID` char(5) NOT NULL,
  `CourseName` varchar(50) NOT NULL,
  `Credit` varchar(5) NOT NULL DEFAULT '0',
  PRIMARY KEY (`CourseID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of Courses
-- ----------------------------
INSERT INTO `Courses` VALUES ('00000', '毕业设计', '18');
INSERT INTO `Courses` VALUES ('11111', 'C语言', '2');
INSERT INTO `Courses` VALUES ('22222', 'C++', '3');
INSERT INTO `Courses` VALUES ('33333', 'Web', '3');
INSERT INTO `Courses` VALUES ('44444', 'Java', '3');
INSERT INTO `Courses` VALUES ('55555', '.Net', '3');
INSERT INTO `Courses` VALUES ('66666', 'PHP', '2');
INSERT INTO `Courses` VALUES ('77777', 'C#', '2');

-- ----------------------------
-- Table structure for `SelectCourse`
-- ----------------------------
DROP TABLE IF EXISTS `SelectCourse`;
CREATE TABLE `SelectCourse` (
  `CourseID` char(5) NOT NULL,
  `TeacherID` char(5) NOT NULL DEFAULT '',
  `StudentID` char(5) NOT NULL,
  `Status` varchar(10) DEFAULT '未选',
  `Score` varchar(3) DEFAULT '0',
  PRIMARY KEY (`CourseID`,`TeacherID`,`StudentID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of SelectCourse
-- ----------------------------
INSERT INTO `SelectCourse` VALUES ('00000', '00115', '11001', '已选', '0');
INSERT INTO `SelectCourse` VALUES ('11111', '00111', '11001', '已选', '91');
INSERT INTO `SelectCourse` VALUES ('11111', '00111', '11002', '已选', '85');
INSERT INTO `SelectCourse` VALUES ('11111', '00111', '11003', '已选', '64');
INSERT INTO `SelectCourse` VALUES ('11111', '00111', '11004', '已选', '71');
INSERT INTO `SelectCourse` VALUES ('11111', '00111', '11005', '已选', '66');
INSERT INTO `SelectCourse` VALUES ('11111', '00111', '11006', '已选', '78');
INSERT INTO `SelectCourse` VALUES ('11111', '00111', '11007', '已选', '73');
INSERT INTO `SelectCourse` VALUES ('22222', '00112', '11001', '已选', '0');
INSERT INTO `SelectCourse` VALUES ('33333', '00113', '11001', '已选', '0');
INSERT INTO `SelectCourse` VALUES ('66666', '00111', '11001', '已选', '65');

-- ----------------------------
-- Table structure for `Student`
-- ----------------------------
DROP TABLE IF EXISTS `Student`;
CREATE TABLE `Student` (
  `StudentID` char(5) NOT NULL,
  `StudentName` varchar(10) NOT NULL,
  `StudentSex` varchar(5) NOT NULL DEFAULT '男',
  `StudentAge` varchar(5) NOT NULL,
  `StudentMajor` varchar(20) NOT NULL,
  `StudentClass` varchar(20) NOT NULL,
  `StudentAllScoreRequire` varchar(5) DEFAULT '180',
  PRIMARY KEY (`StudentID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of Student
-- ----------------------------
INSERT INTO `Student` VALUES ('11001', '李某', '男', '22', '计算机科学与技术', '计算机131', '180');
INSERT INTO `Student` VALUES ('11002', '王某某', '女', '22', '计算机科学与技术', '计算机131', '180');
INSERT INTO `Student` VALUES ('11003', '张某某', '女', '21', '计算机科学与技术', '计算机132', '180');
INSERT INTO `Student` VALUES ('11004', '张某', '男', '23', '计算机科学与技术', '计算机133', '180');
INSERT INTO `Student` VALUES ('11005', '周某某', '男', '21', '计算机科学与技术', '计算机131', '180');
INSERT INTO `Student` VALUES ('11006', '毕某某', '男', '23', '计算机科学与技术', '计算机132', '180');
INSERT INTO `Student` VALUES ('11007', '韩某某', '男', '22', '计算机科学与技术', '计算机132', '180');
INSERT INTO `Student` VALUES ('11008', '凯某', '女', '22', '计算机科学与技术', '计算机132', '');

-- ----------------------------
-- Table structure for `Teacher`
-- ----------------------------
DROP TABLE IF EXISTS `Teacher`;
CREATE TABLE `Teacher` (
  `TeacherID` char(5) NOT NULL,
  `TeacherName` varchar(10) NOT NULL,
  `TeacherSex` varchar(5) NOT NULL,
  `TeacherAge` varchar(5) NOT NULL,
  `TeacherMajor` varchar(20) NOT NULL,
  PRIMARY KEY (`TeacherID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of Teacher
-- ----------------------------
INSERT INTO `Teacher` VALUES ('00111', '李某', '男', '41', '计算机科学与技术');
INSERT INTO `Teacher` VALUES ('00112', '蔡某', '男', '35', '计算机科学与技术');
INSERT INTO `Teacher` VALUES ('00113', '周某', '男', '37', '计算机科学与技术');
INSERT INTO `Teacher` VALUES ('00114', '柯某', '女', '46', '计算机科学与技术');
INSERT INTO `Teacher` VALUES ('00115', '王某', '男', '39', '计算机科学与技术');
INSERT INTO `Teacher` VALUES ('00116', '赖某', '男', '35', '计算机科学与技术');

-- ----------------------------
-- Table structure for `TeacherCourse`
-- ----------------------------
DROP TABLE IF EXISTS `TeacherCourse`;
CREATE TABLE `TeacherCourse` (
  `CourseID` char(5) NOT NULL,
  `TeacherID` char(5) NOT NULL,
  `Where` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`CourseID`,`TeacherID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of TeacherCourse
-- ----------------------------
INSERT INTO `TeacherCourse` VALUES ('00000', '00115', null);
INSERT INTO `TeacherCourse` VALUES ('11111', '00111', 'W1');
INSERT INTO `TeacherCourse` VALUES ('22222', '00112', 'W2');
INSERT INTO `TeacherCourse` VALUES ('33333', '00113', 'W3');
INSERT INTO `TeacherCourse` VALUES ('44444', '00114', 'W4');
INSERT INTO `TeacherCourse` VALUES ('55555', '00115', 'W5');
INSERT INTO `TeacherCourse` VALUES ('66666', '00111', 'W1211');
INSERT INTO `TeacherCourse` VALUES ('77777', '00112', 'W2322');

-- ----------------------------
-- Table structure for `Users`
-- ----------------------------
DROP TABLE IF EXISTS `Users`;
CREATE TABLE `Users` (
  `UserName` varchar(255) NOT NULL,
  `PassWord` varchar(20) NOT NULL,
  `UserRole` varchar(10) NOT NULL,
  PRIMARY KEY (`UserName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of Users
-- ----------------------------
INSERT INTO `Users` VALUES ('00000', '123', 'Admin');
INSERT INTO `Users` VALUES ('00111', '123', 'Teacher');
INSERT INTO `Users` VALUES ('00112', '123', 'Teacher');
INSERT INTO `Users` VALUES ('00113', '123', 'Teacher');
INSERT INTO `Users` VALUES ('00114', '123', 'Teacher');
INSERT INTO `Users` VALUES ('00115', '123', 'Teacher');
INSERT INTO `Users` VALUES ('00116', '123', 'Teacher');
INSERT INTO `Users` VALUES ('11001', '123', 'Student');
INSERT INTO `Users` VALUES ('11002', '123', 'Student');
INSERT INTO `Users` VALUES ('11003', '123', 'Student');
INSERT INTO `Users` VALUES ('11004', '123', 'Student');
INSERT INTO `Users` VALUES ('11005', '123', 'Student');
INSERT INTO `Users` VALUES ('11006', '123', 'Student');
INSERT INTO `Users` VALUES ('11007', '123', 'Student');
INSERT INTO `Users` VALUES ('11008', '123', 'Student');
INSERT INTO `Users` VALUES ('a', 'a', 'Admin');
INSERT INTO `Users` VALUES ('s', 's', 'Student');
INSERT INTO `Users` VALUES ('t', 't', 'Teacher');
