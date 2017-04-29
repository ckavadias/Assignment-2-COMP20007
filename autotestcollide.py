Python 3.4.3 (v3.4.3:9b73f1c3e601, Feb 23 2015, 02:52:03) 
[GCC 4.2.1 (Apple Inc. build 5666) (dot 3)] on darwin
Type "copyright", "credits" or "license()" for more information.
>>> WARNING: The version of Tcl/Tk (8.5.9) in use may be unstable.
Visit http://www.python.org/download/mac/tcltk/ for current information.
from subprocess import Popen, PIPE
from sys import stdout, stderr, argv, exit
import sys

TEST_FILE_NAME = "1element.txt"

with open(TEST_FILE_NAME, "w") as F:
    F.write("1")

for method in ('c', 'd'):
    for seed in range(1, 101):
        p = Popen(["bash", "./runcollide.sh", method, str(seed)],
                  stdout=PIPE, stderr=None)
        cnt = 0
        lines = p.stdout.read()
        lines = lines.decode("cp437").splitlines()
        for line in lines:
            if "Looking for:" in line:
                cnt += 1
                s = line.split("starting in ")[1]
                s = s.split()[0]
                s = s[:-1]
                if s != '0':
                    print("ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", line)
                    print(s)
                    exit(1)
        if cnt == 0:
            print("CNT == 0 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
            exit(1)
        print("Passed!", method, "seed:", seed)
