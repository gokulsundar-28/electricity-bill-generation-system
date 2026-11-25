# Electricity Bill Generation System (C)

A console-based Electricity Bill Generation System written in pure C. It manages consumers, meter readings, slab-based bill calculation, late-payment penalties and payment tracking, with all records saved to plain text files so data survives between runs.

## Features

### 1. Consumer Management
- Register consumers with a unique **Consumer ID**, name, address and 10-digit phone number
- Duplicate IDs are rejected; IDs must be positive numbers
- Capacity of **50 consumers**, with a clear message when the list is full
- Every new consumer starts with a clean, empty billing record

### 2. Billing Workflow
- **Meter Reading Entry**: record previous and current readings; units consumed are computed automatically
- **Bill Calculation**: slab-based charges applied to the units consumed
- **Penalty Calculation**: Rs. 5 per overdue day added to the bill
- **Print Bill**: formatted bill with consumer details, readings, charges and payment status
- **Payment Status**: view the status and mark a bill as PAID

### 3. Enforced Order of Operations
The program does not let steps be skipped. Each stage checks that the previous one is complete:
- A bill cannot be calculated before a meter reading is entered
- A penalty cannot be applied or a bill printed before the bill is calculated
- A paid bill cannot be recalculated or penalised

### 4. Defensive Input Handling and Data Integrity
- Custom input functions (`readInt`, `readFloat`, `readText`) re-prompt on invalid input instead of crashing
- Leftover characters in the input buffer are flushed after every read, which prevents infinite loops
- Text fields reject empty input, over-long input and the `|` character (used as a file separator)
- Data is saved to disk after every change, so nothing is lost if the program is closed

## Business Rules

| Rule | Value |
|------|-------|
| Maximum consumers | 50 |
| Consumer ID | Positive integer, unique |
| Name length | Up to 49 characters |
| Address length | Up to 99 characters |
| Phone number | Exactly 10 digits |
| Meter readings | Cannot be negative; current reading must be at least the previous reading |
| Late penalty | Rs. 5 per overdue day |
| Total amount | Bill Amount + Penalty |

### Tariff Slabs

| Units Consumed | Rate per Unit |
|----------------|---------------|
| First 100 units (1 – 100) | Rs. 1.50 |
| Next 100 units (101 – 200) | Rs. 3.00 |
| Above 200 units | Rs. 5.00 |

**Example:** 250 units consumed
- First 100 units: 100 × 1.50 = Rs. 150
- Next 100 units: 100 × 3.00 = Rs. 300
- Remaining 50 units: 50 × 5.00 = Rs. 250
- **Bill Amount = Rs. 700**

## Getting Started

### Requirements
- A C compiler such as **GCC** (MinGW on Windows, GCC on Linux/macOS)
- A terminal (Command Prompt, PowerShell, or any Linux/macOS terminal)

### Compile and Run

```bash
gcc projectEB.c -o projectEB
./projectEB          # Linux / macOS
projectEB.exe        # Windows
```

> Run the program from the folder where you want the data files to be stored. They are created in the current working directory.

### Project Structure

```
Electricity-Bill-Generation-System/
├── projectEB.c      # Complete source code
├── README.md        # Project documentation
├── consumers.txt    # Auto-generated: consumer records
└── bills.txt        # Auto-generated: billing records
```

## Usage

When the program starts, it shows the main menu:

```
====== ELECTRICITY BILL GENERATION SYSTEM ======
1. Consumer Registration
2. Meter Reading Entry
3. Bill Calculation
4. Penalty Calculation
5. Print Bill
6. Payment Status
7. Exit
Enter your choice:
```

### Recommended Flow for Each Consumer

| Step | Menu Option | What Happens |
|------|-------------|--------------|
| 1 | Consumer Registration | Enter ID, name, address and phone number |
| 2 | Meter Reading Entry | Enter previous and current readings; units are calculated |
| 3 | Bill Calculation | Slab charges are applied to the units |
| 4 | Penalty Calculation *(optional)* | Enter days overdue; penalty and total are updated |
| 5 | Print Bill | A formatted bill is displayed |
| 6 | Payment Status | Mark the bill as PAID once payment is received |

> Entering a new meter reading starts a fresh billing cycle: the amount, penalty, total and payment status are reset for that consumer.

### Sample Bill Output

```
==================================
         ELECTRICITY BILL
==================================
Consumer ID     : 101
Name            : Ravi Kumar
Address         : 12 Gandhi Street, Chennai
Phone           : 9876543210
----------------------------------
Previous Reading: 1200.00
Current Reading : 1450.00
Units Consumed  : 250.00
----------------------------------
Bill Amount     : Rs. 700.00
Penalty         : Rs. 15.00
Total Amount    : Rs. 715.00
----------------------------------
Payment Status  : UNPAID
==================================
```

## Input Validation

| Input | Validation |
|-------|------------|
| Menu choice | Must be a number from 1 to 7 |
| Numeric fields | Non-numeric input is rejected and the user is asked again |
| Consumer ID | Must be positive and not already registered |
| Name / Address | Cannot be empty, cannot exceed the length limit, cannot contain `\|` |
| Phone number | Exactly 10 digits, numbers only |
| Previous reading | Cannot be negative |
| Current reading | Cannot be less than the previous reading |
| Days overdue | Cannot be negative (enter 0 if not late) |

## How It Works

### Data Storage

All records are stored in two plain text files, created automatically on first use. They are loaded when the program starts and rewritten after every change.

| File | Format | Example |
|------|--------|---------|
| `consumers.txt` | `ID\|Name\|Address\|Phone` | `101\|Ravi Kumar\|12 Gandhi Street, Chennai\|9876543210` |
| `bills.txt` | Nine space-separated values, one line per consumer | `1200.00 1450.00 250.00 700.00 15.00 715.00 1 1 0` |

The fields in `bills.txt` appear in this order:

```
previous  current  units  amount  penalty  total  readingEntered  calculated  paid
```

Line *n* of `bills.txt` belongs to consumer *n* of `consumers.txt`.

### Billing Status Flags

Each bill carries three flags that control the order of operations:

| Flag | Set to 1 when |
|------|---------------|
| `readingEntered` | Meter reading has been recorded |
| `calculated` | Bill amount has been computed |
| `paid` | Payment has been marked as received |

### Data Structures

```c
struct Consumer {
    int  id;
    char name[50], address[100], phone[11];
};

struct Bill {
    float previous, current, units, amount, penalty, total;
    int   readingEntered, calculated, paid;
};
```

Both are stored in global arrays of size `MAX` (50), linked by array index.

## Code Overview

| Function | Purpose |
|----------|---------|
| `clearBuffer()` | Flushes leftover input after a read |
| `readInt()` / `readFloat()` | Safe numeric input with re-prompting |
| `readText()` | Safe text input with length, empty and `\|` checks |
| `isValidPhone()` | Checks for exactly 10 digits |
| `loadData()` | Reads both data files into memory at startup |
| `saveData()` | Writes both data files to disk |
| `findConsumer()` | Returns the array index for a Consumer ID, or -1 |
| `registerConsumer()` | Adds a new consumer with an empty bill |
| `enterMeterReading()` | Records readings and calculates units |
| `calculateBill()` | Applies the tariff slabs |
| `calculatePenalty()` | Adds the late-payment penalty |
| `printBill()` | Displays the formatted bill |
| `paymentStatus()` | Shows and updates the payment status |
| `main()` | Menu loop |

## Limitations and Future Improvements

- Consumer records cannot yet be edited or deleted
- No option to list all consumers at once
- Only the latest bill per consumer is stored; there is no billing history
- The previous reading is entered manually rather than taken from the last bill
- Penalty days are entered by the user instead of being calculated from a due date

## Technologies Used

- C programming language
- Structures and arrays
- File handling (text files)
- Standard libraries: `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`

## Author
**Gokul Sundar M**
