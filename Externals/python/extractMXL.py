import os
import zipfile

dir = "D:/Academics/Audio Software Engg/LeadSheets/Wikifonia/"

filenames = os.listdir(dir)

i = 1
for file in filenames:
	if file.endswith(".mxl"):
		newfile = file.split(".")[0] + ".xml"
		mxlfile = zipfile.ZipFile(dir+file)
		for name in mxlfile.namelist():
			if name == "musicXML.xml":
				mxlfile.extract(name,dir+"xml/")
				os.rename(dir+"xml/musicXML.xml", dir+"xml/"+str(i)+".xml")
				i = i+1