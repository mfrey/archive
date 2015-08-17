library(lattice);
library(plyr,lib.loc="~/.R/library");

data <- read.csv(file="final-5.csv",header=FALSE);

experiment <- c(as.numeric(data[,1]));
repetition <- c(as.numeric(data[,2]));
alpha <- c(as.numeric(data[,3]));
beta <- c(as.numeric(data[,4]));
jumps <- c(as.numeric(data[,5]));

plotData <- data.frame(experiment, repetition, alpha, beta, jumps);

result <- data.frame()

for(a in 1:length(unique(alpha))){
  for(b in 1:length(unique(beta))){
    current_alpha = unique(alpha)[a]
    current_beta = unique(beta)[b]
    temp <- subset(plotData, plotData$alpha == current_alpha & plotData$beta == current_beta);
    temp_result <- mean(temp[,5])
    result <- rbind(result, data.frame(alpha = current_alpha, beta = current_beta, mean = temp_result))
  }
}

png(file=paste("mean_jumps.png"),width=1280, height=1280);
meanJumpPlot <- wireframe(result$mean ~ result$alpha * result$beta, data=result,
  scales = list(arrows=FALSE, cex= .45, col = "black", font = 3),
  #scales = list(arrows=FALSE, cex= .45, col = "black", font = 3, x = list(labels = seq(1.0, 5.0, by = 0.5)),
  #y = list(labels = seq(1.0, 5.0, by = 0.5))),
  drape = TRUE, colorkey = TRUE, main = "Mean Jumps per Alpha/Beta",
  # set the color
  col.regions = terrain.colors(100),
  # rotate the figure
  screen = list(z = -60, x = -60),
  # set the labels of the axis
  xlab="alpha", ylab="beta", zlab="mean \n jumps");
print(meanJumpPlot);
dev.off();

