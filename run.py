import os

path = "/Users/wmhst7/Desktop/SysCourse/exp1/SimpleCache/test_trace/"
outpath = "/Users/wmhst7/Desktop/SysCourse/exp1/SimpleCache/out/"
files = ["1.trace", "2.trace", "3.trace", "4.trace"]
block_size = [8, 32, 64]
pos = ['PLRU', 'LRU', 'RANDOM']
was = ['a', 'n']
wns = ['1', '4', '8']

os.system('make')
# block_size way_num wa policy path outpath(None)

bs = '8'
wn = '8'
wa = 'a'
po = 'PLRU'

for filename in files:
    
    cmd = './main {} {} {} {} '.format(bs, wn, wa, po) + path+filename+' '+outpath+filename.replace('trace', 'log')
    # print(cmd)
    os.system(cmd)
    print(" ", bs, " ", wn, ' ', wa, po, filename)
    