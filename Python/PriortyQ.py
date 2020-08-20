class PriorityQ:
    def __init__(self):
        self.U = []
        self.lenU = 0
        self.maxU = 0
    
    def _smallestKey(self, s, sPrime):
        if s[0] < sPrime[0]:
            return s
        elif s[0] == sPrime[0]:
            if s[1] < sPrime[1]:
                return s
            else:
                return sPrime
        else:
            return sPrime
    
    def _isSmaller(self, s, sPrime):
        if s[0] < sPrime[0]:
            return True
        elif s[0] == sPrime[0]:
            if s[1] < sPrime[1]:
                return True
            else:
                return False
        else:
            return False
    
    def _plusLen(self, inc=1):
        self.lenU += inc
        if self.lenU > self.maxU:
            self.maxU = self.lenU


    def insert(self, s, k):
        if self.lenU > 0:
            i = 0
            while  (i < self.lenU) and self._isSmaller(self.U[i].getKey(), k):
                i += 1
            self.U.insert(i, s)
            self._plusLen()
        
        else:
            self.U.append(s)
            self._plusLen()

    def topKey(self):
        #print(self.U)
        if self.lenU > 0:
            return self.U[0].getKey()
        else:
            return [99,99]
    
    def pop(self):
        if self.lenU > 0:
            self.lenU -= 1
            return self.U.pop(0)

    def update(self, s, k):
        if self.lenU > 0:
            i = 0
            while self.U[i] != s:
                i += 1
            if i != k:
                self.U.remove(s)
                self.lenU -= 1
                self.insert(s, k)
    
    def remove(self, s):
        self.U.remove(s)
        self.lenU -= 1      
    
    def __repr__(self):
        if self.lenU > 0:
            printList = [x.getPrintable() for x in self.U]
            # row, obj in row, line of obj
            # print(printList[0][0][0])
            for line in range(7): #For each obj in row
                for j in range(self.lenU): #For each line in obj
                    #print(line, j)
                    print(printList[j][line], end='   ')
                print()
        return ''

    # def __getitem__(self):
    #     outList = []
    #     for x in self.U:
    #         outList.append(x)
    #     return outList

    # def __iter__(self):
    #     return [x for x in self.U]
    
    def __contains__(self, item):
        for x in self.U:
            if item == x:
                return True
        return False
        



        

if __name__ == "__main__":
    import classes
    U = PriorityQ()
    U.insert(classes.Vertex(3,0,0, key=[1,2]), [1,2])
    U.insert(classes.Vertex(1,0,0, key=[0,1]), [0,1])
    U.insert(classes.Vertex(2,0,0, key=[1,1]), [1,1])
    v = classes.Vertex(0,0,0, key=[10,1])
    U.insert(v, [10,1])
    U.update(v, [0,0])
    print(v in U)
    print(U)
