#! /usr/bin/env python2
# coding=utf-8
# Copyright © Advanced Micro Devices, Inc., or its affiliates.

import os
import sys

git_ids = []
try:
    while True:
        sha = raw_input().split()
        git_ids.insert(0,sha[0])
except EOFError:
    pass

build_command = "ninja clean; ninja amd-llvm"
test_command = "ninja"
cherry_pick_dir = "../compiler/amd-llvm"

if len(sys.argv) > 1:
    build_command = sys.argv[1]

if len(sys.argv) > 2:
    test_command = sys.argv[2]

if len(sys.argv) > 3:
    cherry_pick_dir = sys.argv[3]

#Inclusive bounds
lower_bound = 0
upper_bound = len(git_ids)-1
ftb_shas = set()

def only_ftb_shas_between(l,u):
    for i in range(l+1,u):
        if git_ids[i] not in ftb_shas:
            return False
    return True

def output_bounds():
    print("Latest passing SHA: "+git_ids[lower_bound])
    print("Earliest failing SHA: "+git_ids[upper_bound])


os.system("pushd "+cherry_pick_dir+"; x=`git rev-parse HEAD`; popd; echo $x > base_sha.txt")
base_sha=open("base_sha.txt").read().rstrip()
print("Base SHA, so you have it in case anything goes wrong: "+base_sha)

while lower_bound + 1!= upper_bound and not only_ftb_shas_between(lower_bound,upper_bound):
    current_sha_idx = (lower_bound + upper_bound)/2
    while current_sha_idx <= upper_bound and git_ids[current_sha_idx] in ftb_shas:
        current_sha_idx+=1
    while current_sha_idx >= lower_bound and git_ids[current_sha_idx] in ftb_shas:
        current_sha_idx-=1

    current_sha = git_ids[current_sha_idx]
    if current_sha in ftb_shas:
        output_bounds()
        exit(0)

    #Cherry pick the necessary commits
    os.system("cd "+cherry_pick_dir+"; git reset --hard "+base_sha+"; git cherry-pick --abort")
    for idx in range(0,current_sha_idx+1):
        if os.system("cd "+cherry_pick_dir+"; git cherry-pick --empty=drop "+git_ids[idx])!=0:
            print("FAILED TO APPLY CHERRY PICK: "+git_ids[idx])
            exit(1)

    #Cherry picks completed

    #Fails to build?  Bad SHA!
    if os.system(build_command)!=0:
        ftb_shas.add(current_sha)
        continue

    #Now run test CMD
    if os.system(test_command)==0:
        #Good SHA.  Lower bound should be moved up to it.
        lower_bound = current_sha_idx
    else:
        #Bad SHA.  Upper bound should be moved down to it.
        upper_bound = current_sha_idx

output_bounds()
