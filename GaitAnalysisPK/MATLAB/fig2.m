x = linspace(0,100,1000);
y1 = sin(x/3.5)/5 +0.4;
figure
plot(x,y1,'LineWidth',4);
axis([0 100 0 0.8]);
xlabel('Frames');
ylabel('\Delta between ankles');
set(gca,'fontsize',16)