% Sorting UPCV Database for Cells 16 & 19
% Ankle Left and Ankle Right

% Change to load proper sequence
clear;
sequence = load('seq3.txt');       %Loading subsection of database

%Finding the geometric positions of the left ankle
EndPoint = size(sequence,1);   %Finding Number of rows
n = 16;     %Numeric value of joint
a = 1;      

while n <= EndPoint  
   AnkleLeft(a,:) = sequence(n,:);
   n = n+20;
   a = a+1;
end

%Finding the geometric positions of the right ankle
n = 19;
a = 1;
while n <= EndPoint  
   AnkleRight(a,:) = sequence(n,:);
   n = n+20;
   a = a+1;
end

%Finding the JRD (Joint Relative Distance) between the ankles
%A.K.A The gait cycle
m = 1;
EndPoint = size(AnkleLeft,1);
while m <= EndPoint
T1 = (AnkleLeft(m,1) - AnkleRight(m,1))^2;
T2 = (AnkleLeft(m,2) - AnkleRight(m,2))^2;
T3 = (AnkleLeft(m,3) - AnkleRight(m,3))^2;
JRD(m) = sqrt(T1+T2+T3);
m = m+1;
end
JRD = transpose(JRD);

%Smoothing & Plotting Gait Cycle
JRD = smooth(JRD);
plot(JRD);

