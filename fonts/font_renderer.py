import pygame, sys
from pygame.locals import *

resolution = (640,480)
blocksize = 7
pixelsize = 50

pygame.init()
clock = pygame.time.Clock()
surface = pygame.display.set_mode(resolution)

white = pygame.Color(255,255,255)
black = pygame.Color(0,0,0)

f = open('fontDict','r')
font = eval(f.read())
f.close()

def render(char):
	charArray = fontTranspose[char]
	x = 0
	for i in charArray:
		y = 0
		for j in i:
			if int(j):
				pygame.draw.rect(surface, white, (x, y, pixelsize, pixelsize), 0)
			else: pygame.draw.rect(surface, black, (x, y, pixelsize, pixelsize), 0)
			y += pixelsize
		x += pixelsize

for j in font:
        A = font[j]
        font[j] = [('0'*(7-len(bin(i)[2:])) + bin(i)[2:]) for i in A[1:-3:2]]

fontTranspose = font.copy()

for i in fontTranspose:
	fontTranspose[i] = [zip(*j) for j in list(fontTranspose[i])]
	fontTranspose[i] = [''.join(list(fontTranspose[i][j][0]))[::-1] for j in range(len(fontTranspose[i]))]

surface.fill(black)
render('A')
keyArray = sorted(fontTranspose.keys())
charIndex = keyArray.index('A')

while True:

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
		

	pygame.display.update()
	clock.tick(60)

