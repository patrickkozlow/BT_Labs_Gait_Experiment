%Combining KneeAngles+GaitCycle For Whole Database DEMO VERSION
%By: Patric Kozlow
%Biometric Technology Labs, University of Calgary

%% Preliminary
clear;                  % Clearing Workspace
close all;              % Closing all figures
min_angle = 150;        % Setting the range(degrees) for determining limp
fig_num = 1;
run = 1;

% Loading Database
load('UPCVgait.mat');      

%% Gait cycle
% Loading Subsections of DataBase
% Where: j = person # and k = sequence #
for j = 1:30
    for k = 1:5
sequence = cell2mat(upcv(j,k));

% Joint ID
% 1. Head            6. Wrist Left 		11. Spine       16. Ankle Left
% 2. Shoulder Center 7. Hand Left 		12. Hip Center 	17. Foot Left
% 3. Shoulder Right  8. Elbow Right 	13. Hip Right 	18. Knee Right
% 4. Shoulder Left 	 9. Wrist Right 	14. Hip Left 	19. Ankle Right
% 5. Elbow Left 	10. Hand Right 	    15. Knee Left 	20. Foot Right

% Finding the geometric positions of the left ankle using the above table
EndPoint = size(sequence,1);    % Finding Number of rows
n = 16;                         % Numeric value of joint
a = 1;      
while n <= EndPoint  
   AnkleLeft(a,:) = sequence(n,:);
   n = n+20;
   a = a+1;
end

% Finding the geometric positions of the right ankle
n = 19;
a = 1;
while n <= EndPoint  
   AnkleRight(a,:) = sequence(n,:);
   n = n+20;
   a = a+1;
end

% Finding the JRD (Joint Relative Distance) between the ankles
% Also known as the gait cycle
m = 1;
EndPoint = size(AnkleLeft,1);
while m <= EndPoint
T1 = (AnkleLeft(m,1) - AnkleRight(m,1))^2;
T2 = (AnkleLeft(m,2) - AnkleRight(m,2))^2;
T3 = (AnkleLeft(m,3) - AnkleRight(m,3))^2;
JRD(m) = sqrt(T1+T2+T3);
m = m+1;
end

% Transposing to make it a Nx1 vector
JRD = transpose(JRD);

% smoothing JRD
JRD = smooth(JRD);

%% plotting the filtered graph (DEMO)
hFig = figure(fig_num);
fig_num = fig_num + 1;
set(hFig, 'Position', [600,200,600,600]);
%subplot(2,1,1);
plot(JRD,'LineWidth',4);
axis([0 100 0 0.8]);
ylabel('\Delta between ankles');
xlabel('Frames');
set(gca,'fontsize',20)
%title(['Person ' num2str(j) ' Sequence ' num2str(k)]);
%%

% Finding the peaks to be used for finding gait cycle
[peaks,loc,width,prom] = findpeaks(JRD,1:size(JRD,1),'MinPeakProminence',0.25, 'MinPeakWidth', 7.4, 'MaxPeakWidth', 16);

% Checking to see if gait cycle is valid
if size(loc) < 3
   disp(['Invalid Data for person ' num2str(j) ' sequence ' num2str(k)]);
   GaitCycleSpeed(j,k) = 0;
   
else   
FrameStart = loc(1);
FrameEnd = loc(3);

%% Add lines to represent where gait cycle starts and stops (DEMO)
line([FrameStart FrameStart], get(gca, 'YLim'),'Color',[.5 .5 .5],'LineStyle','--','LineWidth',3);
line([FrameEnd FrameEnd], get(gca, 'YLim'),'Color',[.5 .5 .5],'LineStyle','--','LineWidth',3);
%%
% Finding Gait Cycle
GaitCycleSpeed(j,k) = (FrameEnd - FrameStart)/30;

%% (DEMO)
disp(['The Average Gait Cycle Speed for person ' num2str(j) ' sequence ' num2str(k) ' is: ' num2str(GaitCycleSpeed(j,k))]);

% end
% end
end

%% KneeAngles
% Finding the geometric positions of the left ankle
EndPoint = size(sequence,1);    % Finding Number of rows
n = 16;                         % Numeric value of Ankle Left
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

% Finding JRA for Left Knee
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

%% (DEMO)
%subplot(2,1,2);
plot(angledegL,'LineWidth',4);
hold on

%%
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

% Initializing
Status = 'Unknown';

% If sequence is invalid
if GaitCycleSpeed(j,k) == 0
    Status = 'Unknown'; 
% If sequence is valid
elseif MinangledegR >= min_angle
    Status = 'Right';
elseif MinangledegL >= min_angle
    Status = 'Left';
elseif MinangledegR && MinangledegL >= min_angle
    Status = 'Left and Right';
elseif MinangledegR && MinangledegL <= min_angle
    Status = 'No Limp';
end

% EXAMPLE TRAINING SET
% Person    Sequence   Min_Right_Knee     Min_Left_Knee     Limp(R/L/N)
%   1           1          ###                ###               L/R/N
%   1           2          ###                ###               L/R/N
%   1           3          ###                ###               L/R/N
% ...


% Creating tables for the data and corresponding gait status
Data(run,:) = [j k MinangledegR MinangledegL];
Result(run,:) = [Status];
run = run + 1;

%% (DEMO)
plot(angledegR,'LineWidth',4);
if size(loc) < 3
legend('Left Knee Angle','Right Knee Angle','Location','southwest');
axis([0 100 0 200]);
ylabel('Angle in Degrees');
xlabel('(b)');
else
line([FrameStart FrameStart], [0 200],'Color',[.5 .5 .5],'LineStyle','--','LineWidth',3);
line([FrameEnd FrameEnd], [0 200],'Color',[.5 .5 .5],'LineStyle','--','LineWidth',3);
legend('Left Knee Angle','Right Knee Angle','Location','southwest');
axis([0 100 0 200]);
ylabel('Angle in Degrees');
xlabel('Frames');
end
set(gca,'fontsize',20)

    end     % INSERT BREAKPOINT HERE FOR DEMO
close all;
end

% Creating Headers when finished
Data = array2table(Data,'VariableNames',{'Person','Sequence','Min_R_Knee_Angle','Min_L_Knee_Angle'});
Result = cellstr(Result);
Result = cell2table(Result,'VariableNames',{'Status'});
TrainingData = [Data Result];

