class Game():
    def __init__(self):
        # Define game dimensions
        self.col_count = 3
        self.row_count = 10
        
        # Initialize move counter
        self.moveCounter = 0

        # Initialize buffer with game states
        tmp = [0] * self.row_count
        self.cols = []
        for i in range(self.col_count):
            self.cols.append(tmp[:])

        # Add disks to the first row
        for i in range(self.row_count):
            self.cols[0][i] =(i + 1)

    def showStats(self):
        print(f"Number of moves: {self.moveCounter}")

    def popDisc(self, iCol):
        srcIndex = self._find_firts_non_empty_index(iCol)
        if srcIndex != None:
            tmpDisc = self.cols[iCol][srcIndex]
            self.cols[iCol][srcIndex] = 0
            return tmpDisc
        else:
            return None

    def pushDisc(self, iCol, discSize):
        dstIndex = self._find_last_empty_index(iCol)
        #print(f"Pushing {discSize} to index {dstIndex}")
        if (dstIndex != None):
            self.cols[iCol][dstIndex] = discSize

    def move(self, src_col, dest_col):
        discSize = self.popDisc(src_col)
        #print("**********************")
        #print(f"discSize {discSize}")
        lastDestDisc = self.popDisc(dest_col)

        #print(f"lastDestDisc {lastDestDisc}")

        if lastDestDisc:
            self.pushDisc(dest_col, lastDestDisc)

        if (discSize != None) and ((lastDestDisc == None) or (discSize < lastDestDisc)):
            self.pushDisc(dest_col, discSize)
            self.moveCounter += 1
        elif (discSize != None):
            self.pushDisc(src_col, discSize)

    def _find_firts_non_empty_index(self, col_index):
        for disc_index, disc_size in enumerate(self.cols[col_index]):
            #print(disc_index, disc_size)
            if disc_size != 0:
                return disc_index
        return None

    # TODO TEST THIS METHOD
    def _find_last_empty_index(self, col_index):
        for disc_index, disc_size in enumerate(self.cols[col_index]):
            #print(disc_index, disc_size)
            if disc_size != 0:
                if disc_index == 0:
                    return None
                else:
                    return disc_index - 1
        return disc_index


    def show(self):
        for row in range(self.row_count):
            row_string = ""
            for col in range(self.col_count):
                disc_size = self.cols[col][row]
                col_width = 10
                row_string += ((col_width - disc_size) + 1 )//2 * "_"
                row_string += (disc_size * "#")
                row_string += (col_width - disc_size)//2 * "_"

            print(f"row{row}: {row_string}")
            #print(self.cols)


def moveit(game, frm, to):
   print (f'Presun {frm} --> {to}')
   game.move(frm, to)
   game.show()
   

def dohanoi(game, n, to, frm, using):
   if n == 0: return []
   dohanoi(game, n-1, using, frm, to);
   moveit(game, frm, to);
   dohanoi(game, n-1, to, using, frm);


if __name__ == "__main__":
    game = Game()

    dohanoi(game, 10, 2, 0, 1)
    game.showStats()
    exit(1)
    plan = ((0, 1),
    (0, 2),
    (1, 2),
    (0, 1),
    (2, 0),
    (2, 1),
    (0, 1))
    for item in plan:
        game.move(item[0], item[1])
        game.show()

    
    #game.move(0,1)
    #game.show()
    #game.move(0,1)
    #game.show()
    #game.move(0,1)
    #game.show()
    #game.move(0,1)
    #game.show()
    #game.move(0,1)
    #game.show()
    #game.move(0,1)
    #game.show()
    #game.move(0,1)
    #game.show()
    #game.move(0,1)
    #game.show()
    #game.move(0,1)
    #game.show()
    #game.move(0,1)
    #game.show()
    #game.move(0,1)
    #game.show()
    

    #game.move(0,1)
    #game.show()
    #game.move(0,1)