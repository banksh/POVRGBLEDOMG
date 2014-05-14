import sys, pygame

sample = sys.argv[1]
window = (480,400)
blocksize = 8
xbits = 7

drawx = 10
drawy = 20

pygame.init()
surface = pygame.display.set_mode(window)

white = pygame.Color(255, 255, 255)
black = pygame.Color(0, 0, 0)

def load():
	f = open('font_output','r')
	fontTranspose = eval(f.read())
	f.close()
			
	return fontTranspose

font = load()

def render_char(char, pos, size):
	charArray = font[char]
	output_string = ''
	x, y = pos
	tempy = y
	if (x + blocksize > window[0] - 30):
		x = 10
		tempy += size*12
		y = tempy
	for i in charArray:
		for j in i:
			if int(j):
				pygame.draw.rect(surface, white, (x, y, size, size), 0)
			else:
				pygame.draw.rect(surface, black, (x, y, size, size), 0)
			y += size
		y = tempy
		x += size
		output_string += '%s, ' %str(hex(int(i,2)))
	return x+size, y, output_string

def save():
	pygame.image.save(surface, 'output.bmp')

for char in sample:
	drawx, drawy, output_string = render_char(char, (drawx,drawy), 3)
	print output_string
save()
