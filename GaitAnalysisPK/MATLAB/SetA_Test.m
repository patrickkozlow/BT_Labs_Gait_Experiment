%Code to Predict using Set A

pred = Training_Set_Paper(1:end,3:4);   %Setting the predictor variables
pred = table2array(pred);
R = table2cell(Training_Set_Paper(1:end,5));    %Change to Combined_TData
resp = strcmp(R,'No Limp');         %No Limp = 1, Limp = 0

MdlDA = fitcdiscr(pred,resp);
%labelA = predict(MdlDA,A);

%Scatter Plot
%No Limp == Group 1
%Left == Group 2 
%Right == Group 3
group = table2array(Training_Set_Paper(1:end,6));
x = table2array(Training_Set_Paper(1:end,3));
y = table2array(Training_Set_Paper(1:end,4));

plot(x(group==1),y(group==1),'.','markersize',20,'color','blue');
hold on
plot(x(group==2),y(group==2),'.','markersize',20,'color','red');
plot(x(group==3),y(group==3),'.','markersize',20,'color','green');
title('Scatter Plot of Gait Data');
xlabel('Right Knee Angle');
ylabel('Left Knee Angle');
%axis([100 160 100 180]);
grid on

