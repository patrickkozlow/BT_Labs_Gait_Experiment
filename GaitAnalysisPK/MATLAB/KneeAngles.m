% Finding Knee Angle Plot
% Getting Positions of Left/Right Ankle, Hip and Knee

% Joint ID
% 1. Head            6. Wrist Left 		11. Spine       16. Ankle Left
% 2. Shoulder Center 7. Hand Left 		12. Hip Center 	17. Foot Left
% 3. Shoulder Right  8. Elbow Right 	13. Hip Right 	18. Knee Right
% 4. Shoulder Left 	 9. Wrist Right 	14. Hip Left 	19. Ankle Right
% 5. Elbow Left 	10. Hand Right 	    15. Knee Left 	20. Foot Right
clear;
sequence = load('seq1.txt');       %Loading subsection of database

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
plot(angledegR);
legend('Left Knee Angle','Right Knee Angle','Location','southwest');
ylim([0 200]);
hold off

