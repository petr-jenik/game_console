import time

class Queue():
    def __init__(self):
        pass
        self.buffer = []

    def push(self, item):
        self.buffer.insert(0, item)
        #print(self.buffer)

    def pop(self):
        tmp = self.buffer[-1]
        del self.buffer[-1]
        return tmp

    def contains(self, item):
        return item in self.buffer

class Game():
    SIZE_X = 40
    SIZE_Y = 20
    def __init__(self):
        # Define game dimensions
        self.size_x = Game.SIZE_X
        self.size_y = Game.SIZE_Y

        self.queue = Queue()

        # Create a snake's body in the middle of the game area
        middle_point_x = self.size_x//2
        middle_point_y = self.size_y//2
        self.head_position_x = middle_point_x + 1
        self.head_position_y = middle_point_y

        self.queue.push((middle_point_x -1, middle_point_y))
        self.queue.push((middle_point_x   , middle_point_y))
        self.queue.push((middle_point_x +1, middle_point_y))

    def setDirection(self, direction):
        self.direction = direction

    def move(self):
        if self.direction == "up":
            self.head_position_y += 1
        elif self.direction == "down":
            self.head_position_y -= 1
        elif self.direction == "left":
            self.head_position_x -= 1
        elif self.direction == "right":
            self.head_position_x += 1

        self.head_position_x = (self.head_position_x + self.size_x) % self.size_x
        self.head_position_y = (self.head_position_y + self.size_y) % self.size_y

        self.queue.push((self.head_position_x, self.head_position_y))
        self.queue.pop()

    def show(self):
        
        print("*" * self.size_x)

        for y in range(self.size_y):
            row_string = ""
            for x in range(self.size_x):

                if (self.head_position_x == x) and (self.head_position_y == y):
                    row_string += "O"
                elif self.queue.contains((x,y)):
                    row_string += "#"
                else:
                    row_string += "_"
            print(f"row: {row_string}")



if __name__ == "__main__":
    game = Game()
    
    for i in range(10):
        for direction in ("right", "up", "left", "down"):
            game.setDirection(direction)
            for j in range(5 + i):
                game.move()
                game.show()
                time.sleep(0.1)

    