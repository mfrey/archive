library(ggplot2);
library(lattice);

data <- read.csv(file="example.csv",header=FALSE);

# experiment id
experiment <- c(as.numeric(data[,1]));
# repetition in an experiment
repetition <- c(as.numeric(data[,2]));
# alpha parameter for the experiment
alpha <- c(as.numeric(data[,3]));
# beta parameter for the experiment
beta <- c(as.numeric(data[,4]));
# packet id
packet <- c(as.numeric(data[,6]));
# the id of the route
route <- c(as.numeric(data[,5]));
# the phi value of the route
phi <- c(as.numeric(data[,7]));

plotData <- data.frame(experiment, repetition, alpha, beta, packet, route, phi);
result <- data.frame();
result_mean <- data.frame();
temp_result_mean <- data.frame();
subset_temp_result_mean <- data.frame();

# iterate over the experiments
for(e in 1:length(unique(experiment))){
  for(a in 1:length(unique(alpha))){
    for(b in 1:length(unique(beta))){
      # save the alpha value
      current_alpha = unique(alpha)[a]
      # save the beta value
      current_beta = unique(beta)[b]
      # create a subset for each experiment
      experiment_data <- subset(plotData, plotData$experiment == unique(experiment)[e] & plotData$alpha == current_alpha & plotData$beta == current_beta);
     
      # iterate over the repetitions
      for(r in 1:length(unique(experiment_data$repetition))){
        # iterate over the current repetition
        repetitionData <- subset(experiment_data, experiment_data$repetition == unique(experiment_data$repetition)[r]);

        for(p in 1:length(unique(repetitionData$packet))){
          # DEBUG: cat(sprintf("packet: %i, p: %i \n", unique(repetitionData$packet)[p], p));
          counter <- 0;

          temp <- subset(repetitionData, repetitionData$packet == unique(repetitionData$packet)[p])
          # DEBUG: cat(sprintf("number of rows in temp data.frame: %i\n",nrow(temp)));

          if(temp[1,7] > 0.09){
            # DEBUG: cat(sprintf("temp[1,7]: %f\n",temp[1,7]));
            counter <- counter + 1;
          }

          if(temp[2,7] > 0.09){
            # DEBUG: cat(sprintf("temp[2,7]: %f\n",temp[2,7]));
            counter <- counter + 1;
          }

          # DEBUG: cat(sprintf("number of available routes: %i\n",counter));

          # determine which was the last packet with the number of routes greater than one
          if(p > 1 & counter < 2){
            previous <- subset(result, experiment == unique(experiment)[e] & alpha == current_alpha & beta == current_beta & repetition == unique(experiment_data$repetition)[r] & packet == unique(repetitionData$packet)[p]-1);
            # check if the previous data set has an higher active count than one
            if(previous[1,6] > 1){
			  temp_result_mean <- rbind(temp_result_mean, data.frame(experiment = repetitionData$experiment, alpha = repetitionData$alpha, beta = repetitionData$beta, repetition = repetitionData$repetition, packet = unique(repetitionData$packet)[p]-1));
            }
          }
          result <- rbind(result, data.frame(experiment = unique(experiment)[e], alpha = current_alpha, beta = current_beta, repetition = unique(experiment_data$repetition)[r], packet = unique(repetitionData$packet)[p], active = counter));
        }
      }
      # DEBUG: cat(sprintf("experiment: %i, alpha: %f, beta: %f\n",unique(experiment)[e], current_alpha, current_beta));
      subset_temp_result_mean <- subset(temp_result_mean, temp_result_mean$experiment == unique(experiment)[e] & temp_result_mean$alpha == current_alpha & temp_result_mean$beta == current_beta);
      meanValue <- mean(subset_temp_result_mean$packet)
      result_mean <- rbind(result_mean, data.frame(experiment = unique(experiment)[e], alpha = current_alpha, beta = current_beta, packet = meanValue));
    }
  }

  # this plot shows the mean number of packets until only one route remains
  png(file=paste("network_lifetime_mean",experiment,".png"),width=1280, height=1280);
  meanNetworkLifetimePlot <- wireframe(result_mean$packet ~ result_mean$alpha * result_mean$beta, data=result_mean,
    scales = list(arrows=FALSE, cex= .45, col = "black", font = 3),
#x = list(labels = seq(1.0, 1.0, by = 1.0)),
#    y = list(labels = seq(1.0, 1.0, by = 1.0))),
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

