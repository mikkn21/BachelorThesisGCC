import os
import pandas as pd
import time

trials = 3

current_directory = os.getcwd()
files = os.listdir(current_directory)
chad_files = [file for file in files if file.endswith('.chad')]

flags = ["", "-n", "-E", "-nE"]
map_flag = {
    "": "",
    "-n": "Naive",
    "-E": "no Peephole",
    "-nE": "Naive + no Peephole",
}
data_frame = pd.DataFrame(columns=['size', 'name', 'time'])
for file in chad_files:
    for flag in flags:
        pre_command = f"./../build/giga {flag} {file}"
        assemble_command = "as chad.s -o chad.o"
        link_command = "ld chad.o -o chad"
        run_command = "./chad"
        os.system(pre_command)
        os.system(assemble_command)
        os.system(link_command)
        all_times = []
        for i in range(trials):

            now_time = time.time()
            os.system(run_command)
            end_time = time.time()
            all_times.append(end_time - now_time)
            print(f"trial {i + 1} of {trials} for {file} with flag {flag} done...")
        size = file.split('_')[-1].split('.')[0]
        name = f"{file.split('_')[0]} {map_flag[flag]}"
        data_frame = data_frame.append({'size': size, 'name': name, 'time': sum(all_times) / trials}, ignore_index=True)
        print(f"result of benchmark for {file} with flag {flag}: {sum(all_times) / trials} seconds...")
    
    print(f"done benchmarking {file}...")
data_frame.to_csv('benchmark.csv', index=False)
#save data frame as dat file
data_frame.to_csv('benchmark.dat', sep='\t', index=False)
