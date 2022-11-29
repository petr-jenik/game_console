import pygame
import copy
import random
 
RESOLUTION_X = 600
RESOLUTION_Y = 300
 

class LcdScreen():
    def __init__(self, lcd_width = 84, lcd_height = 48):

        self.LCD_WIDTH = lcd_width
        self.LCD_HEIGHT = lcd_height
        
        self.BLOCK_SIZE_X = RESOLUTION_X // self.LCD_WIDTH
        self.BLOCK_SIZE_Y = RESOLUTION_Y // self.LCD_HEIGHT

        print("DEBUG: ", self.LCD_WIDTH, self.LCD_HEIGHT)

        #self.pixels = self.LCD_WIDTH*[[0] * self.LCD_HEIGHT]
        self.pixels = [ [0]*self.LCD_HEIGHT for i in range(self.LCD_WIDTH)]
        self.x = 0
        self.y = 0
        self.run = True

    def setBuffer(self, message):
        #print("Len: ", len(message))
        for x in range(self.LCD_WIDTH):
            for y in range(self.LCD_HEIGHT):

                message_index = x + (y // 8) * self.LCD_WIDTH

                #//index = x * self.LCD_HEIGHT + y
                #//message_index = index // 8
                bit_index = y % 8
                #print(index, x, y, self.LCD_WIDTH, self.LCD_HEIGHT)
                #print(message[index])
                #print(type(message[index]))
                self.pixels[x][y] = ((1 << bit_index) & message[message_index]) != 0

    def setPixel(self, x,y, value):
        #print(x, y)
        #self.pixels = self.LCD_WIDTH * copy.deepcopy([0] * self.LCD_HEIGHT)]
        #print(self.pixels)
        #self.pixels[x][y]
 
        #self.pixels = [ [0]*self.LCD_HEIGHT for i in range(self.LCD_WIDTH)]

        if x in range(self.LCD_WIDTH):
            if y in range(self.LCD_HEIGHT):
                self.pixels[x][y] = value
 
    def draw_rect(self, screen, x, y, color):
        #print(x * self.BLOCK_SIZE_X, y * self.BLOCK_SIZE_Y, self.BLOCK_SIZE_X, self.BLOCK_SIZE_Y)
        rect = pygame.Rect(x * self.BLOCK_SIZE_X, y * self.BLOCK_SIZE_Y, self.BLOCK_SIZE_X - 1, self.BLOCK_SIZE_Y - 1)
        pygame.draw.rect(screen, color, rect)
 
    def draw(self, screen):
        screen.fill(pygame.Color("green"))
 
        for x in range(self.LCD_WIDTH):
            for y in range(self.LCD_HEIGHT):
                if (self.pixels[x][y] != 0):
                    self.draw_rect(screen, x, y, pygame.Color("black"))
 
 
 
 
    def evaluate_keyboard_input_event(self, event):
        registered_keys = { pygame.K_LEFT  : "left", 
                            pygame.K_RIGHT : "right",
                            pygame.K_UP    : "up",
                            pygame.K_DOWN  : "down",
                            pygame.K_p     : "pause"}
 
        #print(event)
        if event.type == pygame.KEYDOWN and event.key in registered_keys.keys():        

            if event.key == pygame.K_LEFT:
                if (self.x > 0):
                    self.x -= 1
            if event.key == pygame.K_RIGHT:
                if (self.x < self.LCD_WIDTH - 1):
                    self.x += 1
            if event.key == pygame.K_UP:
                if (self.y > 0):
                    self.y -= 1
            if event.key == pygame.K_DOWN:
                if (self.y < self.LCD_HEIGHT - 1):
                    self.y += 1
            if event.key == pygame.K_p:
                # Do not move when paused
                pass
            
            print(self.x, self.y)
            self.setPixel(self.x,self.y, 1)

    def terminate(self):
        self.run = False

    def isRunning(self):
        return self.run

    def start_game(self):
        pygame.init()
    
        game_display = pygame.display.set_mode((RESOLUTION_X, RESOLUTION_Y))
        pygame.display.set_caption("LCD simulator")
        clock = pygame.time.Clock()
    
        #lcdScreen = LcdScreen()
    
        self.run = True
    
        frame_rate = 60
    

        while self.run:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.run = False
    
                # Update game speed
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_a:
    
                        frame_rate += 1
                    if event.key == pygame.K_s:
                        frame_rate -= 1
    
                    if frame_rate < 1:
                        frame_rate = 1
                    print(frame_rate)
    
                    self.evaluate_keyboard_input_event(event)
    
            self.draw(game_display)
    
            pygame.display.update()
            clock.tick(frame_rate)
    
        pygame.quit()
        #exit()
 
if __name__ == "__main__":
    lcdScreen = LcdScreen()
    lcdScreen.start_game()
 