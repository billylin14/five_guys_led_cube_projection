import numpy as np

name = input("Insert size for the cube: ")
# Open a file for writing
file = open("/Users/vchendrawan/Desktop/five_guys_led_cube_projection/Coordinates/Cube_"+ str(name) + ".txt", "w")
size = int(name)

# Write the cube coordinates to the file
for i in range(size):
	for j in range(size):
		for k in range(size):
			file.write(str(i) + " " + str(j) + " " + str(k) + "\n")


# Close the file
file.close()
