import subprocess
import csv

sizes = [10, 15, 20, 25]
threads = [2, 5, 10, 15, 20]

with open('results.csv', 'w') as csvfile:
    fieldnames = ['n', 'processes', 'time']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

    writer.writeheader()
    for s in sizes:
        for t in threads:
            time = subprocess.check_output(['mpiexec', '-n', str(t), './tsp', 'tests/' + str(s) + '.mtx','--no-output'])
            print(float(time))
            writer.writerow({'n': s, 'processes': t, 'time': float(time)})
