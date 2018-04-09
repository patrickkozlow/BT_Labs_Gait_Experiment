%Combining KneeAngles+GaitCycle For Person 1 Sequence 1-5
%By: Patric Kozlow
%Biometric Technology Labs, University of Calgary

%% GaitCycle
% Change to load proper sequence
clear;
close all;
min_angle = 150;

for seq_num = 1:5
    
subsec = ['seq' num2str(seq_num) '.txt'];
sequence = load(subsec);       %Loading subsection of database

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

% smoothing JRD
JRD = smooth(JRD);
JRDT = JRD;

% looking at frames 10-80
JRDT(1:10) = [0];
JRDT(80:size(JRD,1)) = [0];

% plotting the filtered graph
hFig = figure(seq_num);
set(hFig, 'Position', [600,200,600,600]);
subplot(2,1,1);
plot(JRD);
axis([0 100 0 0.8]);

% Finding the peaks to be used for finding gait cycle
[peaks,loc] = findpeaks(JRDT);
A = peaks >= 0.4;
FiltPeaks = peaks(A);
FiltLoc = loc(A);
FrameStart = FiltLoc(1);
FrameEnd = FiltLoc(3);

% Add lines to represent where gait cycle starts and stops
line([FrameStart FrameStart], get(gca, 'YLim'),'Color',[.5 .5 .5],'LineStyle','--');
line([FrameEnd FrameEnd], get(gca, 'YLim'),'Color',[.5 .5 .5],'LineStyle','--');

% Finding Gait Cycle
GaitCycleSpeed = (FrameEnd - FrameStart)/30;
disp(['The Average Gait Cycle Speed for sequence ' num2str(seq_num) ' is: ' num2str(GaitCycleSpeed)]); 

%% KneeAngles
%Finding the geometric positions of the left ankle
EndPoint = size(sequence,1);   %Finding Number of rows
n = 16;     %Numeric value of Ankle Left
a = 1;      

while n <= EndPoint  
   AnkleLeft(a,:) = sequence(n,:);
   n = n+20;
   a = a+1;
end

n = 14;     %Numeric value of Hip Left
a = 1;      

while n <= EndPoint  
   HipLeft(a,:) = sequence(n,:);
   n = n+20;
   a = a+1;
end

n = 15;     %Numeric value of Knee Left
a = 1;      

while n <= EndPoint  
   KneeLeft(a,:) = sequence(n,:);
   n = n+20;
   a = a+1;
end

n = 16;     %Numeric value of Ankle Left
a = 1;      

while n <= EndPoint  
   AnkleLeft(a,:) = sequence(n,:);
   n = n+20;
   a = a+1;
end

n = 13;     %Numeric value of Hip Right
a = 1;      

while n <= EndPoint  
   HipRight(a,:) = sequence(n,:);
   n = n+20;
   a = a+1;
end

n = 18;     %Numeric value of Knee Right
a = 1;      

while n <= EndPoint  
   KneeRight(a,:) = sequence(n,:);
   n = n+20;
   a = a+1;
end

n = 19;     %Numeric value of Ankle Right
a = 1;      

while n <= EndPoint  
   AnkleRight(a,:) = sequence(n,:);
   n = n+20;
   a = a+1;
end

%Finding JRA for Left Knee
m = 1;
while m <= size(AnkleLeft,1)
T1 = AnkleLeft(m,:) - KneeLeft(m,:);
T2 = HipLeft(m,:) - KneeLeft(m,:);

normA = sqrt((T1(1)^2) + (T1(2)^2) + (T1(3)^2));
normB = sqrt((T2(1)^2) + (T2(2)^2) + (T2(3)^2));

anglerad = acos((T1(1)*T2(1) + T1(2)*T2(2) + T1(3)*T2(3))/(normA*normB));
angledegL(m) = anglerad*(180/pi);
m = m+1;
end

angledegL = transpose(angledegL);
angledegL = smooth(angledegL);
subplot(2,1,2);
plot(angledegL);
hold on

%Finding JRA for Right Knee
m = 1;
while m <= size(AnkleRight,1)
T1 = AnkleRight(m,:) - KneeRight(m,:);
T2 = HipRight(m,:) - KneeRight(m,:);

normA = sqrt((T1(1)^2) + (T1(2)^2) + (T1(3)^2));
normB = sqrt((T2(1)^2) + (T2(2)^2) + (T2(3)^2));

anglerad = acos((T1(1)*T2(1) + T1(2)*T2(2) + T1(3)*T2(3))/(normA*normB));
angledegR(m) = anglerad*(180/pi);
m = m+1;
end

angledegR = transpose(angledegR);
angledegR = smooth(angledegR);

angledegR_T = angledegR(FrameStart:FrameEnd);
angledegL_T = angledegL(FrameStart:FrameEnd);

% Finding lowest point in the gait cycle
MinangledegR = min(angledegR_T);
MinangledegL = min(angledegL_T);

Status = 'Unknown';

if MinangledegR >= min_angle
    Status = 'Right';
end

if MinangledegL >= min_angle
    Status = 'Left';
end

if MinangledegR && MinangledegL >= min_angle
    Status = 'Left and Right';
end

if MinangledegR && MinangledegL <= min_angle
    Status = 'No Limp';
end

% EXAMPLE TRAINING SET
% Sample     Min_Right_Knee     Min_Left_Knee     Limp(R/L/N)
% 1          ###                ###               L/R/N
% 2          ###                ###               L/R/N
% 3          ###                ###               L/R/N
% ...

Data(seq_num,:) = [seq_num MinangledegR MinangledegL];
Result(seq_num,:) = [Status];

if seq_num == 5
Data = array2table(Data,'VariableNames',{'Sample','Min_R_Knee_Angle','Min_L_Knee_Angle'});
Result = cellstr(Result);
Result = cell2table(Result,'VariableNames',{'Status'});
TrainingData = [Data Result];
end

plot(angledegR);
line([FrameStart FrameStart], [0 200],'Color',[.5 .5 .5],'LineStyle','--');
line([FrameEnd FrameEnd], [0 200],'Color',[.5 .5 .5],'LineStyle','--');
legend('Left Knee Angle','Right Knee Angle','Location','southwest');
axis([0 100 0 200]);

end


