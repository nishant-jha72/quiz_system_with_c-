# User Data Management System

[![C++](https://img.shields.io/badge/Language-C%2B%2B-blue)](https://isocpp.org/)
[![MySQL](https://img.shields.io/badge/Database-MySQL-orange)](https://www.mysql.com/)

## Description
This C++ project demonstrates how to integrate a MySQL database with a C++ application to manage user data. Users can be added with their name, email, and score. The system handles **auto-incremented primary keys** and retrieves the UID of the last inserted user. It is designed to showcase basic CRUD operations, database connectivity, and error handling in C++.

## Features
- Add new users to the MySQL database
- Automatically generate unique user IDs (`AUTO_INCREMENT`)
- Retrieve the UID of the last inserted user
- Handles errors such as duplicate emails or database connection issues
- Demonstrates basic C++ and MySQL integration

## Technologies Used
- **C++** – Core programming language
- **MySQL** – Relational database for storing user information
- **MySQL Connector/C++** – Library to connect C++ programs with MySQL

## Database Schema

**Table: `user_data`**

| Column   | Type         | Description                          |
|----------|-------------|--------------------------------------|
| uid      | INT          | Auto-incremented primary key          |
| u_name   | VARCHAR(100) | User's name (not NULL)               |
| u_email  | VARCHAR(100) | User's email (unique, not NULL)      |
| score    | INT          | User's score (default 0)             |

## Functionality
1. Insert a new user with name, email, and score.
2. Automatically generate a unique user ID.
3. Retrieve the UID of the newly added user.
4. Handles insertion errors and invalid data.

## Usage

### 1. Setup MySQL Database
Run the following SQL script in your MySQL server:

```sql
CREATE DATABASE user_system;

USE user_system;

CREATE TABLE user_data (
    uid INT AUTO_INCREMENT PRIMARY KEY,
    u_name VARCHAR(100) NOT NULL,
    u_email VARCHAR(100) UNIQUE NOT NULL,
    score INT DEFAULT 0
);
```
2. Install MySQL Connector/C++
On Ubuntu/Debian:

```bash
Copy code
sudo apt-get update
sudo apt-get install libmysqlcppconn-dev
3. Compile the Program
```
Assume your code file is main.cpp:
Copy code
```bash
g++ main.cpp -o user_system -lmysqlclient
4. Run the Program
bash
Copy code
./user_system
```
Make sure to update your MySQL connection details (host, username, password, database) inside your code before running.

Benefits
Understand database connectivity in C++

Learn how to handle auto-incremented IDs

Practice error handling with MySQL in C++

Build a foundation for larger user management systems

Added clear screen Function 

``` bash
IF You Are In window use

system("clr");

if you are in linux , Ubuntu use

system("clear");
```

DevLoped By Nishant Jha
