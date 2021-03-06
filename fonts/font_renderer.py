import pygame, sys
from pygame.locals import *

resolution = (640,480)
blocksize = 8
pixelsize = 50

pygame.init()
clock = pygame.time.Clock()
surface = pygame.display.set_mode(resolution)

white = pygame.Color(255,255,255)
black = pygame.Color(0,0,0)
red = pygame.Color(255,40,20)

menu_font = pygame.font.SysFont("monospace",30)

def render(char):
	charArray = fontTranspose[char]
	surface.blit(menu_font.render(char, True, white),(500,100))
	x = 0
	smallx = 150
	smallsize = 3
	for i in charArray:
		y = 0
		smally = 425
		for j in i:
			if int(j):
				pygame.draw.rect(surface, white, (x, y, pixelsize, pixelsize), 0)
				pygame.draw.rect(surface, white, (smallx, smally, smallsize, smallsize), 0)
			else:
				pygame.draw.rect(surface, black, (x, y, pixelsize, pixelsize), 0)
				pygame.draw.rect(surface, black, (smallx, smally, smallsize, smallsize), 0)
			y += pixelsize
			smally += smallsize
		x += pixelsize
		smallx += smallsize

def draw_bit(char, pos, draw=1):
	bitpos = (pos[0] / pixelsize, pos[1] / pixelsize)
	if (bitpos[0] >= xbits) or (bitpos[1] > ybits):
		return
	listBuffer = list(fontTranspose[char][bitpos[0]])
	listBuffer[bitpos[1]] = str(draw)
	fontTranspose[char][bitpos[0]] = ''.join(listBuffer)
	render(char)


class Button:
	clicked = False

	def __init__(self, text, pos, size):
		self.pos = pos
		self.text = text
		self.size = size
		self.render()

	def render(self):
		pygame.draw.rect(surface, red, (self.pos[0],self.pos[1],self.size[0],self.size[1]),0)
		self.textbox = menu_font.render(self.text, True, white)
		self.textpos = self.textbox.get_rect(centerx = self.pos[0]+self.size[0]/2, centery = self.pos[1]+self.size[1]/2)
		surface.blit(self.textbox,self.textpos)

def draw_grid():
	x1 = y1 = y2 = 0
	x2 = pixelsize * (xbits)
	for i in range(xbits+2):
		pygame.draw.line(surface, red, (x1, y2), (x2, y2), 1)
		y2 += pixelsize
	x2 = 0
	y2 -= pixelsize
	for j in range(ybits+1):
		pygame.draw.line(surface, red, (x2, y1), (x2, y2), 1)
		x2 += pixelsize

def save():
	f = open('font_output','w')
	f.write(str(fontTranspose))
	f.close()
	surface.blit(menu_font.render('Saved',True,white), (500, 450))

def revert(char):
	try: revertTrans = load()
	except:
		revertTrans = load_old()
		revertTrans[char] = [zip(*j) for j in list(fontTranspose[char])]
		revertTrans[char] = [''.join(list(fontTranspose[char][j][0]))[::-1] for j in range(len(fontTranspose[char]))]
	fontTranspose[char] = revertTrans[char]
	render(char)

def load_old():
	f = open('fontDict','r')
	font = eval(f.read())
	f.close()
	for j in font:
		A = font[j]
		font[j] = [('0'*(8-len(bin(i)[2:-1])) + bin(i)[2:-1]) for i in A[1:-3:2]]

	fontTranspose = font.copy()

	for i in fontTranspose:
		fontTranspose[i] = [zip(*j) for j in list(fontTranspose[i])]
		fontTranspose[i] = [''.join(list(fontTranspose[i][j][0]))[::-1] for j in range(len(fontTranspose[i]))]
	return fontTranspose

def load():
	f = open('font_output','r')
	fontTranspose = eval(f.read())
	f.close()
	return fontTranspose

try:
	fontTranspose = load()
	print "Loaded saved font file"
except:
	fontTranspose = load_old()
	print "Loaded original font file"

xbits = ybits = blocksize-1

surface.fill(black)
render('A')
keyArray = sorted(fontTranspose.keys())
charIndex = keyArray.index('A')

savebutton = Button('Save', (500,400), (125,50))
revertbutton = Button('Revert', (365,400), (125,50))

while True:

	if (savebutton.textpos.collidepoint(pygame.mouse.get_pos()) and pygame.mouse.get_pressed()[0]):
		save()
	if (revertbutton.textpos.collidepoint(pygame.mouse.get_pos()) and pygame.mouse.get_pressed()[0]):
		revert(keyArray[charIndex])
	
	for event in pygame.event.get():
		if event.type == QUIT:
			pygame.quit()
			sys.exit()
		elif event.type == KEYDOWN:
			if event.key == K_RIGHT:
				if (charIndex <=  len(fontTranspose.keys())-2):
					surface.fill(black)
					charIndex += 1
					render(keyArray[charIndex])
			elif event.key == K_LEFT:
				if (charIndex >= 1):
					surface.fill(black)
					charIndex -= 1
					render(keyArray[charIndex])
	
	if (pygame.mouse.get_pressed()[0]): draw_bit(keyArray[charIndex], pygame.mouse.get_pos())
	if (pygame.mouse.get_pressed()[2]): draw_bit(keyArray[charIndex], pygame.mouse.get_pos(), draw=0)	


	draw_grid()
	savebutton.render()
	revertbutton.render()
	pygame.display.update()
	clock.tick(60)

