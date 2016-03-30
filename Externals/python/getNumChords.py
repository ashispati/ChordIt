import xml.etree.ElementTree as ET
import os

dir = "D:/Academics/Audio Software Engg/LeadSheets/Wikifonia/xml"

filenames = os.listdir(dir)

num_files_with_chords = 0
num_chords = 0

flag = 0
for file in filenames:
	tree = ET.parse(file)
	root = tree.getroot()
	for h in root.iter('harmony'):
		if h:
			num_chords = num_chords + 1
			flag = 1
	if flag == 1:
		num_files_with_chords = num_files_with_chords + 1
		flag = 0
print("Number of files with chords: "+str(num_files_with_chords)+"\nNumber of chords: "+str(num_chords)+"\n")