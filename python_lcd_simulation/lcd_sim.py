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

        self.key_states_indexes = { 
            "up" : 0,
            "down" : 1,
            "left" : 2,
            "right" : 3,
            "enter" : 4,
            "back" : 5
        }

        self.key_states = [0] * len(self.key_states_indexes)

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
                            pygame.K_SPACE : "enter",
                            pygame.K_b     : "back"}
 
        #print(event)

        is_pressed = event.type == pygame.KEYDOWN
        if event.key in registered_keys:
            self.setKeyState(registered_keys[event.key], is_pressed)

    def terminate(self):
        self.run = False

    def isRunning(self):
        return self.run

    def setKeyState(self, key, is_pressed):
        if key in self.key_states_indexes.keys():
            self.key_states[self.key_states_indexes[key]] = is_pressed

    def getKeyState(self):
        return self.key_states

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
                if event.type in (pygame.KEYDOWN, pygame.KEYUP):
                    #if event.key == pygame.K_a:
                        #frame_rate += 1
                    #if event.key == pygame.K_s:
                        #frame_rate -= 1
    
                    #if frame_rate < 1:
                    #    frame_rate = 1
                    #print(frame_rate)
    
                    self.evaluate_keyboard_input_event(event)
    
            self.draw(game_display)
    
            pygame.display.update()
            clock.tick(frame_rate)
    
        pygame.quit()
        #exit()
 
if __name__ == "__main__":
    lcdScreen = LcdScreen()
    lcdScreen.start_game()
 