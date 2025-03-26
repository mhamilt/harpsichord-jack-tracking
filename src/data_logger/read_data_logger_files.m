clear all
close all
 
%% Read Byte Data 
bufferSize = 40000; % size of data in bytes

filename = './'
fileID = fopen(filename );
data = fread(fileID,[4,40000],'uint16')';
fclose(fileID);

plot(data);
