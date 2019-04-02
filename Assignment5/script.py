import os

def foofile(file):

	print("---------------------------------------------------------------------------------------------------------")
	print(file + "\n")
	print("OUR SCLP")
	os.system("./sclp -d " + file + " > sclp.s")
	# os.system("./sclp -icode -d " + file + " > sclp1.out")
	print("\nGIVEN SCLP")
	os.system("./givensclp -d " + file + " > givensclp.s")
	# os.system("./givensclp -icode -d " + file + " > givensclp1.out")
	os.system("spim -file sclp.s > sclp.out")
	os.system("spim -file givensclp.s > givensclp.out")

	print("\nDIFF\n")
	os.system("diff sclp.out givensclp.out")	

	# os.system("diff sclp1.out givensclp1.out")
	os.system("rm *.out")
	os.system("rm *.s")
	print("\n")

def foo(mainDirectory):
	os.chdir(mainDirectory)
	files = next(os.walk('.'))[2]
	os.chdir('..')

	for file in files:
		print("---------------------------------------------------------------------------------------------------------")
		print(file + "\n")
		file = mainDirectory + "/" + file;
		print("OUR SCLP")
		os.system("./sclp -d " + file + " > sclp.s")
		# os.system("./sclp -icode -d " + file + " > sclp1.out")
		print("\nGIVEN SCLP")
		os.system("./givensclp -d " + file + " > givensclp.s")
		# os.system("./givensclp -icode -d " + file + " > givensclp1.out")
		os.system("spim -file sclp.s > sclp.out")
		os.system("spim -file givensclp.s > givensclp.out")

		print("\nDIFF\n")
		os.system("diff sclp.out givensclp.out")
		# os.system("diff sclp1.out givensclp1.out")
		input()
	os.system("rm *.out")
	os.system("rm *.s")
	print("\n")
	print("#########################################################################################################")
	print("#########################################################################################################")
	print("\n")

os.system("make")

directories = ["mytesting"]#, "testing", "errortesting"]
file = "test.c"
# foofile(file)

for dir in directories:
	foo(dir)