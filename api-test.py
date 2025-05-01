#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
import file2mat


# In[3]:


test1 = np.fromfile("matrices/wav/file_001.wav.bin", dtype=np.uint32).reshape(256, 256)
test2 = file2mat.signature_from_filepath_by2("data/music/wav/file_001.wav", 1)
np.all(test2 == test1)


# In[4]:


# !./build.sh
test2 = file2mat.signature_from_filepath_by2("data/music/wav/file_001.wav1", 1)
# FileNotFoundError

