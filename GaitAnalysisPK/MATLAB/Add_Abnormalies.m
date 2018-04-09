%Combining KneeAngles+GaitCycle For Personal Database
%By: Patric Kozlow
%Biometric Technology Labs, University of Calgary

%% GaitCycle
% Change to load proper sequence

%function Updated_T_D = Add_Abnormalies(TrainingData)
clear;
close all;

subjects = 2;
sequences = 6;

min_angle = 150;
person = 1;
run = 1;

for sub_num = 1:subjects
    for run_num = 1:sequences
        
%subsec = ['seq' num2str(seq_num) '.txt'];
subsec1 = ['Subject_' num2str(sub_num) '_Seq_' num2str(run_num) '_GaitCycle_N.csv'];
sequence = readtable(subsec1);       %Loading gait subsection of database
GaitCycleData = sequence{:,1};
GaitCycleData = smooth(GaitCycleData);

% plotting the filtered graph
hFig = figure(run);
set(hFig, 'Position', [600,200,600,600]);
%subplot(2,1,1);
plot(GaitCycleData,'LineWidth',4);
axis([0 100 0 0.8]);
ylabel('\Delta between ankles');
xlabel('Frames');
set(gca,'fontsize',20)
%title(['Person ' num2str(sub_num) ' Sequence ' num2str(run_num)]);

FrameStart = 1;
FrameEnd = size(GaitCycleData,1);

% Finding the peaks to be used for finding gait cycle
[peaks,loc,width,prom] = findpeaks(GaitCycleData(10:FrameEnd),10:size(GaitCycleData,1),'MinPeakProminence',0.13, 'MinPeakWidth', 7.4, 'MaxPeakWidth', 18);

% Checking to see if gait cycle is valid
if size(loc) < 3
   disp(['Invalid Data for person ' num2str(sub_num) ' sequence ' num2str(run_num)]);
   GaitCycleSpeed(sub_num,run_num) = 0;
   
else   
FrameStart = loc(1);
FrameEnd = loc(3);

% Add lines to represent where gait cycle starts and stops (DEMO)
line([FrameStart FrameStart], get(gca, 'YLim'),'Color',[.5 .5 .5],'LineStyle','--','LineWidth',3);
line([FrameEnd FrameEnd], get(gca, 'YLim'),'Color',[.5 .5 .5],'LineStyle','--','LineWidth',3);

% Finding Gait Cycle
GaitCycleSpeed(sub_num,run_num) = (FrameEnd - FrameStart)/30;

disp(['The Average Gait Cycle Speed for person ' num2str(sub_num) ' sequence ' num2str(run_num) ' is: ' num2str(GaitCycleSpeed(sub_num,run_num))]);
end
%% KneeAngles
%Finding the geometric positions of the left ankle

subsec2 = ['Subject_' num2str(sub_num) '_Seq_' num2str(run_num) '_JointAngles_N.csv'];
sequence = readtable(subsec2);       %Loading gait subsection of database
AngleData = sequence{:,[1,2]};

AngleDataL = AngleData(:,1);
AngleDataR = AngleData(:,2);

AngleDataL = smooth(AngleDataL);
AngleDataR = smooth(AngleDataR);

%subplot(2,1,2);
plot(AngleDataL,'LineWidth',4);
hold on
plot(AngleDataR,'LineWidth',4);
set(gca,'fontsize',20)

angle_LK_T = AngleDataL(FrameStart:FrameEnd);
angle_RK_T = AngleDataR(FrameStart:FrameEnd);

% Finding lowest point in the gait cycle
MinangleLK = min(angle_LK_T);
MinangleRK = min(angle_RK_T);

% Initializing
Status = 'Unknown';

% If sequence is invalid
% If sequence is valid
if MinangleRK >= min_angle
    Status = 'Right';
end
if MinangleLK >= min_angle
    Status = 'Left';
end
if MinangleRK >= min_angle && MinangleLK >= min_angle
    Status = 'Left and Right';
end
if MinangleRK <= min_angle && MinangleLK <= min_angle
    Status = 'No Limp';
end
if GaitCycleSpeed(sub_num,run_num) == 0
    Status = 'Unknown';
end

%Adding to Table
Data_BT(run,:) = [sub_num run_num MinangleRK MinangleLK];
Status_str = string(Status);
Result_BT(run,1) = [Status_str];
run = run + 1;


line([FrameStart FrameStart], [0 200],'Color',[.5 .5 .5],'LineStyle','--','LineWidth',3);
line([FrameEnd FrameEnd], [0 200],'Color',[.5 .5 .5],'LineStyle','--','LineWidth',3);
legend('Left Knee Angle','Right Knee Angle','Location','southwest');
axis([0 100 0 200]);
ylabel('Angle in Degrees');
xlabel('Frames');

    end
    close all;
end

Data_BT = array2table(Data_BT,'VariableNames',{'Person','Sequence','Min_R_Knee_Angle','Min_L_Knee_Angle'});
Result_BT = cellstr(Result_BT);
Result_BT = cell2table(Result_BT,'VariableNames',{'Status'});
Test_B = [Data_BT Result_BT];
