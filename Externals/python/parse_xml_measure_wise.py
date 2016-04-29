import xml.etree.ElementTree as ET
import os

dir = "D:/Academics/Audio Software Engg/LeadSheets/Wikifonia/xml/"

filenames = os.listdir(dir)

notes_dict = {'B#': 0, 'C': 0, 'C#': 1, 'Db': 1, 'D': 2, 'D#': 3, 'Eb': 3, 'E': 4, 'Fb': 4, 'E#': 5, 'F': 5, 'F#': 6, 'Gb': 6, 'G': 7, 'G#': 7, 'Ab': 8, 'A': 9, 'A#': 10, 'Bb': 10, 'B': 11, 'Cb': 11}

transpose_factor = {'-1': -5, '-2': 2, '-3': -3, '-4': 4, '-5': -1, '-6': 6, '-7': 1, '7': -1, '6': 6, '5': 1, '4': -4, '3': 3, '2': -2, '1': 5, '0': 0}

def getChord(child, key):
	for r in child.findall('root'):
		step = r.find('root-step').text
		alt = r.find('root-alter').text if (r.find('root-alter') is not None) else ''
	kind = child.find('kind') # Check condition if it exists
	#kind = kind.get('text') if (kind is not None) else ''
	if(kind is not None):
		if(kind.get('text') is not None):
			kind = kind.get('text')
		else:
			kind = kind.text if kind.text is not None else ''
	else:
		kind = ''
		
	# Reduce chord step and alteration to number from notes dictionary
	if(alt == '1'):
		step = step+'#'
	elif(alt == '-1'):
		step = step+'b'
	step = notes_dict[step]
	transpose = transpose_factor[key]
	step = (step+transpose)%12
	chord = str(step)+','+kind
	return chord
	
def getKey(song):
	mode = []
	fifths = []
	c = 0
	for key in song.iter('key'):
		#mode[c] = key.find('mode').text if (key.find('mode') is not None) else ''
		#fifths[c] = key.find('fifths').text if (key.find('fifths') is not None) else ''
		mode.append(key.find('mode').text if (key.find('mode') is not None) else '')
		fifths.append(key.find('fifths').text if (key.find('fifths') is not None) else '')
		c = c + 1
	return mode, fifths	

counter = 1
for file in filenames:
	tree = ET.parse(dir+file)
	root = tree.getroot()
	key_mode, key_fifths = getKey(root)
	part_num = 0
	if(len(key_mode) == 0 or len(key_fifths) == 0):
		continue
	else:
		# Segment based on 'part' tag
		for part in root.iter('part'):
			first_chord = True
			f_chords = open('D:/Academics/Audio Software Engg/LeadSheets/chords/'+str(counter)+'_chord.txt','w')
			f_notes = open('D:/Academics/Audio Software Engg/LeadSheets/notes/'+str(counter)+'_notes.txt','w')
			# Write the key for the part in the first line of the chord file
			f_chords.write(key_fifths[part_num]+ ' ' + key_mode[part_num]+ '\n')
			# Check each measure for the presence of chords
			notes = []
			chord = ''
			# change the line in the notes and chord file when measure changes and print the same chord if chord spans multiple measures
			for measure in root.iter('measure'):
				for child in measure:
					if(child.tag == 'harmony'):
						if(len(notes) != 0 and chord != ''):
							f_chords.write(chord+'\n')
							f_notes.write(','.join(map(str,notes)))
							f_notes.write('\n')
						chord = getChord(child, key_fifths[part_num])
						#write chord to file
						notes = []
					elif(child.tag == 'note'):
						pitch = child.find('pitch')
						if(pitch is not None):
							note = pitch.find('step').text if (pitch.find('step') is not None) else ''
							alter = pitch.find('alter')
							if(alter is not None):
								if(alter.text == '1'):
									note = note+'#'
								elif(alter.text == '-1'):
									note = note+'b'
							note = notes_dict[note]
							transpose = transpose_factor[key_fifths[part_num]]
							note = (note+transpose)%12
							notes.append(note)
				########################
				if(len(notes) != 0 and chord != ''):
					f_chords.write(chord+'\n')
					f_notes.write(','.join(map(str,notes)))
					f_notes.write('\n')
					notes = []
				########################
			part_num = part_num + 1
			counter = counter + 1
			f_chords.close()
			f_notes.close()