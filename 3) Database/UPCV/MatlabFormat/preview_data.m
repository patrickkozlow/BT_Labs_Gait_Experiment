%
%
% Supplementary function for ploting gait sequences.
% Author: Dimitris Kastaniotis, 
% 
% University of Patras Computer vision Group, 
% Electronics laboratory, Department of Physics, Patras, Greece
% http://upcv.upatras.gr/
%
load 'UPCVgait.mat' upcv
connections % load connections
 
 

for m=1:30
    for rep=1:5
        data = upcv{m,rep};
        len=size(data,1)/20;
        figure;view(3);hold on
        fprintf('Person %d sequence %d\n',m,rep);
        for i=1:len
        clf;
        
        hold on; axis([-3 3 -2 2 -3 3]);
        view([-172.5, -68]);
        plot3(data((i-1)*20+1:i*20,1),data((i-1)*20+1:i*20,2),data((i-1)*20+1:i*20,3),'.');
        hold on
        
   
     
        for k=1:length(jcon)
            line( [data((i-1)*20+jcon(k,1),1), data((i-1)*20+jcon(k,2),1)],...
                [data((i-1)*20+jcon(k,1),2),data((i-1)*20+jcon(k,2),2)],...
                [data((i-1)*20+ jcon(k,1),3),data((i-1)*20+ jcon(k,2),3)]);
            
        end
        
        for jj=1:20
        text(data((i-1)*20+jj,1),data((i-1)*20+jj,2),data((i-1)*20+jj,3),num2str(jj));
        end
        
        pause;
        end
    end
end