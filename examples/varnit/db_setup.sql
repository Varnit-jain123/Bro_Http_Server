-- Run this script once to set up the EduBase database
-- mysql -u root -p < db_setup.sql

CREATE DATABASE IF NOT EXISTS edubase;
USE edubase;

CREATE TABLE IF NOT EXISTS students (
    id          INT AUTO_INCREMENT PRIMARY KEY,
    name        VARCHAR(100) NOT NULL,
    roll        VARCHAR(50)  NOT NULL UNIQUE,
    branch      VARCHAR(100),
    year        VARCHAR(20),
    email       VARCHAR(100),
    phone       VARCHAR(20),
    created_at  TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS faculty (
    id             INT AUTO_INCREMENT PRIMARY KEY,
    name           VARCHAR(100) NOT NULL,
    emp_id         VARCHAR(50)  NOT NULL UNIQUE,
    dept           VARCHAR(100),
    designation    VARCHAR(100),
    email          VARCHAR(100),
    phone          VARCHAR(20),
    specialization VARCHAR(200),
    experience     INT DEFAULT 0,
    created_at     TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Sample data (optional)
INSERT IGNORE INTO students (name, roll, branch, year, email, phone)
VALUES
  ('Varnit Jain',    'CS2024001', 'Computer Science', '3rd Year', 'varnit@college.edu',  '9876543210'),
  ('Prafull Sharma', 'CS2024002', 'Computer Science', '3rd Year', 'prafull@college.edu', '9876543211'),
  ('Ananya Singh',   'EC2024001', 'Electronics',       '2nd Year', 'ananya@college.edu',  '9876543212');

INSERT IGNORE INTO faculty (name, emp_id, dept, designation, email, specialization, experience)
VALUES
  ('Dr. Rajesh Kumar',  'FAC001', 'Computer Science', 'Professor',           'rajesh@college.edu', 'Data Structures, Algorithms', 15),
  ('Prof. Meera Gupta', 'FAC002', 'Mathematics',      'Associate Professor', 'meera@college.edu',  'Discrete Math, Calculus',      10);
