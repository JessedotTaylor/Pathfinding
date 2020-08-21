from math import sqrt

class Vertex:
    def __init__(self, Type, row, col, rhs=99, h=99, g=99, key=[99,99]):
        self._Type = Type
        self._row = row
        self._col = col
        #Unexplored = 0 , Expanded = 1, Accessed = 2
        #Not gonna care about accesses for status
        self._status = 0
        self._rhs = rhs
        self._h = h
        self._g = g
        self._key = key
        self._x = 0
        self._y = 0
        self._COM = ()
        #self._accesses = 10
        self._accessRead = 0
        self._accessWrite = 10
        #self.CORNER_COST = sqrt(2)
        #self.CORNER_COST = 1
        #self.sequence = [[-1,-1,self.CORNER_COST],[-1,0,1],[-1,1,self.CORNER_COST],[0,-1,1],[0,1,1],[1,-1,self.CORNER_COST],[1,0,1],[1,1,self.CORNER_COST]]

    def setType(self, val):
        self._accessWrite += 1
        self._Type = val
    
    def getType(self):
        self._accessRead += 1
        return self._Type
    
    def getRow(self):
        self._accessRead += 1
        return self._row
    
    def getCol(self):
        self._accessRead += 1
        return self._col
    
    def getIJ(self):
        self._accessRead += 1
        return ([self._row, self._col])
    
    def setRHS(self, val):
        self._accessWrite += 1
        if val > 99:
            val = 99
        self._rhs = val

    def getRHS(self):
        self._accessRead += 1
        return self._rhs

    def setH(self, val):
        self._accessWrite += 1
        self._h = val
    
    def getH(self):
        self._accessRead += 1
        return self._h
    
    def setG(self, val):
        self._accessWrite += 1
        if val > 99:
            val = 99
        self._g = val
    
    def getG(self):
        self._accessRead += 1
        return self._g

    def setKey(self, key):
        self._accessWrite += 1
        self._key = key
    
    def getKey(self):
        self._accessRead += 1
        return self._key
    
    def setCOM(self, val):
        self._accessWrite += 1
        self._COM = val
    
    def getCOM(self):
        #self._accessRead += 1 #Changed every screen update
        return self._COM
    
    def giveNeighbours(self, lst):
        self._accessWrite += 1
        self._neighbours = lst
    
    def getNeighbours(self):
        self._accessRead += 1
        return self._neighbours

    def giveCoords(self, x, y, COM):
        #self._accessWrite += 3  # This changed every screen update
        self._x = x
        self._y = y
        self._COM = COM

    def updateEdgeCost(self, seqVal, newCost):
        #print(self.neighbours, seqVal)
        ind = self.neighbours.index(seqVal)
        self.neighbours.pop(ind)
        self.neighbours.insert(ind, [seqVal[0], seqVal[1], newCost])
        self._accessWrite += 2
        self._accessRead += 1
        #print(self.grid.map[i][j].sequence)
    
    def __repr__(self):
        print(" / " +  '({},{})'.format(chr(self._row + 64), self._col - 1).center(9)  +"\\ ")
        print("|" +  '({},{})'.format(self._row, self._col).center(12)  +"|")
        print("|" + "h = {}".format(self._h).center(12) + "|")
        print("|" + "g = {}".format(self._g).center(12) + "|")
        print("|" + "rhs = {}".format(self._rhs).center(12) + "|")
        print("|" + "type = {}".format(self._Type).center(12) + "|")
        print(" \\ " +  '{}'.format(self._key).center(8) + " / ")
        return ''
        #return str(self.Type),    
    
    def getPrintable(self):
        outlist = []
        outlist.append(" / " +  '({},{})'.format(chr(self._row + 64), self._col - 1).center(9)  +"\\ ")
        outlist.append("|" +  '({},{})'.format(self._row, self._col).center(12)  +"|")
        outlist.append("|" + "h = {}".format(self._h).center(12) + "|")
        outlist.append("|" + "g = {}".format(self._g).center(12) + "|")
        outlist.append("|" + "rhs = {}".format(self._rhs).center(12) + "|")
        outlist.append("|" + "type = {}".format(self._Type).center(12) + "|")
        outlist.append(" \\ " +  '{}'.format(self._key).center(8) + " / ")
        return outlist
    
    def printNeighbours(self):
        if len(self._neighbours) != 8:
            print("Nope!")
            return
        #([[dx, dy, 99], [i + dx, j + dy], 1])
        #               ( , ),       ( , ),      ( , ),
        #                 \\          |           /
        #               {cost}      {cost}      {cost}
        #                     \\      |       /
        #                     /   ({grid}) \\
        # ( , ), - {cost} -   | type = {}   | - {cost} - ( , ), 
        #                      \\  real ?  /
        #                     /       |       \\
        #                 {cost}      {cost}  {cost}
        #                 /           |           \\
        #             ( , ),         ( , ),       ( , ), 

        n = self._neighbours
        #print(n)
        width = 45
        print(('{}'.format(n[0][1]) + ' ' * 11 + '{}'.format(n[1][1]) + ' ' * 11 + '{}'.format(n[2][1])).center(width))
        print('\\          |           /'.center(width))
        print('{}          {}         {}'.format(n[0][0][2], n[1][0][2], n[2][0][2]).center(width))
        print('\\      |       /'.center(width))
        print((" / " +  '({},{})'.format(self._row, self._col).center(8)  + "\\ ").center(width))
        print(('{}'.format(n[3][1]) + ' - ' + '{}'.format(n[3][0][2]) + ' - |' + "type = {}".format(self._Type).center(12) + '| - ' + '{}'.format(n[4][0][2]) + ' - ' + '{}'.format(n[4][1])).center(width))
        print((" \\" +  ' {}, {}, {} '.format(self._h, self._g, self._rhs) + "/ ").center(width))
        print("/       |       \\".center(width))
        print('{}       {}          {}'.format(n[5][0][2], n[6][0][2], n[7][0][2]).center(width))
        print("/           |           \\".center(width))
        print(('{}'.format(n[5][1]) + ' ' * 11 + '{}'.format(n[6][1]) + ' ' * 11 + '{}'.format(n[7][1])).center(width))
        print()

    #Overloading == (Equal to) operator
    def __eq__(self, other):
        self._accessRead +=1 
        if self._key[0] == other._key[0]:
            if self._key[1] == other._key[1]:
                return True
        return False
    
    #Overloading < (less than) operator
    def __lt__(self, other):
        self._accessRead += 1 
        if type(other) != Vertex:
            other = Vertex(0, 99, 99, key=[other[0], other[1]])
        if self._key[0] < other._key[0]:
            return True
        elif self._key[0] == other._key[0]:
            if self._key[1] < other._key[1]:
                return True
            else:
                return False
        else:
            return False

    #Overloading <= (less equal) operator
    def __le__(self, other):
        self._accessRead +=1 
        if self > other:
            return False
        else:
            return True
    
    #Overloading > (greater than) operator
    def __gt__(self, other):
        self._accessRead +=1 
        if type(other) != Vertex:
            other = Vertex(0, 99, 99, key=[other[0], other[1]])
        if self._key[0] > other._key[0]:
            return True
        elif self._key[0] == other._key[0]:
            if self._key[1] > other._key[1]:
                return True
            else:
                return False
        else:
            return False

    
    #Guess overload != (Not equal) operator
    def __ne__(self, other):
        self._accessRead +=1 
        if self._key[0] == other._key[0]:
            if self._key[1] == other._key[1]:
                return False
        return True

    def leG(self, other):
        self._accessRead += 1
        if self._key <= other._key:
            return True
        return False



    
class Grid:
    def __init__(self, filename, sequence):
        self.genFromFile(filename)
        self.start = self.findType(6)
        self.goal = self.findType(7)
        self.sequence = sequence
        self.genNeighbours()

    def genFromFile(self, filename):

        with open('Python/' + filename, 'r') as f:
            inp = f.readlines()

        #print(int(inp[0]), int(inp[1]))

        rows = int(inp[0])
        cols = int(inp[1])
        grid = []
        
        for i in range(rows):
            grid.append([Vertex(int(inp[i+2][j]), i, j) for j in range(cols)])
            # for j in range(cols-1):
            #     print(int(inp[i+2][j]))
        
        self.rows = rows
        self.cols = cols
        self.map = grid
        self.mapName = filename.split('/')[1]

        print("Loaded File: " + self.mapName)
    
        #print(grid)
    
    def setVertex(self, i, j,Type):
        self.map[i][j].setType(Type)
        self.genNeighbours([i,j])
        self.writeToFile()
    
    def findType(self, targType):
        row, col = 0, 0
        for i in range(self.rows):
            for j in range(self.cols):
                if self.map[i][j].getType() == targType:
                    return i,j
            
        return -99,99 #No type found

    def writeToFile(self):
        orgFile = self.mapName
        outFile = 'Python/grids/'+ orgFile.split('.')[0] + 'MOD.map'
        with open(outFile, 'w') as f:
            outLines = [str(self.rows) + '\n', str(self.cols) + '\n']
            for i in range(self.rows):
                for j in range(self.cols):
                    outLines.append(str(self.map[i][j]._Type))
                    #pass
                outLines.append('\n')
        
            f.writelines(outLines)


    def genNeighbours(self, x1=None):
        if x1 == None:
            for i in range(self.rows):
                for j in range(self.cols):
                    neighbours = []
                    for x in self.sequence:
                        dx = x[0]
                        dy = x[1]
                        cost = x[2]
                        if i + dx >= 0 and i + dx <= self.rows-1 and j + dy >= 0 and j + dy <= self.cols-1: #If within bounding box of map
                            if self.map[i][j].getType() == 1 or self.map[i+dx][j+dy].getType() == 1: #If cell type is blocked, connections noted as red. May be unnecissary
                                neighbours.append([[dx, dy, 99], [i + dx, j + dy], 1])
                            else:
                                neighbours.append([[dx, dy, cost], [i + dx, j + dy], 0])

                    self.map[i][j].giveNeighbours(neighbours)
        else:
            i, j = x1
            neighbours = []
            for x in self.sequence:
                dx = x[0]
                dy = x[1]
                cost = x[2]
                if i + dx >= 0 and i + dx <= self.rows-1 and j + dy >= 0 and j + dy <= self.cols-1: #If within bounding box of map
                    if self.map[i][j].getType() == 1 or self.map[i+dx][j+dy].getType() == 1: #If cell type is blocked, connections noted as red. May be unnecissary
                        neighbours.append([[dx, dy, 99], [i + dx, j + dy], 1])
                    else:
                        neighbours.append([[dx, dy, cost], [i + dx, j + dy], 0])

            self.map[i][j].giveNeighbours(neighbours)

        # for i in self.map[0]:
        #     print(i.neighbours[0][0])

    def sensorSweep(self, i, j):
        changes = []
        for x in self.map[i][j].getNeighbours():
            targV = self.map[x[1][0]][x[1][1]]
            if targV.getType() == 8:
                targV.setType(0)
                changes.append(targV)

            if targV.getType() == 9:
                targV.setType(1)
                changes.append(targV)

        if len(changes) > 0:
            self.genNeighbours([i, j])
            return changes
        return []
    
    def getStatus(self):
        results = {}
        results['Total Cells'] = self.rows * self.cols
        results['Cells Less Border'] = (self.rows-2) * (self.cols - 2)
        results['Cells Expanded'] = 0
        results['Cells Accessed'] = 0
        results['Total Vertex Reads'] = 0
        results['Total Vertex Writes'] = 0
        for i in range(1, self.rows-1):
            for j in range(1, self.cols-1):
                if self.map[i][j]._status == 1:
                    results['Cells Expanded'] += 1
                elif self.map[i][j]._status == 2:
                    results['Cells Expanded'] += 1
                    results['Cells Accessed'] += 1
                
                results['Total Vertex Reads'] += self.map[i][j]._accessRead
                results['Total Vertex Writes'] += self.map[i][j]._accessWrite
        
        return results

                


    def __repr__(self):
        print(self.mapName)
        print()
        printList = []
        for i in self.map:
            printList.append([j.getPrintable() for j in i])

        for i in range(self.rows):
            for x in range(7):
                for z in range(self.cols):
                    #print(x,z)
                    print(printList[i][z][x], end='    ')
                print()
            print()

        return ''


                
if __name__ == '__main__':
    #CORNER_COST = sqrt(2)
    CORNER_COST = 1
    sequence = [[-1,-1,CORNER_COST],[-1,0,1],[-1,1,CORNER_COST],[0,-1,1],[0,1,1],[1,-1,CORNER_COST],[1,0,1],[1,1,CORNER_COST]]
    gridWorld = Grid('grids/grid_DStar_journal.map', sequence)
    #gridWorld.sensorSweep(3,2)
    #print(gridWorld)
    z = Vertex(3,0,0, key=[1,2])
    x = Vertex(1,0,0, key=[0,1])
    c = Vertex(2,0,0, key=[1,1])
    v = Vertex(0,0,0, key=[10,1])
    v1 = Vertex(0,0,0, key=[10,1])
    print(v1 <= v)
    print( not (v1 > v))
    print(c < v)
    print(c < [10,1])
    print(z != x)
    # gridWorld.map[1][1].printNeighbours()
    # print(gridWorld.map[1][1])

    #Grid.genFromFile('grids/grid_Dstar_journal.map')
    #print(gridWorld)