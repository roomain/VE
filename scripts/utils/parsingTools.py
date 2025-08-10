import re

class TextFile :
    def __init__(self, filename):
        self.file = open(filename)
        self.lines = self.file.readlines()
        self.numLines = len(self.lines)
        self.lineIter = 0

    def seek(self, index):
        self.lineIter = index

    def nextLine(self):
        if self.lineIter >= self.numLines:
            return ""
        line = self.lines[self.lineIter]
        self.lineIter += 1
        return line
    
    def nextTrimmedLine(self):
        return self.nextLine().strip()

    def atEnd(self):
        return self.lineIter >= self.numLines
    

class BracketCounter:
    numOpen = 0
    numClose = 0

    def countBracket(self, line):
        self.numOpen += line.count('{')
        self.numClose += line.count('}')

    def valid(self):
        return self.numOpen > 0
    
    def closed(self):
        return self.numOpen == self.numClose and self.valid()
    

def removeMultilineComments(file, line):
    beforeComment = re.search("(.*)/\\*", line).group(1) 
    afterComment = re.search("\\*/(.*)", line).group(1)
    while (afterComment is None) and not file.atEnd():
        line = file.nextTrimmedLine()
        afterComment = re.search("\\*/(.*)", line).group(1)

    if beforeComment is None:        
        usedLine = afterComment
    elif afterComment is None:          
        usedLine = beforeComment
    else :
        usedLine = beforeComment + " " + afterComment
    return usedLine




def removeComments(file, line):
    if "//" in line: # single line comment
        usedLine = re.search("(.*)//", line).group(1)
    elif "/*" in line: # multiline comments
        usedLine = removeMultilineComments(file, line)
    else : # no comment
        usedLine = line
    return usedLine