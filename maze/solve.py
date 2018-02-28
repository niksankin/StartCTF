import time
import socket

class Cell:
	def __init__(self,x,y,val):
		self.x = x
		self.y = y
		self.val = val

	def getX(self):
		return self.x

	def getY(self):
		return self.y

	def getValue(self):
		return self.val

def getFree(point, maze, visited):
	x = point.getX()
	y = point.getY()

	neighbours = [Cell(x,y+1,maze[y+1][x]), Cell(x,y-1,maze[y-1][x]), Cell(x+1,y,maze[y][x+1]), Cell(x-1,y,maze[y][x-1])]
	for point in neighbours:
		if((point.getValue() == ' ' or point.getValue() == 'f') and (point.getX(), point.getY()) not in visited):
			return point
	return 0

sock = socket.socket()
sock.connect(('127.0.0.1',5500))

sock.recv(1024)
data = sock.recv(16392).decode()
maze = data.split('\n')
for i in range(len(maze)):
	maze[i] = list(maze[i])

visited = []
stack = []
current = Cell(1,1,maze[1][1])

while(1):
	if(current.getX() == 119 and current.getY() == 119):
		stack.append(current)
		print('end')
		break

	new = getFree(current, maze, visited)
	visited.append((current.getX(), current.getY()))
	
	if(new):
		stack.append(current)
		current = new
	else:
		current = stack.pop()

for point in stack:
	maze[point.getY()][point.getX()] = '#'

answer = ''

for i in maze:
	answer += ''.join(i + ['\n'])

print(answer)

answer = ""

for i in range(1,len(stack)):
	if(stack[i].getX()<stack[i-1].getX()):
		answer += 'l'
	elif(stack[i].getX()>stack[i-1].getX()):
		answer += 'r'
	elif(stack[i].getY()<stack[i-1].getY()):
		answer += 'u'
	elif(stack[i].getY()>stack[i-1].getY()):
		answer += 'd'

sock.send(answer.encode())
print(sock.recv(1024))
sock.close()