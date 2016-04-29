transitions_file = 'D:\Git Repos\ChordIt\Externals\python\chord_transitions.txt';

dim = 62; % set to 45 for minor

transition_mat = zeros(dim);
fid = fopen(transitions_file);
line = fgetl(fid);
while ischar(line)
    line = strsplit(line,',');
    line = str2double(line);
    line = line+1;
    if numel(line) == 2
        line = fgetl(fid);
        continue;
    end
    t_mat = getTransitionMatrix(line, 1, 62);
    transition_mat = transition_mat + t_mat;
    line = fgetl(fid);
end