library(plyr);
library(ggplot2);

data <- read.csv(file="route_pheromone_log.csv",header=FALSE);

# experiment id
experiment <- c(as.numer(data[,1]));
# repetition in an experiment
repetition <- c(as.numer(data[,1]));
# alpha parameter for the experiment
alpha <- c(as.numer(data[,1]));
# beta parameter for the experiment
beta <- c(as.numer(data[,1]));
# packet id
packet <- c(as.numeric(data[,1]));
# the id of the route
route <- c(as.numeric(data[,2]));
# the phi value of the route
phi <- c(as.numeric(data[,3]));

plotData <- data.frame(experiment, repetition, alpha, beta, packet, route, phi);
result <- data.frame();
result_mean <- data.frame();
temp_result_mean <- data.frame();

# iterate over the experiments
for(e in 0:length(unique(experiment)){
  # create a subset for each experiment
  experimentData <- subset(plotData, plotData$experiment == e);
  # iterate over the alpha paramter
  for(a in 1:length(unique(alpha))){
    # iterate over the beta paramter
    for(b in 1:length(unique(beta))){
      # save the alpha value
      current_alpha = unique(alpha)[a]
      # save the beta value
      current_beta = unique(beta)[b]
      # build up the experiment subset (including the alpha and beta parameter)
      specificExperimentData <- subset(experimentData, experimentData$alpha == current_alpha & experimentData$beta == current_beta);
     
      # iterate over the repetitions
      for(r in 1:length(unique(specificExperimentData$repetition))){
        # iterate over the current repetition
        repetitionData <- subset(specificExperimentData, specificExperimentData$repetition == r);

        for(p in 1:length(unique(repetitionData$packet))){
          counter <- 0;

          temp <- subset(repetitionData, repetitionData$packet == p)
          if(temp[1,3] > 0.09){
            counter <- counter + 1;
          }

          if(temp[2,3] > 0.09){
            counter <- counter + 1;
          }

          # determine which was the last packet with the number of routes greater than one
          if(p > 1 & counter < 2){
            previous <- subset(result, experiment == repetitionData$experiment & alpha == repetitionData$alpha & beta == repetitionData$beta & repetition == repetitionData$repetition & packet = p-1);
            # check if the previous data set has an higher active count than one
            if(previous$active > 1){
              temp_result_mean <- rbind(temp_result_mean, data.frame(experiment = repetitionData$experiment, alpha = repetitionData$alpha, beta = repetitionData$beta, repetition = repetitionData$repetition, packet = p-1, active = counter));
            }
          }

          result <- rbind(result, data.frame(experiment = repetitionData$experiment, alpha = repetitionData$alpha, beta = repetitionData$beta, repetition = repetitionData$repetition, packet = p, active = counter));
        }
      }
      subset_temp_result_mean <- subset(temp_result_mean, temp_result_mean$experiment == e & temp_result_mean$alpha == current_alpha & temp_result_mean$beta == current_beta);
      meanValue <- mean(subset_temp_result_mean$packet)
      result_mean <- rbind(result_mean, data.frame(experiment = specificExperimentData$experiment, alpha = current_alpha, beta = current_beta, packet = meanValue));
    }
  }

  # this plot shows the mean number of packets until only one route remains
  png(file=paste("network_lifetime_mean",experiment,".png"),width=1280, height=1280);
  meanNetworkLifetimePlot <- wireframe(result_mean$packet ~ result_mean$alpha * result_mean$beta, data=result_mean,
    scales = list(arrows=FALSE, cex= .45, col = "black", font = 3),
    #scales = list(arrows=FALSE, cex= .45, col = "black", font = 3, x = list(labels = seq(1.0, 5.0, by = 0.5)),
    #y = list(labels = seq(1.0, 5.0, by = 0.5))),
    drape = TRUE, colorkey = TRUE, main = "Mean Packets (Network Lifetime)  per Alpha/Beta",
    # set the color
    col.regions = terrain.colors(100),
    # rotate the figure
    screen = list(z = -60, x = -60),
    # set the labels of the axis
    xlab="alpha", ylab="beta", zlab="mean \npackets");
    print(meanNetworkLifetimePlot);
  dev.off();
}

