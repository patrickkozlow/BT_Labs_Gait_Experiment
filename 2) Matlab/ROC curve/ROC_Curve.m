%Code to Plot ROC curves
%By: Patrick Kozlow
%Biometric Technologies Lab, University of Calgary

%load Combined_TData.mat             %Loading Data
pred = Combined_TData(1:end,3:4);   %Setting the predictor variables
pred = table2array(pred);           %Converting to array
R = table2cell(Combined_TData(1:end,5));    %Change to Combined_TData
resp = strcmp(R,'No Limp');         %No Limp = 1, Limp = 0

%Fitting a Discriminant Analysis model
CVmdlDA = fitcdiscr(pred,resp,'KFold',5)
[labelDA,score_DA] = kfoldPredict(CVmdlDA);
ConfusionTrainDA = confusionmat(resp,labelDA)
Loss_DA = kfoldLoss(CVmdlDA);
Accuracy_DA = (1 - Loss_DA) * 100

%ConfusionMatrixPlot
resp_double = +resp;
da_double = +labelDA;
resp_double = transpose(resp_double);
da_double = transpose(da_double);
plotconfusion(resp_double,da_double);
set(gca,'fontsize',16);
set(findobj(gca,'type','text'),'fontsize',16) 

%Training an SVM classifier
CVmdlSVM = fitcsvm(pred,resp,'Standardize',true,'KFold',5);
%CVmdlSVM = fitPosterior(CVmdlSVM);
[labelSVM,score_svm] = kfoldPredict(CVmdlSVM);
ConfusionTrainSVM = confusionmat(resp,labelSVM);
Loss_SVM = kfoldLoss(CVmdlSVM);
Accuracy_SVM = (1 - Loss_SVM) * 100

%Training a naive Bayes classifier
CVmdlNB = fitcnb(pred,resp,'KFold',5);
[labelNB,score_nb] = kfoldPredict(CVmdlNB);
ConfusionTrainNB = confusionmat(resp,labelNB);
Loss_NB = kfoldLoss(CVmdlNB);
Accuracy_NB = (1 - Loss_NB) * 100

%Training a KNN classifier
%mdlKNN = fitcknn(pred,R,'NumNeighbors',5,'Standardize',true);
%[~,score_knn] = resubPredict(mdlKNN);

%Cross-Validation 5 folds
CVmdlKNN = fitcknn(pred,R,'NumNeighbors',10,'Standardize',true,'KFold',5);
[labelKNN,score_knn] = kfoldPredict(CVmdlKNN);
%ConfusionTrainKNN = confusionmat(resp,labelKNN);
Loss_KNN = kfoldLoss(CVmdlKNN);
Accuracy_KNN = (1 - Loss_KNN) * 100

%Computing the ROC curve
[Xd,Yda,Tda,AUCda] = perfcurve(resp,score_DA(:,CVmdlSVM.ClassNames),'true');
[Xsvm,Ysvm,Tsvm,AUCsvm] = perfcurve(resp,score_svm(:,CVmdlSVM.ClassNames),'true');
[Xnb,Ynb,Tnb,AUCnb] = perfcurve(resp,score_nb(:,CVmdlNB.ClassNames),'true');
[Xknn,Yknn,Tknn,AUCknn] = perfcurve(resp,score_knn(1:end,2),'true');

%Plotting
%semilogx(Xd,Yda,'LineWidth',4)
plot(Xd,Yda,'LineWidth',2)
hold on
%semilogx(Xsvm,Ysvm,'LineWidth',4)
plot(Xsvm,Ysvm,'LineWidth',2)
%semilogx(Xnb,Ynb,'LineWidth',4)
plot(Xnb,Ynb,'LineWidth',2)
%semilogx(Xknn,Yknn,'LineWidth',4)
plot(Xknn,Yknn,'LineWidth',2)

grid on
%ylim([0.75 1]);
ylim([0 1])
set(gca,'fontsize',16)
legend('Linear Discriminant Analysis','Support Vector Machines','Naive Bayes','KNN (10 Neighbors)','Location','Best')
xlabel('False positive rate'); ylabel('True positive rate');
%title('ROC Curves for Logistic Regression, SVM, and Naive Bayes Classification')
hold off