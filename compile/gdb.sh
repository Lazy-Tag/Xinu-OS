#!/bin/bash

qemu-system-i386 -nographic -serial mon:stdio -S -s -kernel xinu.elf
