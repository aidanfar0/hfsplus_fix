# HFS+ Fix
Simple program to make HFS+ Volumes Read-Write mountable on linux.

## WARNING
This program disables the journal, which may result in data loss!

### Compiling:
  `make`

### Usage:
  `sudo ./hfsplus_fix /dev/sdX`

![Screenshot](https://raw.githubusercontent.com/aidanfar0/hfsplus_fix/master/screenshot.png)

### What it does:
 - Disables the Journal
 - Sets the 'unmounted' bit
 - Sets the 'lastMountVersion' to H+Lx or your own input
 - Zero's the Journal's block number

This should make it mountable as read-write on linux using hfsutils/hfsprog