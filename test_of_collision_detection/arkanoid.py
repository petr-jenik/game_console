import pygame
import copy
import random
 
RESOLUTION_X = 600
RESOLUTION_Y = 300

wall_width = RESOLUTION_X // 30

class DrawObject():
    def __init__(self, x, y, size_x, size_y, v_x, v_y, breakable = True):
        self.x = x
        self.y = y
        self.size_x = size_x
        self.size_y = size_y
        self.v_x = v_x
        self.v_y = v_y
        self.float_x = x
        self.float_y = y
        self.visible = True
        self.breakable = breakable

        self.material = 1

    def update(self):
        self.float_x += self.v_x
        self.float_y += self.v_y

        self.x = int(self.float_x)
        self.y = int(self.float_y)

def solve_collision(ball, static_box):
    if static_box.visible == False:
        return

    going_from_left = False
    going_from_right = False
    going_from_top = False
    going_from_bottom = False

    # Ball is not touching static box, but it will in next update - going from left side of static_box
    if ((ball.x + ball.size_x) < static_box.x) and ((ball.x + ball.size_x + ball.v_x) >= static_box.x):
        # It it going to cross
        #ball.v_x = -ball.v_x
        going_from_left = True
        #print("going_from_left")

    # Ball is not touching static box, but it will in next update - going from right side of static_box
    if (ball.x > (static_box.x + static_box.size_x)) and ((ball.x + ball.v_x) <=  (static_box.x + static_box.size_x)):
        # It it going to cross
        #ball.v_x = -ball.v_x
        going_from_right = True
        #print("going_from_right")

    # Ball is not touching static box, but it will in next update - going from top of the static_box
    if ((ball.y + ball.size_y) < static_box.y) and ((ball.y + ball.size_y + ball.v_y) >= static_box.y):
        # It it going to cross
        #ball.v_y = -ball.v_y
        going_from_top = True
        #print("going_from_top")

    # Ball is not touching static box, but it will in next update - going from the bottom side of the static_box
    if (ball.y > (static_box.y + static_box.size_y)) and ((ball.y + ball.v_y) <= (static_box.y + static_box.size_y)):
        # It it going to cross
        #ball.v_y = -ball.v_y
        going_from_bottom = True
        #print("going_from_bottom")

    """
    if going_from_left and going_from_top or \
       going_from_right and going_from_top or \
       going_from_left and going_from_bottom or \
       going_from_right and going_from_bottom:
            ball.v_x = -ball.v_x
            ball.v_y = -ball.v_y
            if static_box.breakable:
                static_box.material = max(static_box.material - 1, 0)
                if static_box.material == 0:
                    static_box.visible = False
            print("Corner!")
            return
    """
    if going_from_left or going_from_right:
        middle_point_1 = ball.y + ball.v_y + ball.size_y//2
        middle_point_2 = static_box.y + static_box.size_y//2
        min_distance = abs(ball.size_y + static_box.size_y) // 2
        distance = abs(middle_point_1 - middle_point_2)
        if distance <= min_distance:
            ball.v_x = -ball.v_x
            print("bounce X")

            if static_box.breakable:
                static_box.material = max(static_box.material - 1, 0)
                if static_box.material == 0:
                    static_box.visible = False
    
        #if len(set(range(ball.y, ball.y + ball.size_y)) & set(range(static_box.y, static_box.y + static_box.size_y))) != 0:
        #if ball.y in range(static_box.y, static_box.y + static_box.size_y) or \
        #(ball.y + ball.size_y) in range(static_box.y, static_box.y + static_box.size_y):
        #if (ball.y >= static_box.y and ball.y <= static_box.y + static_box.size_y):

    if going_from_top or going_from_bottom:
        middle_point_1 = ball.x + ball.v_x + ball.size_x//2
        #print(ball.x,  ball.size_x, middle_point_1)
        middle_point_2 = static_box.x + static_box.size_x//2
        min_distance = abs(ball.size_x + static_box.size_x) // 2
        distance = abs(middle_point_1 - middle_point_2)
        
        #print(ball.x,  ball.size_x, middle_point_1)
        #print(static_box.x, static_box.size_x, middle_point_2)
        #print(distance)
        #print(min_distance)
        if distance <= min_distance:
            ball.v_y = -ball.v_y
            print("bounce Y")
            if static_box.breakable:
                static_box.material = max(static_box.material - 1, 0)
                if static_box.material == 0:
                    static_box.visible = False
        
        #if len(set(range(ball.x, ball.x + ball.size_x)) & set(range(static_box.x, static_box.x + static_box.size_x))) != 0:
        #if ball.x in range(static_box.x, static_box.x + static_box.size_x) or \
        #(ball.x + ball.size_x) in range(static_box.x, static_box.x + static_box.size_x):
        #if (ball.x >= static_box.x and ball.x <= static_box.x + static_box.size_x):
            #ball.v_y = -ball.v_y
            #print("bounce Y")

class LcdScreen():
    def __init__(self, lcd_width = 84, lcd_height = 48):

        self.run = True

        self.key_states_indexes = { 
            "up" : 0,
            "down" : 1,
            "left" : 2,
            "right" : 3,
            "enter" : 4,
            "back" : 5
        }

        self.draw_objects = []

        self.walls = []
        self.walls.append(DrawObject( x = 0, y = 0, size_x = wall_width, size_y = RESOLUTION_Y, v_x = 0, v_y = 0, breakable = False))         # LEFT
        self.walls.append(DrawObject( x = RESOLUTION_X - wall_width, y = 0, size_x = wall_width, size_y = RESOLUTION_Y, v_x = 0, v_y = 0,  breakable = False)) # RIGHT
        self.walls.append(DrawObject( x = 0, y = 0, size_x = RESOLUTION_X, size_y = wall_width, v_x = 0, v_y = 0, breakable = False)) # TOP
        self.walls.append(DrawObject( x = 0, y = RESOLUTION_Y - wall_width, size_x = RESOLUTION_X, size_y = wall_width, v_x = 0, v_y = 0,  breakable = False)) # BOTTOM

        self.ball = DrawObject(x = RESOLUTION_X // 2, y = RESOLUTION_Y * 2 // 3, size_x = 10, size_y = 10, v_x = 3, v_y = 3)
        self.player = DrawObject(x = RESOLUTION_X // 2 - 40, y = RESOLUTION_Y - 60, size_x = 80, size_y = 10, v_x = 6, v_y = 6, breakable = False)

        self.key_states = [0] * len(self.key_states_indexes)

    def level_init(self):
        self.draw_objects = []

        MAX_COLUMNS = 15
        MAX_ROWS = 3
        #MAX_ROWS

        brick_size_x = (RESOLUTION_X - wall_width*2) // MAX_COLUMNS
        brick_size_y = brick_size_x // 2

        for col in range(MAX_COLUMNS):
            for row in range(MAX_ROWS):
                brick = DrawObject( x = wall_width + col * brick_size_x, \
                                    y = wall_width * 6 + row * brick_size_y, \
                                    size_x = brick_size_x, \
                                    size_y = brick_size_y, \
                                    v_x = 0, \
                                    v_y = 0, \
                                    breakable = True)

                self.draw_objects.append(brick)

        self.draw_objects.append(brick)

    def draw_rect(self, x, y, size_x, size_y, color = pygame.Color("black")):
        #print(x * self.BLOCK_SIZE_X, y * self.BLOCK_SIZE_Y, self.BLOCK_SIZE_X, self.BLOCK_SIZE_Y)
        rect = pygame.Rect(x, y, size_x, size_y)
        pygame.draw.rect(self.screen, color, rect, width = 1)
 
    def draw(self):
        self.screen.fill(pygame.Color("green"))

        # Collision with walls
        for wall in self.walls:
            solve_collision(self.ball, wall)

        # Collision with bricks
        for obj in self.draw_objects:
            solve_collision(self.ball, obj)

        # Collision with player
        solve_collision(self.ball, self.player)

        # Update ball position
        self.ball.update()

        # Draw walls
        for wall in self.walls:
            self.draw_rect(wall.x, wall.y, wall.size_x, wall.size_y, color = pygame.Color("black"))

        # Draw bricks
        for obj in self.draw_objects:
            if obj.visible:
                colors = [pygame.Color("black"), pygame.Color("RED"), pygame.Color("BLUE")]  
                color = colors[obj.material - 1]
                self.draw_rect(obj.x, obj.y, obj.size_x, obj.size_y, color)

        # Draw ball
        self.draw_rect(self.ball.x, self.ball.y, self.ball.size_x, self.ball.size_y);

        # Draw player
        self.draw_rect(self.player.x, self.player.y, self.player.size_x, self.player.size_y);
 
 
 
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

    def updateKeys(self):
        if self.key_states[self.key_states_indexes["left"]]:
            #print("left")
            self.player.x = max(wall_width, self.player.x - self.player.v_x)
        if self.key_states[self.key_states_indexes["right"]]:
            #print("right")
            self.player.x = min(RESOLUTION_X - wall_width, self.player.x + self.player.size_x + self.player.v_x) - self.player.size_x

    def start_game(self):
        pygame.init()
    
        self.screen = pygame.display.set_mode((RESOLUTION_X, RESOLUTION_Y))
        pygame.display.set_caption("LCD simulator")
        clock = pygame.time.Clock()
        
        self.run = True
    
        frame_rate = 30
    
        self.level_init()

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
    
            self.updateKeys()
            self.draw()
    
            pygame.display.update()
            clock.tick(frame_rate)
    
        pygame.quit()
        #exit()
 
if __name__ == "__main__":
    lcdScreen = LcdScreen()
    
    
    lcdScreen.start_game()
 