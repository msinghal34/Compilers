import os


def foofile(file):

    print("---------------------------------------------------------------------------------------------------------")
    print(file + "\n")
    print("OUR SCLP")
    os.system("./sclp -d " + file + " > sclp.spim")
    os.system("spim -file sclp.spim > sclp.out")
    # os.system("./sclp -icode -d " + file + " > sclp1.out")
    print("\nGIVEN SCLP")
    os.system("./givensclp -d " + file + " > givensclp.spim")
    os.system("spim -file givensclp.spim > givensclp.out")
    # os.system("./givensclp -icode -d " + file + " > givensclp1.out")

    print("\nDIFF\n")
    os.system("diff sclp.out givensclp.out")

    # os.system("diff sclp1.out givensclp1.out")
    os.system("rm *.out")
    os.system("rm *.s")
    print("\n")


def foo(mainDirectory):
    os.chdir(mainDirectory)
    os.system("rm *.out")
    os.system("rm *.ic")
    os.system("rm *.spim")
    files = next(os.walk('.'))[2]
    os.chdir('..')

    for file in files:
        print("---------------------------------------------------------------------------------------------------------")
        print(file + "\n")
        file = mainDirectory + "/" + file
        print("OUR SCLP")
        # os.system("./sclp -icode " + file )
        # os.system("mv " + file + ".ic " + file + "my.ic ")
        os.system("./sclp -d " + file + " > sclp.spim")
        os.system("spim -file sclp.spim > sclp.out")
        print("\nGIVEN SCLP")
        # os.system("./givensclp -icode " + file )
        os.system("./givensclp -d " + file + " > givensclp.spim")
        os.system("spim -file givensclp.spim > givensclp.out")
        
        print("\nDIFF\n")
        # os.system("diff " + file + "my.ic " + file + ".ic ")
        os.system("diff sclp.out givensclp.out")
        input()
    os.system("rm *.out")
    os.system("rm *.spim")
    os.system("rm *.ic")
    print("\n")
    print("#########################################################################################################")
    print("#########################################################################################################")
    print("\n")

os.system("make")
directories = ["finaltesting"]#, "testing", "mytesting"]
for dir in directories:
    foo(dir)
os.system("rm *.out")
os.system("rm *.spim")
os.system("rm *.ic")