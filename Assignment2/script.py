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
	file = mainDirectory + "/" + file;
	print("SCLP")
	os.system("./sclp -tokens -ast -symtab -eval -d " + file + " > sclp.out")
	print("\nGIVENSCLP")
	os.system("./givensclp -tokens -ast -symtab -eval -d " + file + " > givensclp.out")
	print("\nDIFF\n")
	os.system("diff sclp.out givensclp.out")
os.system("rm *.out")
os.chdir(mainDirectory)
print("\n")