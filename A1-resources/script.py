import os
mainDirectory = 'testing'


os.system("make")
os.chdir(mainDirectory)

os.system("rm *.tocs")
os.system("rm *.toks")
os.system("rm *.spim")
os.system("rm *.mips")


files = next(os.walk('.'))[2]
os.chdir('..')
# print(files)


for file in files:
	print(file + "\n")
	file = "testing/" + file;
	print("YOURSCLP")
	os.system("./yoursclp -tokens " + file)
	os.system("mv " + file + ".toks " + file + ".tocs" )
	os.system("mv " + file + ".spim " + file + ".mips" )
	print("\nSCLP")
	os.system("./sclp -tokens " + file)
	os.system("diff "  + file + ".toks " + file + ".tocs")
	os.system("diff "  + file + ".mips " + file + ".spim")


os.chdir(mainDirectory)
print("\n")