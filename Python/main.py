import pygame
import classes
import PriorityQ
import Algorithm
from math import sqrt
import time

# Define some colors
black     = (   0,   0,   0)
white     = ( 255, 255, 255)
green     = (   0, 255,   0)
red       = ( 255,   0,   0)
lightblue = (   0, 100, 255)
tan       = ( 234, 155, 128)
darkblue  = (   0,   0, 255)
grey      = ( 214, 214, 214)
darkgrey  = ( 100, 100, 100)
yellow    = ( 255, 242,   0)

#-------------------------------------
# Set heuristic

HEURISTIC = 'MANHATTAN'
#HEURISTIC = 'EUCLIDEAN'
#-------------------------------------

#CORNER_COST = sqrt(2)
CORNER_COST = 1
sequence = [[-1,-1,CORNER_COST],[-1,0,1],[-1,1,CORNER_COST],[0,-1,1],[0,1,1],[1,-1,CORNER_COST],[1,0,1],[1,1,CORNER_COST]]

#-------------------------------------
# Set input map, and init grid

inputMapName = 'grids/grid_lpa_journal.map'
#inputMapName = 'grids/grid_Dstar_journal.map'
#inputMapName = 'grids/grid_big.map'
gridWorld = classes.Grid(inputMapName, sequence)
MASTER_GRID = classes.Grid(inputMapName, sequence)
#-------------------------------------


alg = ''

H_CELLS = gridWorld.rows
V_CELLS = gridWorld.cols

#--------------------------------------------
# Starting margins for the top header, results sidebar, margin from edges of the screen, margins of each cell, height of text offset
HEADER = 100
RESULTS = 30
OUTSIDE_MARGIN = 30
GRID_MARGIN = 2
TEXT_OFFSET = 5
#---------------------------------------------



#---------------------------------------------
# pygame startup
pygame.init()

# Set the width and height of the screen
size = [1900, 1000] #1360 768 Screen 1900 1000
screen=pygame.display.set_mode(size)
#-----------------------------------------------



#-------------------------------------------------
# Width and height calculations of each cell

#Width / cell = total width - margins / # cells
width = (size[0] - ((OUTSIDE_MARGIN + RESULTS)+ ((H_CELLS + 1) * GRID_MARGIN))) // V_CELLS



#Height / cell = total height - header - outside margin - margins / # cells
height = (size[1] - (HEADER + OUTSIDE_MARGIN + (V_CELLS + 1) * GRID_MARGIN)) // H_CELLS
#-------------------------------------------------



#-------------------------------------------------
# Font size calculations

cellWidthDiv = (height // 6)
font = pygame.font.SysFont(None, (cellWidthDiv + 1))

HeaderWidthDiv = HEADER // 4
headerFont = pygame.font.SysFont(None, (HeaderWidthDiv +1))
#--------------------------------------------------


#-----------------------------------------
# Static top header render
img1 = headerFont.render('INCREMENTAL OPTIMAL SEARCH (8 CONNECTED GRIDWORLD)', True, yellow, black)
img2 = headerFont.render('F1: show / hide this header, F4: show / hide details, F5: show / hide Performace Measures, F10: run search, L/R Arrow Keys: Move through path', True, green, black) #Haven't added copy map to algorithm function
img3 = headerFont.render('B: block cell, U: unblock cell, H: h-values, K: key-values, S: new START, X: new GOAL, P: cell positions, C: local connections, M: all connections, Q: status', True, white, black)
img4 = headerFont.render("Filname = {}, heuristic = {}".format(gridWorld.mapName, HEURISTIC), True, white, black)
#-----------------------------------



#-----------------------------------
# Dynamic results sidebar. Font size calculation, Memory alocation and re-render function

resultsWidthDiv = 40
resultsFont = pygame.font.SysFont(None, (resultsWidthDiv))

res1, res2, res3, res4, res5, res6, res7, res8, res9, res10, res11, res12, res13, res14 = [0 for x in range(14)]
resList = [res1, res2, res3, res4, res5, res6, res7, res8, res9, res10, res11, res12, res13, res14]

def getAndRenderResults():
    # results['Total Cells'] = self.rows * self.cols
    # results['Cells Less Border'] = (self.rows-2) * (self.cols - 2)
    # results['Cells Expanded'] = 0
    # results['Cells Accessed'] = 0
    # results['Total Vertex Reads'] = 0
    # results['Total Vertex Writes'] = 0
    results = gridWorld.getStatus()

    global res1
    res1 = resultsFont.render("All results ignore border cells", True, white, black)
    global res2
    res2 = resultsFont.render("Total Cells = {}".format(results['Cells Less Border']), True, white, black)
    global res3
    res3 = resultsFont.render("Max Q = {}".format(U.maxU), True, white, black)
    global res4
    res4 = resultsFont.render("Path Length = {}".format(lenP), True, white, black)
    global res5
    res5 = resultsFont.render("Cells Expanded = {}".format(results['Cells Expanded']), True, white, black)
    global res6
    res6 = resultsFont.render("% Cells Expanded = {:.3f}%".format((results['Cells Expanded'] / results['Cells Less Border']) * 100 ), True, white, black)
    global res7
    res7 = resultsFont.render("Cells Accessed = {}".format(results['Cells Accessed']), True, white, black)
    global res8
    res8 = resultsFont.render("% Cells Accessed = {:.3f}%".format((results['Cells Accessed'] / results['Cells Less Border']) * 100 ), True, white, black)
    global res9
    res9 = resultsFont.render("Total Vertex Reads = {}".format(results['Total Vertex Reads']), True, white, black)
    global res10
    res10 = resultsFont.render("Avg Reads / Cell = {:.3f}".format(results['Total Vertex Reads'] / results['Cells Less Border']), True, white, black)
    global res11
    res11 = resultsFont.render("Total Vertex Writes = {}".format(results['Total Vertex Writes']), True, white, black)
    global res12
    res12 = resultsFont.render("Avg Writes / Cell = {:.3f}".format(results['Total Vertex Writes'] / results['Cells Less Border']), True, white, black)
    global res13
    if calcFirstSearch != -99:
        res13 = resultsFont.render("Search 1 Execution Time = {:.6f} ms".format(calcFirstSearch *1000), True, white, black)
    else:
        res13 = resultsFont.render("{}".format("Search 1 calculation not performed"), True, white, black)

    global res14
    if calcSecondSearch != -99:
        res14 = resultsFont.render("Search 2 Execution Time = {:.6f} ms".format(calcSecondSearch *1000), True, white, black)
    else:
        res14 = resultsFont.render("{}".format("Search 2 calculation not performed"), True, white, black)
    global resList
    resList = [res1, res2, res3, res4, res5, res6, res7, res8, res9, res10, res11, res12, res13, res14]

#-----------------------------------
# Variable setup

done = False

drawHValues = False
drawKeyValues = False
drawRHS = False
drawG = False
drawCellPos = False
drawStatus = False

drawConn = False
drawLocal = []

drawPath = False
path = []
lenP = 0

currRobotPos = 0
iRob, jRob = -9999, -9999
prevIRob, prevJRob = -9999, -9999

drawHeader = True
drawResults = False

calcFirstSearch = -99
calcSecondSearch = -99

def resetRobotPathData():
    global path
    path = []
    global lenP
    lenP = 0
    global currRobotPos
    currRobotPos = 0
    global iRob, jRob
    iRob, jRob = -9999, -9999
    global prevIRob, prevJRob
    prevIRob, prevJRob = -9999, -9999

#-----------------------------------------


clock = pygame.time.Clock()

def getGridCoord(pos):
    column = (pos[0] - OUTSIDE_MARGIN- RESULTS) // (width)
    row = (pos[1] - HEADER) // (height)
    return row, column


#-------------------------------------
# Main program loop
while not done:
    for event in pygame.event.get(): #Keyboard event handler
        if event.type == pygame.QUIT:
            done = True
        
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_b: #Block cell
                pos = pygame.mouse.get_pos()
                i, j = getGridCoord(pos)
                if gridWorld.map[i][j]._Type != 1:
                    gridWorld.setVertex(i, j, 1)
                
                resetRobotPathData()
            
            if event.key == pygame.K_u: #Unblock cell
                #Unblock cell
                pos = pygame.mouse.get_pos()
                i, j = getGridCoord(pos)
                if gridWorld.map[i][j]._Type != 0:
                    gridWorld.setVertex(i, j, 0)
                resetRobotPathData() 

            if event.key == pygame.K_s: #Set new start cell
                #New Start Cell
                pos = pygame.mouse.get_pos()
                i, j = getGridCoord(pos)
                if gridWorld.map[i][j]._Type != 6:
                    gridWorld.setVertex(i, j, 6) #Set new Start position
                    oldStart = gridWorld.start #Get old coords of Start (Can use .get_Type(6))
                    gridWorld.start = (i, j) #Set new coords of Start
                    if MASTER_GRID.map[oldStart[0]][oldStart[1]]._Type != 6:
                        gridWorld.setVertex(oldStart[0], oldStart[1], MASTER_GRID.map[oldStart[0]][oldStart[1]]._Type) #Set old Start 
                                                                                                                #_type to what it was on the original file
                    else:
                        gridWorld.setVertex(oldStart[0], oldStart[1], 0) #Else set to unblocked
                resetRobotPathData()  

            if event.key == pygame.K_x: #Set new Goal cell
                #New goal Cell
                pos = pygame.mouse.get_pos()
                i, j = getGridCoord(pos)
                if gridWorld.map[i][j]._Type != 7:
                    gridWorld.setVertex(i, j, 7) #Set new goal position
                    oldGoal = gridWorld.goal #Get old coords of Goal (Can use .get_Type(6))
                    gridWorld.goal = (i, j) #Set new coords of Goal
                    if MASTER_GRID.map[oldGoal[0]][oldGoal[1]]._Type != 7:
                        gridWorld.setVertex(oldGoal[0], oldGoal[1], MASTER_GRID.map[oldGoal[0]][oldGoal[1]]._Type) #Set old Goal 
                                                                                                                #type to what it was on the original file
                    else:
                        gridWorld.setVertex(oldGoal[0], oldGoal[1], 0) #Else set to unblocked
                resetRobotPathData()

            if event.key == pygame.K_p: #Display Cell position
                drawCellPos = not drawCellPos
                drawConn = False

            if event.key == pygame.K_h: #Display H values
                drawHValues = not drawHValues
                drawConn = False

            if event.key == pygame.K_g: #Display G values
                drawG = not drawG
                drawConn = False

            if event.key == pygame.K_r: #Display RHS
                drawRHS = not drawRHS
                drawConn = False
            
            if event.key == pygame.K_k: #Display key values
                drawKeyValues = not drawKeyValues
                drawConn = False
            
            if event.key == pygame.K_q: #Display key values
                drawStatus = not drawStatus
                drawConn = False
            
            if event.key == pygame.K_F4: #Display cell information
                if not drawHValues:
                    drawHValues = True
                    drawKeyValues = True
                    drawRHS = True
                    drawG = True
                    drawCellPos = True
                    drawConn = False
                    drawStatus = True
                else:
                    drawHValues = False
                    drawKeyValues = False
                    drawRHS = False
                    drawG = False
                    drawCellPos = False
                    drawStatus = False

            if event.key == pygame.K_F1: #Display header (help)
                if drawHeader:
                    drawHeader = False
                    HEADER = OUTSIDE_MARGIN
                    resetRobotPathData()
                else:
                    drawHeader = True
                    HEADER = 100
                    resetRobotPathData()
                height = (size[1] - (HEADER + OUTSIDE_MARGIN + (V_CELLS + 1) * GRID_MARGIN)) // H_CELLS
            
            if event.key == pygame.K_F5: #Display results
                if drawResults:
                    drawResults = False
                    RESULTS = OUTSIDE_MARGIN
                    resetRobotPathData()
                else:
                    getAndRenderResults()
                    drawResults = True
                    RESULTS = 600
                    resetRobotPathData()

                width = (size[0] - (OUTSIDE_MARGIN + RESULTS + (H_CELLS + 1) * GRID_MARGIN)) // V_CELLS

            if event.key == pygame.K_m: #Display all connections
                if not drawConn:
                    drawHValues = False
                    drawKeyValues = False
                    drawRHS = False
                    drawG = False
                    drawCellPos = False

                    drawConn = True
                else:
                    drawConn = False

            if event.key == pygame.K_c: #Display local connections and print costs
                drawHValues = False
                drawKeyValues = False
                drawRHS = False
                drawG = False
                drawCellPos = False

                pos = pygame.mouse.get_pos()
                i, j = getGridCoord(pos)
                if [i,j] in drawLocal:
                    drawLocal.remove([i, j])
                else:
                    drawLocal.append([i,j])
                    gridWorld.map[i][j].printNeighbours()            

            if event.key == pygame.K_F7: #Select LPA* and run first search
                U = PriorityQ.PriorityQ()
                LpaStar = Algorithm.LPAStar(gridWorld, U, HEURISTIC)
                
                start = time.time()
                LpaStar.computeShortestPath()
                end = time.time()

                calcFirstSearch = end - start
                drawPath = True
                getAndRenderResults()

                alg = LpaStar.alg

            if event.key == pygame.K_F8: #Select D*Lite and run first search
                U = PriorityQ.PriorityQ()
                DStarLite = Algorithm.DStarLite(gridWorld, U, HEURISTIC)
                
                start = time.time()
                DStarLite.computeShortestPath()
                end = time.time()

                calcFirstSearch = end - start
                drawPath = True
                getAndRenderResults()
                alg = DStarLite.alg

            if event.key == pygame.K_RIGHT: #Move forwards through path
                #print("right arrow")
                if currRobotPos <= lenP-1:
                    currRobotPos += 1
                getAndRenderResults()

            if event.key == pygame.K_LEFT: #Move backwards through path
                if currRobotPos > 0:
                    currRobotPos -= 1
                getAndRenderResults()

            if event.key == pygame.K_ESCAPE: #Quit
                done = True
            
            #-----------------------------------------------------
            #Add debug info keys to header

            if event.key == pygame.K_F11: #Calculate path, don't render path
                start = time.time()
                if alg == 'D':
                    DStarLite.computeShortestPath()
                if alg == 'L':
                    LpaStar.computeShortestPath()
                end = time.time()
                #DStarLite.setSLast(gridWorld.start)
                calcFirstSearch = end - start
                getAndRenderResults()

            if event.key == pygame.K_F12: #SensorSweep on current mouse position, re-render path
                pos = pygame.mouse.get_pos()
                i, j = getGridCoord(pos)
                changes = gridWorld.sensorSweep(i, j)
                if changes != [] and alg == 'D':
                    #print("Changes Detected")
                    DStarLite.km = DStarLite.km + DStarLite.genH(HEURISTIC, [i, j], DStarLite.sLast)
                    DStarLite.setSLast([i, j])
                    DStarLite.genH(HEURISTIC)
                for changedCell in changes:  #The actual cells whoose changes were detected by the sensor sweep
                    for x in changedCell.getNeighbours(): #The neighbours of the changed cells, the effected cells
                        targV = gridWorld.map[x[1][0]][x[1][1]]
                        if targV.getType() != 1:
                            if alg == 'D':
                                DStarLite.updateVertex(targV) #Update the difference of the changed cell and the effected cells
                            if alg == 'L':
                                #print(targV)
                                LpaStar.updateVertex(targV)
                    start = time.time()
                    if alg == 'D':
                        DStarLite.computeShortestPath()
                    if alg == 'L':
                        LpaStar.computeShortestPath()
                    end = time.time()
                    calcSecondSearch = end - start
                    path = []
                    lenP = 0
                    currRobotPos = 0   
                    

    clock.tick(20) #Limit to ?60? frames / second

    screen.fill(black)
    
   
    if drawHeader:  #Draw Top header
        screen.blit(img1, (5, 5)) #Static text headers, so no need to re-render
        screen.blit(img2, (5, 25))
        screen.blit(img3, (5, 45))
        if alg == 'D':
            algOut = "D*Lite"
        elif alg == 'L':
            algOut = "LPA*"
        else :
            algOut = "None"
        img4 = headerFont.render("Filname = {}, heuristic = {}, Selected Algorithm = {}".format(gridWorld.mapName, HEURISTIC, algOut), True, white, black)
        screen.blit(img4, (5, 65))
    
    if drawResults: #Draw results sidebar
        for i in range(14):
            screen.blit(resList[i], (10, (40 * i) + 5 + HEADER))
    
    
    #Draw white background for map
    colour = white
    pygame.draw.rect(screen, white, [RESULTS, HEADER, width*V_CELLS+GRID_MARGIN*V_CELLS, height*H_CELLS+GRID_MARGIN*H_CELLS])

    
    if lenP > 0 and currRobotPos >= 0: #Move the robot along the path, and search for map updates
        #print(currRobotPos)
        if currRobotPos < lenP:
            if alg == 'D':
                iRob, jRob = path[currRobotPos][2]
            if alg == 'L':
                iRob, jRob = path[currRobotPos][1]

            if (iRob, jRob) != (prevIRob, prevJRob):
                changes = gridWorld.sensorSweep(iRob, jRob)
                if changes != [] and alg == 'D':
                    #print("Changes Detected")
                    DStarLite.km = DStarLite.km + DStarLite.genH(HEURISTIC, [iRob, jRob], DStarLite.sLast)
                    DStarLite.setSLast([iRob, jRob])
                    DStarLite.genH(HEURISTIC)
                for changedCell in changes:  #The actual cells whoose changes were detected by the sensor sweep
                    for x in changedCell.getNeighbours(): #The neighbours of the changed cells, the effected cells
                        targV = gridWorld.map[x[1][0]][x[1][1]]
                        if targV.getType() != 1:
                            if alg == 'D':
                                DStarLite.updateVertex(targV) #Update the difference of the changed cell and the effected cells
                            if alg == 'L':
                                #print(targV)
                                LpaStar.updateVertex(targV)
                    start = time.time()
                    if alg == 'D':
                        DStarLite.computeShortestPath()
                    if alg == 'L':
                        LpaStar.computeShortestPath()
                    end = time.time()
                    calcSecondSearch = end - start
                    path = []
                    lenP = 0
                    currRobotPos = 0   
            prevIRob, prevJRob = iRob, jRob

        else: #Handle for goal frame not in path
            #print("Else Handler")
            iRob, jRob = path[lenP - 1][1]
   
    for i in range(H_CELLS):  #Draw Cells and colours
        for j in range(V_CELLS):
            currType = gridWorld.map[i][j]._Type
            if currType == 0:
                #Traversable
                colour = grey
            elif currType == 1:
                #Blocked
                colour = black
            elif currType == 6:
                #Start Vertex
                if alg == 'L' or alg == '':
                    colour = green
                else:
                    colour = grey
            elif currType == 7:
                #Goal
                colour = darkblue
            elif currType == 9:
                #Unknown, blocked
                colour = darkgrey
            elif currType == 8:
                #Unknown, unblocked <-- Check this colour
                colour = tan
            
            if iRob == i and jRob == j:
                colour = green
            
            if alg == 'D' and DStarLite.sLast[0] == i and DStarLite.sLast[1] == j:
                colour = green 

            x1 = (GRID_MARGIN + width) * j + GRID_MARGIN + RESULTS
            y1 = (GRID_MARGIN+height)* i + GRID_MARGIN + HEADER
            COM = width//2 + x1, height//2 + y1
            #Draw background colour for square
            pygame.draw.rect(screen, colour, [x1 , y1, width-GRID_MARGIN, height-GRID_MARGIN])

            gridWorld.map[i][j].giveCoords(x1, y1, COM)
            
            #Draw Debug information
            if drawCellPos:
                img = font.render('[{},{}]    ({}, {})'.format(i, j, x1, y1), True, white, colour)
                screen.blit(img, (x1 + TEXT_OFFSET, y1 + TEXT_OFFSET))
            
            if drawHValues:
                img = font.render('h = {}'.format(gridWorld.map[i][j]._h), True, white, colour)
                screen.blit(img, (x1 + TEXT_OFFSET + (width //2 - cellWidthDiv ) ,y1 + (cellWidthDiv * 1)))
            
            if drawG:
                img = font.render('g = {}'.format(gridWorld.map[i][j]._g), True, white, colour)
                screen.blit(img, (x1 + TEXT_OFFSET + (width //2 - cellWidthDiv ) ,y1 + (cellWidthDiv * 2)))
            
            if drawRHS:
                img = font.render('rhs = {}'.format(gridWorld.map[i][j]._rhs), True, white, colour)
                screen.blit(img, (x1 + TEXT_OFFSET + (width //2 - cellWidthDiv ) ,y1 + (cellWidthDiv * 3)))
            
            if drawKeyValues:
                img = font.render('[{}, {}]'.format(gridWorld.map[i][j]._key[0], gridWorld.map[i][j]._key[1]), True, white, colour)
                screen.blit(img, (x1 + TEXT_OFFSET + (width //2 - cellWidthDiv ) ,y1 + (cellWidthDiv * 4)))
            
            if drawStatus:
                img = font.render('status = {}'.format(gridWorld.map[i][j]._status), True, white, colour)
                screen.blit(img, (x1 + TEXT_OFFSET + (width //2 - cellWidthDiv ) ,y1 + (cellWidthDiv * 5)))

            if drawConn:
                pygame.draw.circle(screen, white, COM, 15, 1) #Draw COM circle
                if currType != 1: #Storage value for if cell is blocked
                    neighbours = gridWorld.map[i][j]._neighbours
                    for x in neighbours: #Sequence, Real grid coords
                        if gridWorld.map[x[1][0]][x[1][1]]._Type == 1: #Check if neighbour is blocked
                            colour2 = red
                        else:
                            colour2 = yellow
                        COM_other = gridWorld.map[x[1][0]][x[1][1]].getCOM()
                        pygame.draw.lines(screen, colour2, False, [COM_other, COM], 2)                   

    for z in drawLocal: #Draw local connections for all selected cells (Press c to toggle cell)
        i = z[0]
        j = z[1]
        COM = gridWorld.map[i][j].getCOM()
        neighbours = gridWorld.map[i][j]._neighbours

        pygame.draw.circle(screen, white, COM, 15, 1) #Draw COM circle
        for x in neighbours: #Sequence, Real grid coords
            if gridWorld.map[x[1][0]][x[1][1]]._Type == 1 or gridWorld.map[i][j]._Type == 1: #Check if neighbour is blocked
                colour2 = red
            else:
                colour2 = yellow
            COM_other = gridWorld.map[x[1][0]][x[1][1]].getCOM()
            pygame.draw.lines(screen, colour2, False, [COM_other, COM], 2)
    

    if (drawPath and lenP == 0): #Search for path to display if algorithm has been run, but no path has been found
        if alg == 'D':
            start = DStarLite.sLast
        else:
            start = gridWorld.start
        goal = gridWorld.goal

        if alg == 'L': 
            #print("Detected LPAStar")
            minNeighbour = gridWorld.map[goal[0]][goal[1]]
            targObj = gridWorld.map[start[0]][start[1]]
        else:
            #print("Detected DStarLite")
            if gridWorld.map[start[0]][start[1]].getG() == 99:
                print("No path exists!")
                exit()
            minNeighbour = gridWorld.map[start[0]][start[1]]
            targObj = gridWorld.map[goal[0]][goal[1]]
        
        #print(minNeighbour, targObj)
        while minNeighbour != targObj:
            minV = classes.Vertex(0, -99, -99, key=[99,99])
            currCost = 99
            targVcoord = [minNeighbour.getRow(), minNeighbour.getCol()]
            #print(targVKey)
            for x in minNeighbour.getNeighbours():
                # dx = x[0][0]
                # dy = x[0][1]
                # cost = x[0][2]
                dx, dy, cost = x[0]
               
                targV = gridWorld.map[targVcoord[0] + dx][targVcoord[1] + dy]

                if targV.getType() != 1:
                    if (targV.leG(minV) and cost <= currCost):
                        #print("Smaller Found")
                        minV = targV
                        currCost = cost
                        #print(minV)
                        #target = (minNeighbour[0] + dx, minNeighbour[1] + dy)
                
            COM_other = minV.getCOM()
            COM_local = minNeighbour.getCOM()

            path.append([[COM_other, COM_local], minV.getIJ(), minNeighbour.getIJ()])
            #print(path)
            minNeighbour = minV

        if alg == 'L':
            path.reverse() #Move path from goal to start to start to goal
        lenP = len(path)
        getAndRenderResults()
        # print(path)
        # print(lenP)


    if drawPath and lenP > 0: #Render path if is been found.
        for x in path:
            pygame.draw.lines(screen, red, False, x[0], 2)

    pygame.display.flip()

pygame.quit()



