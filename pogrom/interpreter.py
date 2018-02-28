import sys

def new():
	global eip
	global variables
	global line
	global ops
	global bytefile

	eip += 1
	var = 0

	if(ops[eip] == 0x00):
		eip += 1
		var = ops[eip]
	elif(ops[eip] in (0x01, 0x02)):
		for i in range(ops[eip] * 2 - 1, -1, -1):
			eip += 1
			var += ops[eip] * pow(256, i)
	elif(ops[eip] == 0x03):
		var = []
		eip += 1

		while(ops[eip] != 0):
			var.append(chr(ops[eip]))
			eip += 1

		var.append('\n')
		var = ''.join(var)
	elif(ops[eip] == 0x04):
		eip += 1
		vartype = ops[eip]

		eip += 1

		if(ops[eip] not in variables.keys()):
			print "Undefined variable {} at line {}".format(ops[eip], line)
			bytefile.close()
			exit(1)
		elif(type(variables[ops[eip]]) != type('')):
			print "Mismatch type at line {}".format(ops[eip], line)
			bytefile.close()
			exit(1)
		
		eip += 2
		if(ops[eip-1] == 0x00):
			index = ops[eip]
		elif(ops[eip-1] == 0x01):
			index = variables[ops[eip]]
		if(vartype):
			var = variables[ops[eip-2]][index]
		else:
			var = ord(variables[ops[eip-2]][index])
			
	else:
		print "Parsing error at line {}".format(line)
		bytefile.close()
		exit(1)

	return var

def jmp(mode=0):
	global stack
	global eip
	global code_section

	addr = stack.pop()

	if(mode==1):
		arg = stack.pop()
		if(arg):
			return 0
	elif(mode == 2):
		arg = stack.pop()
		if(not arg):
			return 0
	elif(mode in (3,4,5,6)):
		sec = stack.pop()
		fst = stack.pop()
		if(mode == 3 and fst <= sec or
			mode == 4 and fst >= sec or
			mode == 5 and fst != sec or
			mode == 6 and fst == sec):
			return 0

	eip = code_section + addr - 1

def init():
	global stack
	global eip
	global variables

	val = stack.pop()

	if(len(variables) > 0x100000000):
		print "Variables limit!"
		return 0

	variables.update({len(variables):val})

def push(mode=0):
	global stack
	global eip
	global variables
	global line
	global bytefile

	arg = new()
	if(mode):
		if(arg not in variables.keys()):
			print "Undefined variable {} at line {}".format(arg, line)
			bytefile.close()
			exit(1)
		else:
			stack.append(variables[arg])
	else:
		stack.append(arg)

def pop():
	global stack
	global eip
	global variables
	global bytefile
	global line

	arg = new()

	if(arg not in variables.keys()):
		print "Undefined variable {} at line {}".format(arg, line)
		bytefile.close()
		exit(1)

	variables[arg] = stack.pop()

def calc(mode=0, action=0):
	global stack
	global eip
	global variables
	global bytefile
	global line

	obj = stack.pop()

	if(obj not in variables.keys()):
		print "Undefined variable {} at line {}".format(obj, line)
		bytefile.close()
		exit(1)

	if(mode):
		index = stack.pop()
		if(index not in variables.keys()):
			print "Undefined variable {} at line {}".format(index, line)
			bytefile.close()
			exit(1)
		else:
			val = variables[index]
	else:
		val = stack.pop()

	if(type(variables[obj]) != type(val)):
		print "Mismatch types at line {}".format(obj, line)
		bytefile.close()
		exit(1)

	if(type(variables[obj]) == type('') or type(val) == type('')):
		print "Calculations are not defined for strings at line {}".format(obj, line)
		bytefile.close()
		exit(1)
	
	if(action == 0):
		variables[obj] += val
	elif(action == 1):
		variables[obj] -= val
	elif(action == 2):
		variables[obj] ^= val

def inp():
	global stack
	global eip
	global variables
	global bytefile
	global line

	obj = stack.pop()
	if(obj not in variables.keys()):
		print "Undefined variable {} at line {}".format(obj, line)
		bytefile.close()
		exit(1)

	mes = stack.pop()

	val = raw_input(mes)

	if(len(val) > 0x100):
		print "My dumb language supports only 256-char length strings("
		bytefile.close()
		exit(1)
	elif(not len(val)):
		val = '\n'

	variables[obj] = val+'\n'

def out():
	global stack
	global eip
	global variables
	global bytefile
	global line

	obj = stack.pop()
	print obj,

if __name__ == "__main__":
	if(len(sys.argv) != 2):
		print "Usage: interpreter.pyc bytefile"
		exit(0)

	bytefile = 0
	eip = 0
	stack = []
	variables = {}
	code_section = 0
	ops = []
	line = 0

	bytefile = open(sys.argv[1], "rb")

	ops = map(ord,bytefile.read())

	if(ops[0] != 0xde or ops[1] != 0xad):
		print "Bad magic value!"
		bytefile.close()
		exit()

	eip += 2

	while(1):
		if(ops[eip] == 0x01):
			line += 1
			var = new()
			variables.update({len(variables):var})
			eip += 1
		elif(ops[eip] == 0xbe):
			break
		else:
			print "Broken data section!"
			bytefile.close()
			exit()

	code_section = eip + 1
	eip += 1

	while(1):
		line += 1
		if(ops[eip] == 0x00):
			jmp()
		elif(ops[eip] == 0x01):
			init()
		elif(ops[eip] == 0x02):
			push()
		elif(ops[eip] == 0x03):
			push(mode=1)
		elif(ops[eip] == 0x04):
			pop()
		elif(ops[eip] == 0x05):
			calc()
		elif(ops[eip] == 0x06):
			calc(action=1)
		elif(ops[eip] == 0x07):
			calc(action=2)
		elif(ops[eip] == 0x08):
			calc(mode=1)
		elif(ops[eip] == 0x09):
			calc(mode=1, action=1)
		elif(ops[eip] == 0x0a):
			calc(mode=1, action=2)
		elif(ops[eip] == 0x0b):
			inp()
		elif(ops[eip] == 0x0c):
			out()
		elif(ops[eip] == 0x0d):
			jmp(1)
		elif(ops[eip] == 0x0e):
			jmp(2)
		elif(ops[eip] == 0x0f):
			jmp(3)
		elif(ops[eip] == 0x10):
			jmp(4)
		elif(ops[eip] == 0x11):
			jmp(5)
		elif(ops[eip] == 0x12):
			jmp(6)
		elif(ops[eip] == 0xef):
			bytefile.close()
			exit(0)
		else:
			print "Unexpected opcode at line {}".format(line) 
			exit(1)

		eip += 1