CREATE SCHEMA IF NOT EXISTS `Users` DEFAULT CHARACTER SET utf8;
USE `Users`;

/* Admin */
CREATE TABLE IF NOT EXISTS `Users`.`admin_credentials` (
	`admin_id` INT NOT NULL,
	`name` VARCHAR(50) NOT NULL,
	PRIMARY KEY (`admin_id`));

--#######################################################################################
/* Companies unapproved */
CREATE TABLE IF NOT EXISTS `Users`.`companies_unapproved` (
	`company_id` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(50) NOT NULL,
	`username` VARCHAR(50) NOT NULL,
	`skill_required` VARCHAR(35) NOT NULL,
	PRIMARY KEY (`company_id`));

/* Applicants unapproved */
CREATE TABLE IF NOT EXISTS `Users`.`applicants_unapproved` (
	`applicant_id` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(50) NOT NULL,
	`username` VARCHAR(50) NOT NULL,
	`skill` VARCHAR(35) NOT NULL,
	PRIMARY KEY (`applicant_id`));

--########################################################################################

/* Companies approved */
CREATE TABLE IF NOT EXISTS `Users`.`companies_approved` (
	`company_approved_id` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(50) NOT NULL,
	`skill_required` VARCHAR(35) NOT NULL,
	PRIMARY KEY (`company_approved_id`));


/* Applicants approved */
CREATE TABLE IF NOT EXISTS `Users`.`applicants_approved` (
	`applicant_approved_id` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(50) NOT NULL,
	`skill` VARCHAR(35) NOT NULL,
	PRIMARY KEY (`applicant_approved_id`));
	
/* Database for sent emails */
CREATE TABLE IF NOT EXISTS `Users`.`sent_emails` (
	`email` VARCHAR(20) NOT NULL,
	`name` VARCHAR(50) NOT NULL,
	`code` VARCHAR(50) NOT NULL,
	PRIMARY KEY (`email`));

--########################################################################################
--UNIQUE INDEX `unique_username` (`username` ASC),--
/* Login */
CREATE TABLE IF NOT EXISTS `Users`.`login` (
	`username` VARCHAR(50) NOT NULL,
	`password` VARCHAR(66) NOT NULL,
	`email` VARCHAR(50) NOT NULL,
	`fullname` VARCHAR(50) NOT NULL,
	`online` TINYINT NOT NULL,
	PRIMARY KEY (`username`),
	UNIQUE INDEX `unique_email` (`email` ASC),
	UNIQUE INDEX `username_UNIQUE` (`username` ASC));
    
/* Sessions */
CREATE TABLE IF NOT EXISTS `Users`.`Sessions` (
  `SessionID` VARCHAR(65) NOT NULL,
  `fk_user` VARCHAR(50) NOT NULL,
  `expires` VARCHAR(45) NOT NULL,
  `IP` VARCHAR(15) NOT NULL,
  `online` TINYINT NOT NULL,
  PRIMARY KEY (`SessionID`),
  UNIQUE INDEX `SessionID_UNIQUE` (`SessionID` ASC),
  INDEX `name_idx` (`fk_user` ASC),
  CONSTRAINT `username`
    FOREIGN KEY (`fk_user`)
    REFERENCES `Users`.`login` (`username`)
    ON DELETE CASCADE
    ON UPDATE CASCADE);
    
/* TOTP */
CREATE TABLE IF NOT EXISTS `Users`.`TOTP` (
  `seed` VARCHAR(56) NOT NULL,
  `fk_user` VARCHAR(50) NOT NULL,
  `online` TINYINT NOT NULL,
  PRIMARY KEY (`fk_user`),
  UNIQUE INDEX `seed_UNIQUE` (`seed` ASC),
  CONSTRAINT `fk_user`
    FOREIGN KEY (`fk_user`)
    REFERENCES `Users`.`login` (`username`)
    ON DELETE CASCADE
    ON UPDATE CASCADE);
    
/* 2FA */
CREATE TABLE IF NOT EXISTS `Users`.`2FA` (
  `code` INT NOT NULL,
  `fk_user` VARCHAR(50) NOT NULL,
  `id` INT NOT NULL AUTO_INCREMENT,
  `expires` DATETIME NULL,
  UNIQUE INDEX `fk_user_idx` (`fk_user` ASC),
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_user_2fa` FOREIGN KEY (`fk_user`) REFERENCES `Users`.`login` (`username`)
  ON DELETE CASCADE
  ON UPDATE CASCADE);
--###########################################################################################

CREATE USER 'admin'@'localhost' IDENTIFIED BY 'vxbyt&^%rxcdquthy$@#990234&';
GRANT SELECT, DELETE ON Users.companies_unapproved TO 'admin'@'localhost';
GRANT SELECT, DELETE ON Users.applicants_unapproved TO 'admin'@'localhost';

GRANT INSERT, SELECT ON Users.companies_approved TO 'admin'@'localhost';
GRANT INSERT, SELECT ON Users.applicants_approved TO 'admin'@'localhost';

CREATE USER 'user'@'localhost' IDENTIFIED BY 'ib546@#$$o0dhfbvxcggg984Am6';
GRANT SELECT ON Users.companies_unapproved TO 'user'@'localhost';
GRANT SELECT ON Users.applicants_unapproved TO 'user'@'localhost';


CREATE USER 'register'@'localhost' IDENTIFIED BY 'xvbtyttrabcegtt&$22789%$&#@';
GRANT SELECT, INSERT, UPDATE ON Users.login TO 'register'@'localhost';
GRANT UPDATE(online) ON Users.login TO 'register'@'localhost';
GRANT SELECT, INSERT, UPDATE ON Users.admin_credentials TO 'register'@'localhost';
GRANT SELECT, INSERT, UPDATE ON Users.companies_unapproved TO 'register'@'localhost';
GRANT SELECT, INSERT, UPDATE ON Users.applicants_unapproved TO 'register'@'localhost';
GRANT SELECT, INSERT ON Users.TOTP TO 'register'@'localhost';
GRANT UPDATE(online,seed) ON Users.TOTP TO 'register'@'localhost';
GRANT SELECT, INSERT ON Users.2FA TO 'register'@'localhost';


CREATE USER 'stateful'@'localhost' IDENTIFIED BY 'mxyct654@3$$%uity987#&hicdf';
GRANT INSERT, SELECT ON Users.Sessions TO 'stateful'@'localhost';


CREATE USER 'login'@'localhost' IDENTIFIED BY 'xcv6ertwq2#$!@098ujtyvbxmsk';
GRANT SELECT, UPDATE ON Users.TOTP TO 'login'@'localhost';
GRANT SELECT, INSERT ON Users.2FA TO 'login'@'localhost';
GRANT SELECT, INSERT, UPDATE ON Users.login TO 'login'@'localhost';
GRANT SELECT, INSERT, UPDATE(online) ON Users.Sessions TO 'login'@'localhost';
GRANT SELECT, INSERT ON Users.sent_emails TO 'login'@'localhost';