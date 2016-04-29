import os

#type = 'major'
type = 'minor'

dir = 'D:/Academics/Audio Software Engg/LeadSheets/' + type + '/chords/'

output_dir = 'D:/Academics/Audio Software Engg/LeadSheets/' + type + '/reduced_chords/'
filenames = os.listdir(dir)

# maps all chord types to the 5 triad types: major, minor, augmented, diminished and suspended
chord_type_dict = {'ma13': 'major', 'dominant-11th': 'major', 'diminished-seventh': 'dim', 'add9': 'major', 'M9': 'major', '5': 'major', 'maj': 'major', 'm7add11': 'minor', '7#9b13': 'major', '7b9': 'major', 'major-seventh': 'major', '': 'major', 'm#5': 'minor', 'Maj9': 'major', 'min7': 'minor', 'susb9': 'sus', 'major-sixth': 'major', '7b5b9': 'major', 'm+': 'aug', 'dominant-ninth': 'major', 'dominant-13th': 'major', 'm7': 'minor', 'Maj7#5': 'major', 'm11b5': 'minor', 'suspended-second': 'sus', '13sus4': 'sus', '7sus': 'sus', '7#11': 'major', '7)': 'major', 'E': 'major', 'diminished': 'dim', 'm9': 'minor', '6': 'major', 'madd2': 'minor', 'minMaj7': 'minor', 'sus4': 'sus', '13#11': 'major', 'mmaj7': 'minor', 'ma7': 'major', 'mi13': 'minor', 'mi7': 'minor', 'm2': 'minor', 'min6': 'minor', 'Maj7#11': 'minor', 'maj9': 'major', 'm6': 'minor', 'F': 'major', ' ': 'major', '7b9b13': 'major', 'mi': 'minor', 'aug': 'aug', '13sus': 'sus', 'A/B': 'major', 'mi(ma7)': 'minor', 'minor': 'minor', '6/9': 'major', '+': 'aug', 'major-ninth': 'major', 'maj13': 'major', '/A': 'major', '13b9': 'major', ' m7': 'minor', ' m': 'minor', 'C': 'major', 'Db': 'major', 'min': 'minor', '9#11': 'major', 'm(Maj7)': 'minor', 'Ma7': 'major', 'min(maj7)': 'minor', '7susb9': 'sus', 'minor-ninth': 'minor', '6add9': 'major', 'Maj9#11': 'major', 'madd9': 'minor', 'mi7b5': 'minor', 'maug': 'aug', '9sus4': 'sus', 'm7  A7': 'minor', '13)': 'major', 'augmented': 'aug', 'M6': 'major', 'M7': 'major', '7b9#11': 'major', 'sus2': 'sus', 'minor-sixth': 'minor', 'N.C.': 'major', 'm(maj7)': 'minor', 'Maj6': 'major', '7b5#9': 'major', 'sus47': 'sus', 'mi#5': 'minor', '°': 'dim', ' sus7': 'sus', 'Maj7b5': 'major', 'minor-11th': 'minor', 'min/G': 'minor', 'mi6': 'minor', 'o7': 'dim', 'min9': 'minor', '9sus': 'sus', ' dim': 'dim', 'MA7': 'major', '13': 'major', 'mi69': 'minor', '7SUS': 'sus', ' dim7': 'dim', '11': 'major', ' 7': 'major', 'Ab/C': 'major', 'augmented-seventh': 'aug', 'm11': 'minor', 'D': 'major', '7#5': 'major', '9b5': 'major', 'm7b9': 'minor', 'Bb/C': 'major', 'Maj13': 'major', 'ma9': 'major', 'sus': 'sus', 'half-diminished': 'dim', 'Maj7': 'major', 'm13': 'minor', '69': 'major', 'add2': 'major', 'm7b5': 'minor', 'dim': 'dim', 'dim7': 'dim', 'suspended-fourth': 'sus', '9#5': 'major', '9+': 'aug', '-7': 'minor', 'm9Maj7': 'minor', '7alt': 'major', 'o': 'dim', '(9)': 'major', 'aug9': 'aug', 'm69': 'minor', '(6)': 'major', 'power': 'major', '4': 'major', 'maj7': 'major', 'mi9': 'minor', 'mMaj7': 'minor', '7#9': 'major', '7#9#11': 'major', '7#5b9': 'major', 'm': 'minor', 'm9b5': 'minor', '7b5': 'major', '#59': 'major', '13b5': 'major', '7+': 'aug', 'Maj': 'major', '7sus4': 'sus', 'G/A': 'major', '-': 'minor', 'minor-seventh': 'minor', '7b13': 'major', 'aug7': 'aug', 'major': 'major', 'mi11': 'minor', '7#5#9': 'major', '9': 'major', 'dominant': 'major', 'major-minor': 'major', '7': 'major', '2': 'major', 'm7#5': 'minor'}


for file in filenames:
	path = dir+file
	f = open(path,'r')
	lines = list(f)
	f_out = open(output_dir+file.split('.')[0]+'reduced.txt', 'w')
	for i in range(1, len(lines)):
		chord = lines[i]
		chord = chord.strip('\n')
		kind = chord.split(',')[1]
		chord = chord.split(',')[0] + chord_type_dict[kind]
		f_out.write(chord+'\n')
	f_out.close()