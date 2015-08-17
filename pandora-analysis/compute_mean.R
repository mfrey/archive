data <- read.csv(file="route_pheromone_data.csv",header=FALSE);

# experiment id
experiment <- c(as.numeric(data[,1]));
# repetition in an experiment
repetition <- c(as.numeric(data[,2]));
# alpha parameter for the experiment
alpha <- c(as.numeric(data[,4]));
# beta parameter for the experiment
beta <- c(as.numeric(data[,5]));
# packet id
packet <- c(as.numeric(data[,3]));
# the id of the route
route <- c(as.numeric(data[,6]));
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
        last_packet <- -1
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

          # DEBUG: cat(sprintf("p: %i, counter: %i\n", p, counter));

          #only add packets which have a counter higher than 1
          if(counter > 1){
            result <- rbind(result, data.frame(experiment = unique(experiment)[e], alpha = current_alpha, beta = current_beta, repetition = unique(experiment_data$repetition)[r], packet = unique(repetitionData$packet)[p], active = counter));
            last_packet <- unique(repetitionData$packet)[p]
          }
        }
        
        temp_result_mean <- rbind(temp_result_mean, data.frame(experiment = unique(experiment)[e], repetition = unique(experiment_data$repetition)[r], alpha = current_alpha, beta = current_beta, packet = last_packet));
      }
      # DEBUG: cat(sprintf("experiment: %i, alpha: %f, beta: %f\n",unique(experiment)[e], current_alpha, current_beta));
      subset_temp_result_mean <- subset(temp_result_mean, temp_result_mean$experiment == unique(experiment)[e] & temp_result_mean$alpha == current_alpha & temp_result_mean$beta == current_beta);
      meanValue <- mean(subset_temp_result_mean$packet)
      result_mean <- rbind(result_mean, data.frame(experiment = unique(experiment)[e], alpha = current_alpha, beta = current_beta, packet = meanValue));
    }
  }

  write.table(result_mean, file="route_pheromone_mean.csv", append=FALSE, quote=FALSE, 
    sep=",", eol="\n", na="NA", dec=".", row.names=FALSE, col.names=TRUE,
    qmethod="escape");
}

