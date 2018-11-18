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

### Credits:
  https://dmunsie.wordpress.com/code/hacks/
    for their (mac, not linux) disable journal code.

  and "blink" https://ubuntuforums.org/showthread.php?t=1420673#post_10303113
  shows how to get that to work on linux

  And Mac Documentation on HFS+: https://developer.apple.com/library/archive/technotes/tn/tn1150.html#//apple_ref/doc/uid/DTS10002989