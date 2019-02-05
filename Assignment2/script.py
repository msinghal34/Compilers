import os
mainDirectory = 'testing'


os.system("make")
os.chdir(mainDirectory)

os.system("rm *.ast")
os.system("rm *.spim")
os.system("rm *.toks")


files = next(os.walk('.'))[2]
os.chdir('..')

for file in files:
	print(file + "\n")
	file = "testing/" + file;
	print("SCLP")
	os.system("./sclp -tokens -ast -d " + file + " > sclp.out")
	print("\nGIVENSCLP")
	os.system("./givensclp -tokens -ast -d " + file + " > givensclp.out")
	print("\nDIFF\n")
	os.system("diff sclp.out givensclp.out")

os.chdir(mainDirectory)
print("\n")