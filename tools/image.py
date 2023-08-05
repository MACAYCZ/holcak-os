#!/bin/python3
from time import time
from sys import argv
import json

table = []
current = int(time())

class Cell:
	def __init__(self, address, sectors, exclude, flags):
		self.address = address
		self.sectors = sectors
		self.exclude = exclude
		self.flags = flags

def table_init(node, f):
	global table
	f.seek(((f.tell() + 0x1FF) // 0x200) * 0x200)
	if type(node) == str:
		data = open(node, "rb").read()
		table.append(Cell(f.tell() // 0x200, 0x00, 0x00, 0x00))
		table[-1].sectors = (len(data) + 0x1FF) // 0x200
		table[-1].exclude = table[-1].sectors * 0x200 - len(data)
		f.write(data)
	elif type(node) == dict:
		size = 0x04
		address = f.tell()
		data = int(len(node)).to_bytes(4, 'little')
		table.append(Cell(address // 0x200, 0x00, 0x00, 0x01))
		for key, value in node.items():
			if len(key) > 0xFF:
				print("Error: Entry size is above 0xFF!")
				exit(1)
			size += 0x05 + len(key)
		table[-1].sectors = (size + 0x1FF) // 0x200
		table[-1].exclude = table[-1].sectors * 0x200 - size
		f.seek(f.tell() + table[-1].sectors * 0x200)
		for key, value in node.items():
			data += int(len(table)).to_bytes(4, 'little') + int(len(key)).to_bytes(1, 'little')
			data += bytes(key, "ascii")
			table_init(value, f)
		cursor = f.tell()
		f.seek(address)
		f.write(data)
		f.seek(cursor)
	else:
		print("Error: Unknown entry type!")
		exit(1)

def file_size(f):
	cursor = f.tell()
	f.seek(0, 2)
	size = f.tell()
	f.seek(cursor)
	return size

if len(argv) < 3 or (argv[1] == "-i" and len(argv) < 4):
	print(f"Usage: {argv[0]} <flags> <image> [optional]")
	print("\t<flags> -c: <json>: Converts the json to image.")
	print("\t<flags> -p: Prints out information about image.")
	print("Error: Insufficient number of arguments were provided!")
	exit(1)
if argv[1] == "-c":
	f = open(argv[2], "rb+")
	f.seek(0x200)
	table_init(json.loads(open(argv[3], "r").read()), f)
	header = [(f.tell() + 0x1FF) // 0x200, 0x00]
	f.seek(header[0] * 0x200)
	f.write(int(len(table)).to_bytes(4, 'little'))
	for cell in table:
		f.write(int(cell.address).to_bytes(4, 'little'))
		f.write(int(cell.sectors).to_bytes(4, 'little'))
		f.write(int(cell.exclude).to_bytes(2, 'little'))
		f.write(int(cell.flags).to_bytes(2, 'little'))
		f.write(int(current).to_bytes(4, 'little'))
		f.write(int(current).to_bytes(4, 'little'))
	header[1] = (f.tell() + 0x1FF) // 0x200
	f.seek(header[1] * 0x200)
	f.write(int(0x01).to_bytes(4, 'little'))
	f.write(int(header[1] + 0x01).to_bytes(4, 'little'))
	f.write(int(file_size(f) // 0x200).to_bytes(4, 'little'))
	f.seek(0x02)
	f.write(b"HFS1")
	f.write(header[0].to_bytes(4, 'little'))
	f.write(header[1].to_bytes(4, 'little'))
	f.close()
elif argv[1] == "-p":
	f = open(argv[2], "rb")
	f.seek(0x02)
	header = (f.read(0x04).decode("ascii"), int.from_bytes(f.read(0x04), 'little'), int.from_bytes(f.read(0x04), 'little'))
	if header[0] != "HFS1":
		print(f"Error: Magic number `{header[0]}` doesn't match `HFS1`!")
		exit(1)
	print(f"Header:\n\tTable: {header[1]}\n\tFree:  {header[2]}")
	print("Table:")
	f.seek(header[1] * 0x200)
	for i in range(int.from_bytes(f.read(0x04), 'little')):
		print(f"\tCell {hex(i)}:")
		print(f"\t\tAddress: {int.from_bytes(f.read(0x04), 'little')}")
		print(f"\t\tSectors: {int.from_bytes(f.read(0x04), 'little')}")
		print(f"\t\tExclude: {int.from_bytes(f.read(0x02), 'little')}")
		print(f"\t\tFlags: {int.from_bytes(f.read(0x02), 'little')}")
		print(f"\t\tModified: {int.from_bytes(f.read(0x04), 'little')}")
		print(f"\t\tAccessed: {int.from_bytes(f.read(0x04), 'little')}")
	print("Free:")
	f.seek(header[2] * 0x200)
	for i in range(int.from_bytes(f.read(0x04), 'little')):
		print(f"\tCell {hex(i)}:")
		print(f"\t\tStart: {int.from_bytes(f.read(0x04), 'little')}")
		print(f"\t\tEnd: {int.from_bytes(f.read(0x04), 'little')}")
	f.close()
else:
	print(f"Error: Unknown flag `{argv[1]}`!")
	exit(1)
