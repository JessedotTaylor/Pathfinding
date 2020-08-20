class LPAStar:
    def __init__(self, grid, queue, hueristic):
        for i in range(grid.rows):
            for j in range(grid.cols):
                grid.map[i][j].setRHS(99)
                grid.map[i][j].setG(99)
        
        self.grid = grid
        self.genH(hueristic)

        self.iStart, self.jStart = grid.start
        self.iGoal, self.jGoal = grid.goal

        grid.map[self.iStart][self.jStart].setRHS(0)
        grid.map[self.iStart][self.jStart].setKey([grid.map[self.iStart][self.jStart].getH(), 0])
        queue.insert(grid.map[self.iStart][self.jStart], grid.map[self.iStart][self.jStart].getKey())

        self.grid = grid
        self.U = queue

    def _calcKeysIJ(self, i, j):
        k2 = min(self.grid.map[i][j].getG(), self.grid.map[i][j].getRHS())
        k1 = k2 + self.grid.map[i][j].getH()
        self.grid.map[i][j].setKey([k1, k2])
        return [k1, k2]
    
    def _calcKeysU(self, u):
        k2 = min(u.getG(), u.getRHS())
        k1 = k2 + u.getH()
        u.setKey([k1, k2])
        return [k1, k2]

    def computeShortestPath(self):
        # print((self.U.topKey(), self._calcKeysIJ(self.iGoal, self.jGoal)))
        # print(self.grid.map[self.iGoal][self.jGoal].rhs, self.grid.map[self.iGoal][self.jGoal].g)
        while (self.U.topKey() < self._calcKeysIJ(self.iGoal, self.jGoal)) or (self.grid.map[self.iGoal][self.jGoal].getRHS() != self.grid.map[self.iGoal][self.jGoal].getG()):

            u = self.U.pop()
            u._status = 2

            if (u.getG() > u.getRHS()):
                u.setG(u.getRHS())
                for x in u.getNeighbours():
                    if x[2] != 1:
                        #print(x)
                        #print("Update called")
                        self.updateVertex(self.grid.map[x[1][0]][x[1][1]], u)
            else:
                u.setG(99)
                for x in u.getNeighbours():
                    if x[2] != 1:
                        self.updateVertex(self.grid.map[x[1][0]][x[1][1]], u)
                        self.updateVertex(u, u)
        #print((self.U.topKey(), self._calcKeysIJ(self.iGoal, self.jGoal)), (self.grid.map[self.iGoal][self.jGoal].rhs, self.grid.map[self.iGoal][self.jGoal].g))
    
    def computeShortestPathStep(self, steps):
        for x in range(steps):
            if (self.U.topKey() < self._calcKeysIJ(self.iGoal, self.jGoal)) or (self.grid.map[self.iGoal][self.jGoal].getRHS() != self.grid.map[self.iGoal][self.jGoal].getG()):
                print()
                print((self.U.topKey() < self._calcKeysIJ(self.iGoal, self.jGoal)), (self.grid.map[self.iGoal][self.jGoal].getRHS() != self.grid.map[self.iGoal][self.jGoal].getG()))

                u = self.U.pop()
                u._status = 2

                print("Step: " + str(x+1))
                print("D-Q")
                print(u)

                # print("EN-Q")
                # print(self.U)

                if (u.getG() > u.getRHS()):
                    u.setG(u.getRHS())
                    for x in u.getNeighbours():
                        if x[2] != 1:
                            #print(x)
                            #print("Update called")
                            self.updateVertex(self.grid.map[x[1][0]][x[1][1]], u)
                else:
                    # print("u.g less")
                    u.setG(99)
                    # print(u)
                    for x in u.getNeighbours():
                        if x[2] != 1:
                            # print("update vertex: ")
                            # print(self.grid.map[x[1][0]][x[1][1]])
                            self.updateVertex(self.grid.map[x[1][0]][x[1][1]], u)
                            self.updateVertex(u, u)

                print("End Step Q")
                print(self.U)
            else:
                print("Goal Found")
                print((self.U.topKey(), self._calcKeysIJ(self.iGoal, self.jGoal)), (self.grid.map[self.iGoal][self.jGoal].getRHS(), self.grid.map[self.iGoal][self.jGoal].getG()))
                break




    def updateVertex(self, u, sPrime):
        if u._status == 0:
            u._status = 1
        if u != self.grid.map[self.iStart][self.jStart]:
            currMin = 999
            for x in u.getNeighbours():
                cost = self.grid.map[x[1][0]][x[1][1]].getG() + x[0][2]
                
                #print(sPrime.g, x[0][2])
                #cost = sPrime.g + x[0][2]
                if cost < currMin:
                    currMin = cost
                #print(x, sPrime.g, x[0][2])
        
            u.setRHS(currMin)
            #print(u)
        if u in self.U:
            self.U.remove(u)

        if u.getG() != u.getRHS():
            self.U.insert(u, self._calcKeysU(u))


    def writeGrid(self):
        return self.grid

    def printGrid(self):
        print(self.grid)

    def genH(self, hueristic):
        if hueristic == "EUCLIDEAN":
            for i in range(self.grid.rows):
                for j in range(self.grid.cols):
                    self.grid.map[i][j].setH(sqrt((i - self.grid.goal[0])**2 + (j - self.grid.goal[1])**2))

        elif hueristic == "MANHATTAN":
            for i in range(self.grid.rows):
                for j in range(self.grid.cols):
                    self.grid.map[i][j].setH(max(abs(i - self.grid.goal[0]), abs(j - self.grid.goal[1])))
    
    
class DStarLite:
    def __init__(self, grid, queue, hueristic):
        self.km = 0

        for i in range(grid.rows):
            for j in range(grid.cols):
                grid.map[i][j].setRHS(99)
                grid.map[i][j].setG(99)
                #self.genH(i, j, hueristic)

        self.iStart, self.jStart = grid.start
        self.iGoal, self.jGoal = grid.goal
        
        self.grid = grid
        self.genH(hueristic)

        grid.map[self.iGoal][self.jGoal].setRHS(0)
        grid.map[self.iGoal][self.jGoal].setKey([grid.map[self.iGoal][self.jGoal].getH(), 0])
        queue.insert(grid.map[self.iGoal][self.jGoal], grid.map[self.iGoal][self.jGoal].getKey())

        self.grid = grid
        self.U = queue


    def genHIJ(self, i, j, hueristic):
        if hueristic == "EUCLIDEAN":
            self.grid.map[i][j].setH(sqrt((i - self.iStart)**2 + (j - self.jStart)**2))
        
        elif hueristic == "MANHATTAN":
            self.grid.map[i][j].setH(max(abs(i - self.iStart), abs(j - self.jStart)))

    def genH(self, hueristic):
        if hueristic == "EUCLIDEAN":
            for i in range(self.grid.rows):
                for j in range(self.grid.cols):
                    self.grid.map[i][j].setH(sqrt((i - self.iStart)**2 + (j - self.jStart)**2))
        
        elif hueristic == "MANHATTAN":
            for i in range(self.grid.rows):
                for j in range(self.grid.cols):
                    self.grid.map[i][j].setH(max(abs(i - self.iStart), abs(j - self.jStart)))

    def _calcKeysIJ(self, i, j):
        k2 = min(self.grid.map[i][j].getG(), self.grid.map[i][j].getRHS())
        k1 = k2 + self.grid.map[i][j].getH() + self.km
        self.grid.map[i][j].setKey([k1, k2])
        return [k1, k2]
    
    def _calcKeysU(self, u):
        k2 = min(u.getG(), u.getRHS())
        k1 = k2 + u.getH() + self.km
        u.setKey([k1, k2])
        return [k1, k2]

    def computeShortestPathStep(self, steps):
        for x in range(steps):
            if (self.U.topKey() < self._calcKeysU(self.iStart, self.jStart) or (self.grid.map[self.iStart][self.jStart].getRHS() != self.grid.map[self.iStart][self.jStart].getG())):
                print()
                print((self.U.topKey() < self._calcKeysU(self.iStart, self.jStart)), (self.grid.map[self.iStart][self.jStart].getRHS() != self.grid.map[self.iStart][self.jStart].getG()))

                kOld = self.U.topKey()

                u = self.U.pop()
                u._status = 2

                print("Step: " + str(x+1))
                print("D-Q")
                print(u)

                if (kOld < self._calcKeysU(u)):
                    self.U.insert(u, u.getKey())

                elif (u.getG() > u.getRHS()):
                    u.setG(u.getRHS())
                    for x in u.getNeighbours():
                        if x[0][2] >= 99: #Changed to use cost vector
                            pass







if __name__ == "__main__":
    import classes
    import PriorityQ
    U = PriorityQ.PriorityQ()
    #CORNER_COST = sqrt(2)
    CORNER_COST = 1
    sequence = [[-1,-1,CORNER_COST],[-1,0,1],[-1,1,CORNER_COST],[0,-1,1],[0,1,1],[1,-1,CORNER_COST],[1,0,1],[1,1,CORNER_COST]]
    hueristic = 'MANHATTAN'
    gridWorld = classes.Grid('grids/grid_DStar_journal.map', sequence)
    DStarLite = DStarLite(gridWorld, U, hueristic)
    print(gridWorld)
    # LPA = LPAStar(gridWorld, U, hueristic)
    # LPA.computeShortestPath()
    # print('\n' * 50)
    # changes = gridWorld.sensorSweep(3,2)
    # #print(gridWorld)
    # print("Step 0 Q")
    # print(LPA.U)
    
    # for changedCell in changes:  #The actual cells whoose changes were detected by the sensor sweep
    #     for x in changedCell.neighbours: #The neighbours of the changed cells, the effected cells
    #         targV = gridWorld.map[x[1][0]][x[1][1]]
    #         if targV.getType() != 1:
    #             #targV.printNeigbours()
    #             #print(targV, changedCell)
    #             LPA.updateVertex(targV, changedCell) #Update the difference of the changed cell and the effected cells
    # LPA.computeShortestPathStep(10)
    #LPA.printGrid()