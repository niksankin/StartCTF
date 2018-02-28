import sys
linenum = 0

def getOpcode(code,line):
	global linenum

	if(line[1] == 'b'):
		t = 0
	elif(line[1] == 's'):
		t = 1
	elif(line[1] == 'i'):
		t = 2
	elif(line[1] == 'str'):
		t = 3
	elif(':' in line[1]):
		t = 4
	else:
		print("Invalid type at line {}".format(linenum))
		exit(1)

	data = bytearray()
	data.append(code)
	data.append(t)
	if(t == 0):
		data.append(int(line[2]))
	elif(t == 1):
		data.append(int(line[2])//0x100)
		data.append(int(line[2])%0x100)
	elif(t == 2):
		data.append(int(line[2])//0x1000000)
		data.append(int(line[2])//0x10000 - (int(line[2])//0x1000000) * 0x100)
		data.append(int(line[2])//0x100 - (int(line[2])//0x10000) * 0x100)
		data.append(int(line[2])%0x100)
	elif(t == 4):
		if("//" in line[1]):
			line[1] = line[1].split("//")[0]

		var = int(line[1].split(':')[0])
		t = int(line[1].split(':')[1])
		index = int(line[1].split(':')[2])
		if(line[2] == 'b'):
			vartype = 0
		elif(line[2] == 'c'):
			vartype = 1
		else:
			print("Unexpected vartype at line {}".format(linenum))
			exit(1)

		if(t not in (0,1)):
			print("Invalid offset type at line {}".format(linenum))
			exit(1)

		data.append(vartype)
		data.append(var)
		data.append(t)
		data.append(index)
	else:
		for i in range(2,len(line)):
			if("//" in line[i]):
				if(data[-1] == ord(' ')):
					data.pop()
				data.append(0)
				break
			elif(line[i] == 'x20'):
				data.append(ord(' '))
				data.append(0)
			else:	
				for j in line[i]:
					data.append(ord(j))
				if(i != len(line) - 1):
					data.append(ord(' '))
				else:
					data.append(0)
	return data

if __name__=="__main__":	
	if(len(sys.argv) != 3):
		print("Usage: compiler.pyc codefile bytefile")
		exit(0)
	bytefile = open(sys.argv[2], "wb")
	code = open(sys.argv[1], "r")

	bytefile.write(b"\xde\xad")

	while(1):
		line = code.readline().split(' ')
		linenum += 1
		for i in range(len(line)):
			line[i] = line[i].replace('\n', '')

		if(line[0] == "store"):
			bytefile.write(getOpcode(1,line))
		else:
			break

	bytefile.write(b"\xbe")

	while(1):
		if(line[0] == "jmp"):
			bytefile.write(b"\x00")
		elif(line[0] == "init"):
			bytefile.write(b"\x01")
		elif(line[0] == "push"):
			bytefile.write(getOpcode(2,line))
		elif(line[0] == "pushv"):
			bytefile.write(getOpcode(3,line))
		elif(line[0] == "pop"):
			bytefile.write(getOpcode(4,line))
		elif(line[0] == "add"):
			bytefile.write(b"\x05")
		elif(line[0] == "sub"):
			bytefile.write(b"\x06")
		elif(line[0] == "xor"):
			bytefile.write(b"\x07")
		elif(line[0] == "addv"):
			bytefile.write(b"\x08")
		elif(line[0] == "subv"):
			bytefile.write(b"\x09")
		elif(line[0] == "xorv"):
			bytefile.write(b"\x0a")
		elif(line[0] == "inp"):
			bytefile.write(b"\x0b")
		elif(line[0] == "out"):
			bytefile.write(b"\x0c")
		elif(line[0] == "jz"):
			bytefile.write(b"\x0d")
		elif(line[0] == "jnz"):
			bytefile.write(b"\x0e")
		elif(line[0] == "ja"):
			bytefile.write(b"\x0f")
		elif(line[0] == "jb"):
			bytefile.write(b"\x10")
		elif(line[0] == "je"):
			bytefile.write(b"\x11")
		elif(line[0] == "jne"):
			bytefile.write(b"\x12")
		elif(line[0] == "exit"):
			bytefile.write(b"\xef")
			break
			
		line = code.readline().split(' ')
		linenum += 1

		for i in range(len(line)):
			line[i] = line[i].replace('\n', '')

	bytefile.close()
	code.close()