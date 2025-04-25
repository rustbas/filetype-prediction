import numpy as np
from tqdm import tqdm

INPUT_FILE = "data/music/wav/file_001.wav"
OUTPUT_FILE = "res.bin"

MSIZE = 256

matrix = np.zeros((256, 256), dtype=np.uint32)

with open(INPUT_FILE, "rb") as f:
    raw_data = f.read()

n = len(raw_data)
    
for i in tqdm(range(n-1)):
    idx1 = raw_data[i]
    idx2 = raw_data[i+1]
        
    matrix[idx1, idx2] += 1

matrix.tofile(OUTPUT_FILE)
