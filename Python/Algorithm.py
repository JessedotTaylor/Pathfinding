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

        self.alg = 'L'

        self.sLast = grid.start

    def _calcKeys(self, x1, x2=None):
        if x2 != None: #Coordinates passed
            x1 = self.grid.map[x1][x2]  #set x1 to vector, like default behaviour

        k2 = min(x1.getG(), x1.getRHS())
        k1 = k2 + x1.getH()
        x1.setKey([k1, k2])
        return [k1, k2]

    def computeShortestPath(self):
        while (self.U.topKey() < self._calcKeys(self.iGoal, self.jGoal)) or (self.grid.map[self.iGoal][self.jGoal].getRHS() != self.grid.map[self.iGoal][self.jGoal].getG()):

            u = self.U.pop()
            u._status = 2

            if (u.getG() > u.getRHS()):
                u.setG(u.getRHS())
                for x in u.getNeighbours():
                    if x[2] != 1:
                        #print(x)
                        #print("Update called")
                        self.updateVertex(self.grid.map[x[1][0]][x[1][1]])
            else:
                u.setG(99)
                for x in u.getNeighbours():
                    if x[2] != 1:
                        self.updateVertex(self.grid.map[x[1][0]][x[1][1]])
                self.updateVertex(u)
        #print((self.U.topKey(), self._calcKeys(self.iGoal, self.jGoal)), (self.grid.map[self.iGoal][self.jGoal].rhs, self.grid.map[self.iGoal][self.jGoal].g))
    
    def computeShortestPathStep(self, steps):
        for x in range(steps):
            if (self.U.topKey() < self._calcKeys(self.iGoal, self.jGoal)) or (self.grid.map[self.iGoal][self.jGoal].getRHS() != self.grid.map[self.iGoal][self.jGoal].getG()):
                print()
                print((self.U.topKey() < self._calcKeys(self.iGoal, self.jGoal)), (self.grid.map[self.iGoal][self.jGoal].getRHS() != self.grid.map[self.iGoal][self.jGoal].getG()))

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
                            self.updateVertex(self.grid.map[x[1][0]][x[1][1]])
                else:
                    print("u.g less")
                    u.setG(99)
                    # print(u)
                    for x in u.getNeighbours():
                        if x[2] != 1:
                            # print("update vertex: ")
                            # print(self.grid.map[x[1][0]][x[1][1]])
                            self.updateVertex(self.grid.map[x[1][0]][x[1][1]])
                    self.updateVertex(u)

                print("End Step Q")
                print(self.U)
            else:
                print("Goal Found")
                print((self.U.topKey(), self._calcKeys(self.iGoal, self.jGoal)), (self.grid.map[self.iGoal][self.jGoal].getRHS(), self.grid.map[self.iGoal][self.jGoal].getG()))
                break

    def updateVertex(self, u):
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
            

        if u in self.U:
            self.U.remove(u)

        if u.getG() != u.getRHS():
            self.U.insert(u, self._calcKeys(u))


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
        self.setSLast(grid.start)

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

        self.alg = 'D'

    def setSLast(self, inp):
        self.sLast = inp
        self.iStart = inp[0]
        self.jStart = inp[1]

    def genH(self, hueristic, workingPair=None, targetPair=None):
        write = False
        if targetPair == None:
            targetPair = self.sLast
            write = True

        if workingPair == None:
            if hueristic == "EUCLIDEAN":
                for i in range(self.grid.rows):
                    for j in range(self.grid.cols):
                        self.grid.map[i][j].setH(sqrt((i - targetPair[0])**2 + (j - targetPair[1])**2))
        
            elif hueristic == "MANHATTAN":
                for i in range(self.grid.rows):
                    for j in range(self.grid.cols):
                        self.grid.map[i][j].setH(max(abs(i - targetPair[0]), abs(j - targetPair[1])))
        
        else:
            if hueristic == "EUCLIDEAN":
                calcH = sqrt((workingPair[0] - targetPair[0])**2 + (workingPair[1] - targetPair[1])**2)
            elif hueristic == "MANHATTAN":
                calcH = max(abs(workingPair[0] - targetPair[0]), abs(workingPair[1] - targetPair[1]))
            
            if write:
                self.grid.map[workingPair[0]][workingPair[1]].setH(calcH)
            else:
                return calcH

    def _calcKeys(self, x1, x2=None):
        if x2 != None: #Coordinates passed
            x1 = self.grid.map[x1][x2]  #set x1 to vector, like default behaviour

        k2 = min(x1.getG(), x1.getRHS())
        k1 = k2 + x1.getH() + self.km
        #print(k2,  x1.getH(), self.km)
        x1.setKey([k1, k2])
        return [k1, k2]

    def computeShortestPath(self):
        while (self.U.topKey() < self._calcKeys(self.iStart, self.jStart) or (self.grid.map[self.iStart][self.jStart].getRHS() != self.grid.map[self.iStart][self.jStart].getG())):

            kOld = self.U.topKey()

            u = self.U.pop()
            u._status = 2


            if (kOld < self._calcKeys(u)):
                self.U.insert(u, u.getKey())

            elif (u.getG() > u.getRHS()):
                u.setG(u.getRHS())
                for x in u.getNeighbours():
                    if x[0][2] < 99: #Changed to use cost vector
                        self.updateVertex(self.grid.map[x[1][0]][x[1][1]])
            
            else:
                u.setG(99)
                for x in u.getNeighbours():
                    if x[0][2] < 99: #Changed to use cost vector
                        self.updateVertex(self.grid.map[x[1][0]][x[1][1]])
                self.updateVertex(u)
                

    def computeShortestPathStep(self, steps):
        for x in range(steps):
            if (self.U.topKey() < self._calcKeys(self.iStart, self.jStart) or (self.grid.map[self.iStart][self.jStart].getRHS() != self.grid.map[self.iStart][self.jStart].getG())):
                print()
                print((self.U.topKey() < self._calcKeys(self.iStart, self.jStart)), (self.grid.map[self.iStart][self.jStart].getRHS() != self.grid.map[self.iStart][self.jStart].getG()))

                kOld = self.U.topKey()

                u = self.U.pop()
                #print(u)
                u._status = 2

                print("Step: " + str(x+1))
                print("D-Q")
                print(u)

                if (kOld < self._calcKeys(u)):
                    self.U.insert(u, u.getKey())

                elif (u.getG() > u.getRHS()):
                    u.setG(u.getRHS())
                    for x in u.getNeighbours():
                        if x[0][2] < 99: #Changed to use cost vector
                            self.updateVertex(self.grid.map[x[1][0]][x[1][1]])
                
                else:
                    u.setG(99)
                    for x in u.getNeighbours():
                        if x[0][2] < 99: #Changed to use cost vector
                            self.updateVertex(self.grid.map[x[1][0]][x[1][1]])
                    self.updateVertex(u)
                
                print("End Step Q")
                print(self.U)
            else:
                print("Goal Found")
                print((self.U.topKey(), self._calcKeys(self.iStart, self.jStart)), (self.grid.map[self.iStart][self.jStart].getRHS(), self.grid.map[self.iStart][self.jStart].getG()))
                break


    def updateVertex(self, u):
        if u._status == 0:
            u._status = 1
        
        if u != self.grid.map[self.iGoal][self.jGoal]:
            currMin = 999
            for x in u.getNeighbours():
                cost = self.grid.map[x[1][0]][x[1][1]].getG() + x[0][2]
                
                #print(sPrime.g, x[0][2])
                #cost = sPrime.g + x[0][2]
                if cost < currMin:
                    currMin = cost
                #print(x, sPrime.g, x[0][2])
        
            u.setRHS(currMin)

        if u in self.U:
            self.U.remove(u)

        if u.getG() != u.getRHS():
            self.U.insert(u, self._calcKeys(u))

    def writeGrid(self):
        return self.grid




if __name__ == "__main__":
    print("Algorithm printing in __main__")
    # import classes
    # import PriorityQ
    # U = PriorityQ.PriorityQ()
    # #CORNER_COST = sqrt(2)
    # CORNER_COST = 1
    # sequence = [[-1,-1,CORNER_COST],[-1,0,1],[-1,1,CORNER_COST],[0,-1,1],[0,1,1],[1,-1,CORNER_COST],[1,0,1],[1,1,CORNER_COST]]
    # hueristic = 'MANHATTAN'
    # gridWorld = classes.Grid('grids/grid_lpa_slides.map', sequence)
    # DStarLite = DStarLite(gridWorld, U, hueristic)
    # #print(DStarLite.genH(hueristic, [5,2], [2,1]))
    # DStarLite.computeShortestPath()
    # changes = gridWorld.sensorSweep(7,12)
    # # print('\n' * 50)
    # # print(gridWorld)
    # print("Step 0 Q")
    # print(DStarLite.U)

    # if changes != []:
    #     print("Changes Detected")
    #     DStarLite.km = DStarLite.km + DStarLite.genH(hueristic, [7, 12], DStarLite.sLast)
    #     DStarLite.setSLast([7, 12])
    #     DStarLite.genH(hueristic)

    # print(gridWorld.map[DStarLite.sLast[0]][DStarLite.sLast[1]])
    # print(gridWorld.map[DStarLite.iGoal][DStarLite.jGoal])

    # for changedCell in changes:  #The actual cells whoose changes were detected by the sensor sweep
    #     for x in changedCell.getNeighbours(): #The neighbours of the changed cells, the effected cells
    #         targV = gridWorld.map[x[1][0]][x[1][1]]
    #         if targV.getType() != 1:
    #             DStarLite.updateVertex(targV, changedCell) #Update the difference of the changed cell and the effected cells
    #     DStarLite.computeShortestPathStep(50)

    #print(gridWorld)