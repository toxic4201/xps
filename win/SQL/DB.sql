/*
SQLyog Enterprise - MySQL GUI v6.15
MySQL - 4.1.22-standard
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;


/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

/*Table structure for table `accounts` */

DROP TABLE IF EXISTS `accounts`;

CREATE TABLE `accounts` (
  (`AccNum`) int(30) NOT NULL default '0',
  `Password` varchar(255) NOT NULL default '',
  `SecretWord` varbinary(255) NOT NULL default '',
  PRIMARY KEY  (`AccNum`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Table structure for table `players` */

DROP TABLE IF EXISTS `players`;

CREATE TABLE `players` (
  `ID` int(30) NOT NULL default '0',
  `AccNum` int(30) NOT NULL default '0',
  `Name` varchar(255) NOT NULL default '',
  `slot` int(30) NOT NULL default '0',
  `Str` int(50) NOT NULL default '0',
  `Agi` int(50) NOT NULL default '0',
  `Cons` int(59)NOT NULL default '0',
  `Int` int(50) NOT NULL default '0',
  `Wis` int(50) NOT NULL default '0',
  `Path` int(30) NOT NULL default '0',
  `Gender` int(30) NOT NULL default '0',
  `Race` int(30) NOT NULL default '0',
  `Class` int(30) NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
