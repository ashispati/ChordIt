import os

dir = 'D:/Academics/Audio Software Engg/LeadSheets/reduced_chords/'
out_dir = 'D:/Academics/Audio Software Engg/LeadSheets/'

files = os.listdir(dir)

def genDictofChords():
	chords = {'start': 0, 'end': 1}
	count = 2
	for file in files:
		f = open(dir+file)
		for line in f:
			chord = line.strip('\n')
			if chord in chords:
				continue
			else:
				chords[chord] = count
				count = count + 1
		f.close()
	return chords

chords = genDictofChords()
print(chords)

f_out = open('chord_transitions.txt','w')
for file in files:
	f = open(dir+file)
	transitions = []
	transitions.append(chords['start'])
	for line in f:
		chord = line.strip('\n')
		transitions.append(chords[chord])
	f.close()
	transitions.append(chords['end'])
	f_out.write(','.join(map(str,transitions)))
	f_out.write('\n')
f_out.close()