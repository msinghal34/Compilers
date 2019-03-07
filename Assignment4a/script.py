import os
mainDirectory = 'mytesting'


os.system("make")
os.chdir(mainDirectory)

files = next(os.walk('.'))[2]
os.chdir('..')

for file in files:
	print("---------------------------------------------------------------------------------------------------------")
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