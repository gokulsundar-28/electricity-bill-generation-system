⚡ Electricity Bill Generation System

A console-based Electricity Bill Generation System developed using C programming. This mini project manages consumer registration, electricity meter readings, bill calculation, penalty calculation and payment status tracking.

📌 Project Overview

The Electricity Bill Generation System provides a simple way to manage consumer details and generate electricity bills based on recorded meter readings.

It uses file handling to store consumer and billing records, allowing data to remain available when the application is restarted.

✨ Features

Consumer Registration: Register consumers with a unique Consumer ID, name, address and phone number.

Meter Reading Entry: Record previous and current meter readings.

Automatic Bill Calculation: Calculate electricity charges based on consumption slabs.

Penalty Calculation: Calculate additional charges for overdue payments.

Bill Generation: Display a formatted electricity bill with consumer and billing details.

Payment Status: View and update payment status as PAID or UNPAID.

Input Validation: Validate numeric inputs, phone numbers and meter readings.

Data Persistence: Store consumer and billing information using text files.

⚙️ Electricity Tariff Structure

The project implements the following sample slab-based tariff:

Consumption

Rate

First 100 units

₹1.50 per unit

Next 100 units

₹3.00 per unit

Above 200 units

₹5.00 per unit

Late Payment Penalty: ₹5 per overdue day.

Note: These are sample rates implemented for this academic project and are not represented as official electricity tariff rates.

🛠️ Technologies and Concepts Used

C Programming

Structures and Arrays

Functions

Pointers

String Handling

File Handling

Input Validation

Conditional Statements and Loops

📂 Project Structure

Electricity-Bill-Generation-System/
│
├── projectEB.c
├── README.md
└── .gitignore

The application creates and uses the following local data files during execution:

consumers.txt – Stores consumer information.

bills.txt – Stores electricity billing records.

These files are excluded from the public repository to protect consumer information.

▶️ How to Run

Prerequisites

GCC Compiler

Windows, Linux or another GCC-supported environment

Compilation

gcc projectEB.c -o electricity_bill

Execution

Windows PowerShell:

.\electricity_bill.exe

Linux:

./electricity_bill

📋 Application Menu

Consumer Registration

Meter Reading Entry

Bill Calculation

Penalty Calculation

Print Bill

Payment Status

Exit

🎓 Project Information

Project: Electricity Bill Generation System
Language: C
Project Type: Mini Project

Developed as part of learning C programming concepts, particularly structures, functions, input validation and file handling.
