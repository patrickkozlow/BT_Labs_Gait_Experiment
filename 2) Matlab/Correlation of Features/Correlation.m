%% Finding Correlation between Cadence & Stride Length
% By: Patrick Kozlow

%% Part 1) For UPCV data set
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
GaitCycleSpeed(j,k) = 2*30*60/(FrameEnd - FrameStart);    %2steps x 30fps x 60s/m x 1/#frames

%% (DEMO)
disp(['Cadence for person ' num2str(j) ' sequence ' num2str(k) ' is: ' num2str(GaitCycleSpeed(j,k))]);

%% Calculate Stride length for left + right leg
StrideRight = AnkleRight(FrameStart,3) - AnkleRight(FrameEnd,3);
StrideLeft = AnkleLeft(FrameStart,3) - AnkleLeft(FrameEnd,3);
StrideLengthLeft(j,k) = StrideLeft;
StrideLengthRight(j,k) = StrideRight;

end
    end
    close all;
end
for s = 1:5
    scatter(StrideLengthLeft(:,s),GaitCycleSpeed(:,s),'blue');
    hold on
end
%% Part 2) For Ucalgary Dataset

        
