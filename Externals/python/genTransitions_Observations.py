import os
import operator
import re

type = 'major'
#type = 'minor'

notes_dict = {0: 'C',1: 'C#',2: 'D',3: 'D#', 4: 'E',5: 'F',6: 'F#',7: 'G',8: 'G#',9: 'A',10: 'Bb',11: 'B'}

chords_dir = 'D:/Academics/Audio Software Engg/LeadSheets/'+ type + '/reduced_chords/'
notes_dir = 'D:/Academics/Audio Software Engg/LeadSheets/' + type + '/notes/'

print(chords_dir)
chordfiles = os.listdir(chords_dir)
notefiles = os.listdir(notes_dir)

def genDictofChords():
	chords = {'start': 0, 'end': 1}
	count = 2
	for file in chordfiles:
		f = open(chords_dir+file)
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
# sorted_x = sorted(chords.items(), key=operator.itemgetter(1))

# c= 0
# for i in sorted_x:
	# if c >= 2:
		# num = re.findall(r'\d+', i[0])
		# chord = i[0].strip(str(num))
		# chord = notes_dict[int(num[0])]+' '+chord
		# print(chord)
	# c = c+1
	
f_out_c = open('chord_transitions_'+ type + '.txt','w')
f_out_n = open('note_observations_'+ type + '.txt','w')
for i, chordfile in enumerate(chordfiles):
	f_n = open(notes_dir + notefiles[i])
	f_c = open(chords_dir+chordfile)
	chordlines = f_c.readlines()
	notelines = f_n.readlines()
	transitions = []
	transitions.append(chords["start"])
	for j, line in enumerate(chordlines):
		chord = line.strip("\n")
		observations = str(chords[chord])+ "," + notelines[j].strip("\n")
		transitions.append(chords[chord])
		f_out_n.write(observations+"\n")
	f_c.close()
	transitions.append(chords["end"])
	f_out_c.write(','.join(map(str,transitions)))
	f_out_c.write('\n')
f_out_c.close()