# Probabilities for accepting an increase of 1 with probability 0.8, 0.85, 0.9 and 0.95
init_temps = [4.5, 6.2, 9.5, 19.5]
decr_coefs = [0.998, 0.9985, 0.999, 0.9995]
# Probabilities for accepting an increase of 1 with probability 0.04, 0.08, 0.12 and 0.16
final_temps = [0.31, 0.4, 0.47, 0.55]

with open("temperature_list.csv", "w") as temperature_file:                
    for init_temp in init_temps:
        for decr_coef in decr_coefs:
            for final_temp in final_temps:
                temperature_file.write(str(init_temp) + " " + str(decr_coef) + " " + str(final_temp) + "\n")
