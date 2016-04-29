observations_file = 'D:\Git Repos\ChordIt\Externals\python\note_observations_major.txt';

dim_1 = 62; % set to 45 for minor
dim_2 = 12;

emission_mat = zeros(dim_1, dim_2);

fid = fopen(observations_file);
line = fgetl(fid);
while ischar(line)
    line = strsplit(line,',');
    line = str2double(line);
    line = line+1;
    if numel(line) == 1
        line = fgetl(fid);
        continue;
    end
    for i = 2:numel(line)
        emission_mat(line(1), line(i)) = emission_mat(line(1), line(i)) + 1;
    end
    line = fgetl(fid);
end

emission_mat(emission_mat == 0) = 0.1;

for i = 1:dim_1
emission_mat(i,:) = emission_mat(i,:)./norm(emission_mat(i,:),1);
end