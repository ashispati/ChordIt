import xml.etree.ElementTree as ET
import os

def getChord(child):
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
	chord = step+','+alt+','+kind
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
	
dir = "D:/Academics/Audio Software Engg/LeadSheets/Wikifonia/xml/"

filenames = os.listdir(dir)

notes_dict = {'B#': 1, 'C': 1, 'C#': 2, 'Db': 2, 'D': 3, 'D#': 4, 'Eb': 4, 'E': 5, 'Fb': 5, 'E#': 6, 'F': 6, 'F#': 7, 'Gb': 7, 'G': 8, 'G#': 9, 'Ab': 9, 'A': 10, 'A#': 11, 'Bb': 11, 'B': 12, 'Cb': 12, '': 0}

counter = 1
for file in filenames:
	print(file)
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
			
			
			# change the line in the notes and chord file when measure changes and print the same chord if chord spans multiple measures
			for measure in root.iter('measure'):
				for child in measure:
					if(child.tag == 'harmony'):
						if(first_chord):
							first_chord = False
						else:
							f_notes.write(','.join(map(str,notes)))
							f_notes.write('\n')
						chord = getChord(child)
						#write chord to file
						f_chords.write(chord+'\n')
						notes = []
					if(child.tag == 'note'):
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
							notes.append(note)
			part_num = part_num + 1
			counter = counter + 1