# InvenTrack

A terminal-based inventory and user management system written in C.

## Features
- Role-based access (Admin, Manager, Employee)
- Secure password entry
- Inventory tracking
- File-based persistence

## How to Compile
Use the provided `Makefile` or run:

```bash
gcc -o InvenTrack InvenTrack.c user.c inventory.c menus.c utils.c
