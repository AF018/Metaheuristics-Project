import os

radius_file = open("radius_list.csv", "r")
radius_values_list = radius_file.read().splitlines()
test_instance_file = open("test_instance_list.csv", "r")
instance_names_list = test_instance_file.read().splitlines()
temperature_file = open("temperature_list.csv", "r")
temperature_values_list = temperature_file.read().splitlines()

for radius_values in radius_values_list:
    capt_radius, comm_radius = radius_values.split(" ")
    for instance_name in instance_names_list:
        for temperature_values in temperature_values_list:

            with open("run_parameters.dat", "w") as parameters_file:                
                with open("test_parameters_template.dat", "r") as template_parameters_file:
                    for template_line in template_parameters_file:
                        parameters_file.write(template_line.replace("INSTANCE_NAME", instance_name).replace("CAPT_RADIUS", capt_radius).replace("COMM_RADIUS", comm_radius).replace("TEMPERATURE_VALUES", temperature_values))
            os.system("echo " + instance_name + " RADIUS " + radius_values + " TEMPERATURE " + temperature_values)
            os.system("Metaheuristics_solution\\Debug\\Metaheuristics_solution.exe")