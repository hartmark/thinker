#!/usr/bin/python

import re
import sys
import argparse

patches = [
    [0x680A1E, 0x53, 0x54],
    [0x680A20, 0x65, 0x69],
    [0x680A21, 0x6C, 0x6E],
    [0x680A22, 0x6C, 0x6B],
    [0x680A23, 0x45, 0x65],
    [0x680A24, 0x78, 0x72],
    [0x680A25, 0x65, 0x4D],
    [0x680A26, 0x63, 0x6F],
    [0x680A27, 0x75, 0x64],
    [0x680A28, 0x74, 0x75],
    [0x680A29, 0x65, 0x6C],
    [0x680A2A, 0x41, 0x65],
    [0x680A2C, 0x53, 0x74],
    [0x680A2D, 0x48, 0x68],
    [0x680A2E, 0x45, 0x69],
    [0x680A2F, 0x4C, 0x6E],
    [0x680A30, 0x4C, 0x6B],
    [0x680A31, 0x33, 0x65],
    [0x680A32, 0x32, 0x72],
    [0x6EF4FC, 0x53, 0x74],
    [0x6EF4FD, 0x48, 0x68],
    [0x6EF4FE, 0x45, 0x69],
    [0x6EF4FF, 0x4C, 0x6E],
    [0x6EF500, 0x4C, 0x6B],
    [0x6EF501, 0x33, 0x65],
    [0x6EF502, 0x32, 0x72],
    [0x6EF50A, 0x53, 0x54],
    [0x6EF50C, 0x65, 0x69],
    [0x6EF50D, 0x6C, 0x6E],
    [0x6EF50E, 0x6C, 0x6B],
    [0x6EF50F, 0x45, 0x65],
    [0x6EF510, 0x78, 0x72],
    [0x6EF511, 0x65, 0x4D],
    [0x6EF512, 0x63, 0x6F],
    [0x6EF513, 0x75, 0x64],
    [0x6EF514, 0x74, 0x75],
    [0x6EF515, 0x65, 0x6C],
    [0x6EF516, 0x41, 0x65],
]

parser = argparse.ArgumentParser()
parser.add_argument('--diff', default=False, 
    action='store_true', help='process input data as binary diff')
parser.add_argument('--revert', action='store_true', help='output revert patch commands')
parser.add_argument('--base', default='400000', help='module image base offset')
args = parser.parse_args()
base = int(args.base, 16)
revert = args.revert
locations = set()

for line in sys.stdin:
    if args.diff:
        m = re.search(r"^(\w+): (\w\w) (\w\w)", line)
        if m:
            addr = int(m.group(1), 16) + base
            byte = m.group(2) if revert else m.group(3)
            sys.stdout.write('PatchByte(0x%X, 0x%s);\n' % (addr, byte))
    else:
        m = re.search(r"{ *0x(\w+), *0x(\w\w), *0x(\w\w) *}", line)
        if m:
            addr = int(m.group(1), 16)
            byte = m.group(2) if revert else m.group(3)
            sys.stdout.write('PatchByte(0x%X, 0x%s);\n' % (addr, byte))
            locations.add(addr)

if not args.diff:
    # Patch also DLL import tables
    for line in patches:
        sys.stdout.write('PatchByte(0x%06X, 0x%02X);\n' % (line[0], line[1] if revert else line[2]))
        locations.add(line[0])
    assert(len(locations) == 3165), 'Patching data length mismatch'


