#!/bin/bash

qemu-system-i386 -S -s -kernel xinu.elf -vnc :2 -hda ../fs_util/fs.img
