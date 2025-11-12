# Shopping Cart Management System
A comprehensive C++ console-based application for managing shopping cart operations with file persistence and robust product management features.

# Project Overview
This Shopping Cart Management System is a feature-rich console application built in C++ that allows users to manage products in a shopping cart efficiently. The system provides complete CRUD (Create, Read, Update, Delete) operations, file handling for data persistence, and detailed billing capabilities.

# Features
# Core Functionality
Product Management: Add, delete, update, and search products

Automatic ID Generation: Unique product ID assignment

Input Validation: Comprehensive data validation for all user inputs

Real-time Calculations: Automatic subtotal and grand total calculations

# Search & Display
Dual Search Options: Search by Product ID or Name (case-insensitive)

Detailed Product Display: Formatted product information display

Cart Statistics: Comprehensive analytics including total value, highest priced item, and stock levels

# Data Persistence
File Storage: Automatic saving and loading from products.txt

Data Recovery: Resume previous sessions with saved data

Error Handling: Robust file operations with error reporting

# Billing & Reporting
Itemized Bills: Detailed receipt-style billing display

Quantity Management: Dynamic quantity updates

Stock Monitoring: Low stock identification

# Technical Features
Object-Oriented Design: Clean class architecture with proper encapsulation

Input Validation: Comprehensive error checking and user input validation

Memory Management: Efficient use of STL containers (vectors)

File Handling: CSV-style file format for data storage

User-Friendly Interface: Intuitive console menu system

# Class Structure
# Product Class
Product attributes management (ID, Name, Price, Quantity)

Validation methods and serialization

File string conversion utilities

# ShoppingCart Class
Cart operations and product management

File I/O operations

Search and statistical functions

Bill calculation and display

# Menu Class
User interface management

Menu navigation and input handling

System flow control

# Getting Started
# Prerequisites
C++ Compiler (GCC, Clang, or MSVC)

C++11 or later compatible environment

# Compilation
bash
g++ -std=c++11 -o shopping_cart main.cpp
Execution
bash
./shopping_cart
# Usage
Add Products: Enter product details with automatic ID generation

Manage Inventory: Update quantities and delete products as needed

Search Functionality: Find products by ID or name

Generate Bills: Calculate total costs with detailed breakdowns

Data Persistence: Automatically save/load cart data

# Sample Operations
Add new products with validated pricing and quantities

Search for products using flexible criteria

Generate detailed invoices with subtotals

View cart statistics and analytics

Maintain data between sessions via file storage

# Validation & Error Handling
Positive value enforcement for prices and quantities

Non-empty product name validation

Duplicate ID prevention

File operation error reporting

Input type validation and recovery

# File Format
Products are stored in CSV format:

text
id,name,price,quantity
1,Laptop,999.99,5
2,Mouse,25.50,10
# Interface Features
Formatted console output with proper alignment

Section headers and separators for better readability

Clear menu navigation with consistent prompts

Confirmation dialogs for destructive operations
