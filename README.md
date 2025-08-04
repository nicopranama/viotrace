# Viotrace – Student Violation Tracker using AVL Tree

**Viotrace** is a console-based student violation tracking system written in C.  
It uses an **AVL Tree** data structure to efficiently store, search, and manage violation data, including automatic drop-out logic when students exceed penalty limits.

---

## Overview

Viotrace helps administrators:
- Register students with unique NIM and name
- Record student violations (type, date, points)
- Automatically "drop out" students who exceed 100 violation points
- Move dropped students into a separate "inactive" AVL tree
- View and search both active and inactive students
- Sort violation histories by date

---

## Technologies

- Language: C
- Data Structures: AVL Tree, Linked List
- Input handling: `scanf`, `fgets`, `strcmp`, `toupper`, etc.
- Sorting: Custom merge sort by date
- Platform: Console / Terminal

---

## Features

- Add and manage student data
- Input multiple violations per student
- Auto-drop mechanism after 100+ points
- Delete students manually or via drop rule
- View full violation history
- Date format: `DD-MM-YYYY`
- Fast search by NIM or name

---

## How to Run

1. Compile the program:
   ```bash
   gcc SistemManajemenPelanggaran.c -o viotrace
