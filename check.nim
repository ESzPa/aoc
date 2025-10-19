import os
import algorithm
import strutils

const
    start_year = 2015
    latest_year = 2024

proc printnice(text: string, color: string) =
    var code = ""
    case color.toLowerAscii()
    of "black": code = "\x1b[30m"
    of "red": code = "\x1b[31m"
    of "green": code = "\x1b[32m"
    of "yellow": code = "\x1b[33m"
    of "blue": code = "\x1b[34m"
    of "magenta": code = "\x1b[35m"
    of "cyan": code = "\x1b[36m"
    of "white": code = "\x1b[37m"
    else: code = "\x1b[0m"
    stdout.write(code & text & "\x1b[0m\n")   

proc checkYear(dir: string) =
    var dirs: seq[string] = @[]

    for kind, path in walkDir(dir):
        let name = splitPath(path)[1]
        if kind == pcDir:
            dirs.add(name)
    
    sort(dirs)

    var dayNums: seq[float] = @[]
    for name in dirs:
        try:
            dayNums.add(parseFloat(name))
        except:
            discard    

    var expectedDay = 1.0
    var i = 0
    while expectedDay <= float(31):
        if i < dayNums.len and dayNums[i] == expectedDay:
            printnice("    " & $dayNums[i], "blue")
            i.inc()
        else:
            printnice("    " & $expectedDay, "red")
            expectedDay += 0.5

for year in start_year .. latest_year:
    let dir: string = "./" & $year
    if dirExists(dir):
        printnice($year & "/", "green")
        checkYear(dir)
    else:
        printnice($year & "/", "red")
