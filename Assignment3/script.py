import os
mainDirectory = 'testing'


os.system("make")
os.chdir(mainDirectory)

files = next(os.walk('.'))[2]
os.chdir('..')

for file in files:
	print("---------------------------------------------------------------------------------------------------------")
	print(file + "\n")
	file = mainDirectory + "/" + file;
	print("SCLP")
	os.system("./sclp -ast -tokens -symtab -eval -d " + file + " > sclp.out")
	os.system("./sclp -ast -symtab -icode -d " + file + " > sclp1.out")
	print("\nGIVENSCLP")
	os.system("./givensclp -ast -symtab -icode -d " + file + " > givensclp1.out")
	os.system("./givensclp -ast -tokens -symtab -eval -d " + file + " > givensclp.out")
	print("\nDIFF\n")
	os.system("diff sclp.out givensclp.out")
	os.system("diff sclp1.out givensclp1.out")
os.system("rm *.out")
os.chdir(mainDirectory)
print("\n")