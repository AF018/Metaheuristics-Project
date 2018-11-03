import os

radius_file = open("radius_list.csv", "r")
radius_values_list = radius_file.read().splitlines()
instance_file = open("instance_list.csv", "r")
instance_names_list = instance_file.read().splitlines()

for radius_values in radius_values_list:
    capt_radius, comm_radius = radius_values.split(" ")
    for instance_name in instance_names_list:

        with open("run_parameters.dat", "w") as parameters_file:                
            with open("parameters_template.dat", "r") as template_parameters_file:
                for template_line in template_parameters_file:
                    parameters_file.write(template_line.replace("INSTANCE_NAME", instance_name).replace("CAPT_RADIUS", capt_radius).replace("COMM_RADIUS", comm_radius))
        os.system("echo " + instance_name + " " + radius_values)
        os.system("Metaheuristics_solution\\Debug\\Metaheuristics_solution.exe")